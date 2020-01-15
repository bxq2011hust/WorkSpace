package main

import (
	// "crypto"
	// "crypto/ecdsa"
	// "crypto/tls"
	// "crypto/x509"
	// "crypto/x509/pkix"
	"encoding/asn1"
	"encoding/pem"
	"errors"
	"fmt"
	"io/ioutil"
	"math/big"
	"os"
	"sync"

	"github.com/bxq2011hust/fisco-tls/crypto/ecdsa"
	"github.com/bxq2011hust/fisco-tls/crypto/elliptic"
	"github.com/bxq2011hust/fisco-tls/crypto/tls"
	"github.com/bxq2011hust/fisco-tls/crypto/x509/pkix"
)

var initonce sync.Once
var sm2p256v1 *elliptic.CurveParams

var (
	oidNamedCurveP224      = asn1.ObjectIdentifier{1, 3, 132, 0, 33}
	oidNamedCurveP256      = asn1.ObjectIdentifier{1, 2, 840, 10045, 3, 1, 7}
	oidNamedCurveP384      = asn1.ObjectIdentifier{1, 3, 132, 0, 34}
	oidNamedCurveP521      = asn1.ObjectIdentifier{1, 3, 132, 0, 35}
	oidNamedCurveSecp256k1 = asn1.ObjectIdentifier{1, 3, 132, 0, 10}
	oidNamedCurveSm2p256v1 = asn1.ObjectIdentifier{1, 2, 156, 10197, 1, 301}
)

func InitSm2p256v1() {
	sm2p256v1 = &elliptic.CurveParams{Name: "sm2p256v1"}
	sm2p256v1.BitSize = 256
	sm2p256v1.P, _ = new(big.Int).SetString("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF", 16)
	sm2p256v1.A, _ = new(big.Int).SetString("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC", 16)
	sm2p256v1.B, _ = new(big.Int).SetString("28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93", 16)
	sm2p256v1.N, _ = new(big.Int).SetString("FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123", 16)
	sm2p256v1.Gx, _ = new(big.Int).SetString("32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7", 16)
	sm2p256v1.Gy, _ = new(big.Int).SetString("BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0", 16)
}

func Sm2p256v1() elliptic.Curve {
	initonce.Do(InitSm2p256v1)
	return sm2p256v1
}

// LoadCertficateAndKeyFromFile reads file, divides into key and certificates
func LoadCertficateAndKeyFromFile(path string) (*tls.Certificate, error) {
	raw, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}

	var cert tls.Certificate
	for {
		block, rest := pem.Decode(raw)
		if block == nil {
			break
		}
		if block.Type == "CERTIFICATE" {
			cert.Certificate = append(cert.Certificate, block.Bytes)
		} else {
			cert.PrivateKey, err = parsePKCS8ECPrivateKey(block.Bytes)
			if err != nil {
				return nil, fmt.Errorf("Failure reading private key from \"%s\": %s", path, err)
			}
		}
		raw = rest
	}

	if len(cert.Certificate) == 0 {
		return nil, fmt.Errorf("No certificate found in \"%s\"", path)
	} else if cert.PrivateKey == nil {
		return nil, fmt.Errorf("No private key found in \"%s\"", path)
	}

	return &cert, nil
}

// LoadECPrivateKeyFromPEM reads file, divides into key and certificates
func LoadECPrivateKeyFromPEM(path string) (*ecdsa.PrivateKey, error) {
	raw, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}

	block, _ := pem.Decode(raw)
	if block == nil {
		return nil, fmt.Errorf("Failure reading pem from \"%s\": %s", path, err)
	}
	if block.Type != "PRIVATE KEY" {
		return nil, fmt.Errorf("Failure reading private key from \"%s\": %s", path, err)
	}
	ecPirvateKey, err := parsePKCS8ECPrivateKey(block.Bytes)
	if err != nil {
		return nil, fmt.Errorf("Failure reading private key from \"%s\": %s", path, err)
	}
	return ecPirvateKey, nil
}

// parseECPrivateKey is a copy of x509.parseECPrivateKey, supported secp256k1 and sm2p256v1
func parsePKCS8ECPrivateKey(der []byte) (key *ecdsa.PrivateKey, err error) {

	oidPublicKeyECDSA := asn1.ObjectIdentifier{1, 2, 840, 10045, 2, 1}

	var pkcs8 struct {
		Version    int
		Algo       pkix.AlgorithmIdentifier
		PrivateKey []byte
		// optional attributes omitted.
	}
	var privKey struct {
		Version       int
		PrivateKey    []byte
		NamedCurveOID asn1.ObjectIdentifier `asn1:"optional,explicit,tag:0"`
		PublicKey     asn1.BitString        `asn1:"optional,explicit,tag:1"`
	}
	if _, err := asn1.Unmarshal(der, &pkcs8); err != nil {
		return nil, errors.New("x509: failed to parse EC private key embedded in PKCS#8: " + err.Error())
	}
	if !pkcs8.Algo.Algorithm.Equal(oidPublicKeyECDSA) {
		return nil, fmt.Errorf("x509: PKCS#8 wrapping contained private key with unknown algorithm: %v", pkcs8.Algo.Algorithm)
	}
	bytes := pkcs8.Algo.Parameters.FullBytes
	namedCurveOID := new(asn1.ObjectIdentifier)
	if _, err := asn1.Unmarshal(bytes, namedCurveOID); err != nil {
		namedCurveOID = nil
	}
	if _, err := asn1.Unmarshal(pkcs8.PrivateKey, &privKey); err != nil {
		return nil, errors.New("x509: failed to parse EC private key: " + err.Error())
	}
	var curve elliptic.Curve

	switch {
	case namedCurveOID.Equal(oidNamedCurveP224):
		curve = elliptic.P224()
	case namedCurveOID.Equal(oidNamedCurveP256):
		curve = elliptic.P256()
	case namedCurveOID.Equal(oidNamedCurveP384):
		curve = elliptic.P384()
	case namedCurveOID.Equal(oidNamedCurveP521):
		curve = elliptic.P521()
	case namedCurveOID.Equal(oidNamedCurveSecp256k1):
		curve = elliptic.Secp256k1()
	case namedCurveOID.Equal(oidNamedCurveSm2p256v1):
		curve = Sm2p256v1()
	default:
		fmt.Printf("unknown namedCurveOID:%+v", namedCurveOID)
	}

	if curve == nil {
		return nil, errors.New("x509: unknown elliptic curve")
	}
	k := new(big.Int).SetBytes(privKey.PrivateKey)
	curveOrder := curve.Params().N
	if k.Cmp(curveOrder) >= 0 {
		return nil, errors.New("x509: invalid elliptic curve private key value")
	}

	priv := new(ecdsa.PrivateKey)
	priv.Curve = curve
	priv.D = k

	privateKey := make([]byte, (curveOrder.BitLen()+7)/8)

	// Some private keys have leading zero padding. This is invalid
	// according to [SEC1], but this code will ignore it.
	for len(privKey.PrivateKey) > len(privateKey) {
		if privKey.PrivateKey[0] != 0 {
			return nil, errors.New("x509: invalid private key length")
		}
		privKey.PrivateKey = privKey.PrivateKey[1:]
	}

	// Some private keys remove all leading zeros, this is also invalid
	// according to [SEC1] but since OpenSSL used to do this, we ignore
	// this too.
	copy(privateKey[len(privateKey)-len(privKey.PrivateKey):], privKey.PrivateKey)
	priv.X, priv.Y = curve.ScalarBaseMult(privateKey)

	return priv, nil
}

func main() {
	// secp256k1 comes from https://github.com/FISCO-BCOS/console/blob/master/tools/get_account.sh
	// sm2p256v1 from https://github.com/FISCO-BCOS/console/blob/master/tools/get_account.sh
	filename := os.Args[1]
	key, err := LoadECPrivateKeyFromPEM(filename)
	if err != nil {
		fmt.Print("parse failed")
		return
	}
	// key, err := LoadCertficateAndKeyFromFile(filename)
	// key, err := x509.ParsePKCS8PrivateKey(der)
	fmt.Printf("private key is %64x, public is %64x%064x\n", key.D, key.X, key.Y)

}

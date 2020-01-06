package main

import (
	"github.com/bxq2011hust/fisco-tls/crypto/tls"
	"github.com/bxq2011hust/fisco-tls/crypto/x509"
	// "github.com/bxq2011hust/go/src/crypto/x509"
	// "github.com/bxq2011hust/go/src/crypto/tls"

	// "github.com/cloudflare/cf-tls/tls"
	// "crypto/tls"
	"io/ioutil"
	"log"
)

// https://github.com/denji/golang-tls
func main() {
	log.SetFlags(log.Lshortfile)

	roots := x509.NewCertPool()
	rootPEM, err := ioutil.ReadFile("ca.crt")
	if err != nil {
		panic(err)
	}
	ok := roots.AppendCertsFromPEM([]byte(rootPEM))
	if !ok {
		panic("failed to parse root certificate")
	}
	cer, err := tls.LoadX509KeyPair("sdk.crt", "sdk.key")
	if err != nil {
		log.Println(err)
		return
	}
	config := &tls.Config{RootCAs: roots, Certificates: []tls.Certificate{cer}, MinVersion: tls.VersionTLS12, PreferServerCipherSuites: true, 
	 CurvePreferences: []tls.CurveID{
		tls.CurveSecp256k1,
		tls.CurveP256,
		tls.CurveP256,
		tls.X25519,
	},InsecureSkipVerify: true,}
	// CipherSuites: []uint16{
	// 	tls.TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256,
	// 	tls.TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
	// 	tls.TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
	// },
	conn, err := tls.Dial("tcp", "127.0.0.1:20200", config)
	if err != nil {
		log.Println(err)
		return
	}
	defer conn.Close()

	n, err := conn.Write([]byte("hello\n"))
	if err != nil {
		log.Println(n, err)
		return
	}

	buf := make([]byte, 100)
	n, err = conn.Read(buf)
	if err != nil {
		log.Println(n, err)
		return
	}

	println(string(buf[:n]))
}

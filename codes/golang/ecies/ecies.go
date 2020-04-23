//Package ecies wraps the rust ecies
package ecies

/*
#cgo CFLAGS: -I${SRCDIR}/libecies
#cgo LDFLAGS: -L${SRCDIR}/libecies -lffi_ecies

char *ecies_secp256k1_decrypt_c(char *hex_private_key, char *hex_encrypt_data);
char *ecies_secp256k1_encrypt_c(char *hex_public_key, char *hex_message);
*/
import "C"

import (
	"encoding/hex"
	"errors"
)

// Encrypt use public key encrypt rawData
func Encrypt(publicKey, rawData []byte) ([]byte, error) {
	if len(publicKey) != 64 {
		return nil, errors.New("invalid public key, length != 64")
	}
	if len(rawData) == 0 {
		return nil, errors.New("empty rawData")
	}
	pubKeyHex := hex.EncodeToString(publicKey)
	rawDataHex := hex.EncodeToString(rawData)
	cipherDataHexPoint := C.ecies_secp256k1_encrypt_c(C.CString(pubKeyHex), C.CString(rawDataHex))
	cipherDataHex := C.GoString(cipherDataHexPoint)
	if len(cipherDataHex) == 0 {
		return nil, errors.New("Encrypt failed")
	}
	return hex.DecodeString(cipherDataHex)
}

// Decrypt use public key decrypt cipherData
func Decrypt(privateKey, cipherData []byte) ([]byte, error) {
	if len(privateKey) != 32 {
		return nil, errors.New("invalid private key, length != 32")
	}
	if len(cipherData) == 0 {
		return nil, errors.New("empty cipherData")
	}
	privateKeyHex := hex.EncodeToString(privateKey)
	cipherDataHex := hex.EncodeToString(cipherData)
	rawDataHexPoint := C.ecies_secp256k1_decrypt_c(C.CString(privateKeyHex), C.CString(cipherDataHex))
	rawDataHex := C.GoString(rawDataHexPoint)
	if len(rawDataHex) == 0 {
		return nil, errors.New("Decrypt failed")
	}
	return hex.DecodeString(rawDataHex)
}

package ecies

import (
	"encoding/hex"
	"testing"
)

const (
	publicKey  = "36f3570c796c7589a150a4d8a3de37cef15f30e141ca9a7e3162d9c2e3edb4e8db2326fe5489fdbe4ce7931779b727242f7df19c0a773f101417616e7776e789"
	message    = "847adcf9b24cf0041ddff02ffe324e30b1271c5170086f8ee799dd1123dacb2e"
	privateKey = "e82a0751b7671d20d24631faa7033ee6909ed73629e1795e830b8fb8666e17b8"
	encData    = "0408be9fccc96a1bd48f21c8ad7b51f888e8be4213e5dcd9da26b2ca1f5ec066355adc4633a39e574bdfb6b1c7ef6c57e9bb2b2bec8c62eea188455ea662fd62a797843c43d87fec36e54d59e9d7e41f4c89214042095ff5a31b8ae34f7c9a45963be1f7fd2c43c918f84d9936697820e1afed64fe715ff3f2beef9228f660d3b2"
)

func TestEncrypter(t *testing.T) {
	pubKey, err := hex.DecodeString(publicKey)
	if err != nil {
		t.Fatal("DecodeString")
	}
	msgBytes, err := hex.DecodeString(message)
	privKey, err := hex.DecodeString(privateKey)
	cipherData, err := Encrypt(pubKey, msgBytes)
	if err != nil {
		t.Fatal("Encrypt err:", err)
	}
	rawData, err := Decrypt(privKey, cipherData)
	if err != nil {
		t.Fatal("decrypt err:", err)
	}
	rawMsg := hex.EncodeToString(rawData)
	if rawMsg != message {
		t.Logf("decrypt message:%s != %s", rawMsg, message)
		t.Fatal("Decrypt message error")
	}
}

func TestDecrypter(t *testing.T) {
	cipherData, err := hex.DecodeString(encData)
	privKey, err := hex.DecodeString(privateKey)
	rawData, err := Decrypt(privKey, cipherData)
	if err != nil {
		t.Fatal("decrypt err:", err)
	}
	rawMsg := hex.EncodeToString(rawData)
	if rawMsg != message {
		t.Logf("decrypt message:%s != %s", rawMsg, message)
		t.Fatal("Decrypt message error")
	}
}

package main

import (
	"crypto/x509"
	// "crypto/tls"
	"io/ioutil"
	"log"

	"github.com/bxq2011hust/fisco-tls/crypto/tls"
)

// https://github.com/denji/golang-tls
func main() {
	log.SetFlags(log.Lshortfile)

	roots := x509.NewCertPool()
	rootPEM, err := ioutil.ReadFile("certs_ec_node/ca.crt")
	if err != nil {
		panic(err)
	}
	ok := roots.AppendCertsFromPEM([]byte(rootPEM))
	if !ok {
		panic("failed to parse root certificate")
	}

	conf := &tls.Config{
		RootCAs: roots,
		//InsecureSkipVerify: true,
	}
	conn, err := tls.Dial("tcp", "127.0.0.1:20200", conf)
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

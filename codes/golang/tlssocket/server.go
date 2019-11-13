package main

import (
	"bufio"
	"crypto/tls"
	"log"
	"net"
)

// secp256k1 is not supported https://github.com/golang/go/pull/26873
// https://github.com/denji/golang-tls
func main() {
	log.SetFlags(log.Lshortfile)

	cer, err := tls.LoadX509KeyPair("certs_rsa/sdk.crt", "certs_rsa/sdk.key")
	if err != nil {
		log.Println(err)
		return
	}

	config := &tls.Config{Certificates: []tls.Certificate{cer}}
	ln, err := tls.Listen("tcp", ":7878", config)
	if err != nil {
		log.Println(err)
		return
	}
	defer ln.Close()

	for {
		conn, err := ln.Accept()
		if err != nil {
			log.Println(err)
			continue
		}
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()
	r := bufio.NewReader(conn)
	for {
		msg, err := r.ReadString('\n')
		if err != nil {
			log.Println(err)
			return
		}

		println(msg)

		n, err := conn.Write([]byte("world\n"))
		if err != nil {
			log.Println(n, err)
			return
		}
	}
}

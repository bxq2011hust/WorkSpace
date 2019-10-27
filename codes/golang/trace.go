package main

import (
	"log"
	"time"
)

func main() {
	bigSlowOperation()

}

func bigSlowOperation() {
	defer trace("bigSlowOperation")()
	time.Sleep(10 * time.Second) // 通过休眠模仿慢操作
}

func trace(msg string) func() {
	start := time.Now()
	log.Printf("enter %s", msg)
	return func() {
		log.Printf("exit %s (%s)", msg, time.Since(start))

	}
}

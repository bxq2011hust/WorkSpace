package main

import (
	"fmt"
	"log"
	"os"

	"examples/links"
)

func main() {
	links.BreadthFirst(crawl, os.Args[1:])
}

func crawl(url string) []string {
	fmt.Println(url)
	list, err := links.Extract(url)
	if err != nil {
		log.Fatal(err)
	}
	return list
}

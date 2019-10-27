package main

import (
	"fmt"
	"os"
	"net/http"
	"strings"

	"golang.org/x/net/html"
	"examples/links"
)

func main() {
	for _, url := range os.Args[1:] {
		title(url)
	}
}

// title返回网页中的所有title
func title(url string) error{
	resp,err := http.Get(url)
	if err != nil {
		return err
	}
	defer resp.Body.Close()
	ct:=resp.Header.Get("Content-Type")
	if ct!="text/html" && !strings.HasPrefix(ct, "text/html"){
		return fmt.Errorf("%s has type %s, not test/html", url, ct)
	}
	doc,err := html.Parse(resp.Body)
	if err != nil {
		return fmt.Errorf("parsing %s as HTML %v", url, err)
	}
	visitNode := func (n *html.Node){
		if n.Type == html.ElementNode && n.Data == "title" && n.FirstChild != nil{
			fmt.Println(n.FirstChild.Data)
		}
	}
	links.ForEachNode(doc,visitNode,nil)
	return nil
}

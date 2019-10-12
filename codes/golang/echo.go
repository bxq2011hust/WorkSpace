// echo1 输出其命令行参数

package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {

	// var s string
	// for _, arg := range os.Args[1:] {
	// 	s += arg + " "
	// }
	// fmt.Println(s)

	fmt.Println(strings.Join(os.Args[1:], " "))
}

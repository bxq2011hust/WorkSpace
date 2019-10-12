// 将符合搜索条件的issue输出为一个表格
package main

import (
	"fmt"
	"html/template"
	"log"
	"os"
	"time"

	"./github"
)

const templ = `<h1>{{.TotalCount}} issues:</h1>
<table>
<tr style='text-align: left'>
  <th>#</th>
  <th>State</th>
  <th>User</th>
  <th>Title</th>
</tr>
{{range .Items}}
<tr>
  <td><a herf='{{.HTMLURL}}'>{{.Number}}</a></td>
  <td>{{.State}}</td>
  <td><a herf='{{.User.HTMLURL}}'>{{.User.Login}}</a></td>
  <td><a herf='{{.HTMLURL}}'>{{.Title}}</a></td>
</tr>
{{end}}
</table>
`

func daysAgo(t time.Time) int {
	return int(time.Since(t).Hours() / 24)
}

var report = template.Must(template.New("issuelist").Funcs(template.FuncMap{"daysAgo": daysAgo}).Parse(templ))

// ./issues repo:FISCO-BCOS/FISCO-BCOS is:open
func main() {
	result, err := github.SearchIssues(os.Args[1:])
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("%d issues:\n", result.TotalCount)
	// for _, item := range result.Items {
	// 	fmt.Printf("#%-5d %9.9s %.55s\n", item.Number, item.User.Login, item.Title)
	// }

	if err := report.Execute(os.Stdout, result); err != nil {
		log.Fatal(err)
	}
}

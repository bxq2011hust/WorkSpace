// fetch 输出从URL获取的内容

package main

import (
	"io"
	"net/http"
	"os"
	"path"
	"strings"
)

func main() {
	for _, url := range os.Args[1:] {
		fetchAndSave(url)
	}
}

// fetchAndSave 下载url并返回本地文件的名字和长度
func fetchAndSave(url string) (filename string, n int64, err error) {
	if !strings.HasPrefix(url, "http://") {
		url = "http://" + url
	}
	resp, err := http.Get(url)
	if err != nil {
		//fmt.Fprintf(os.Stderr, "fetch: %v\n", err)
		return "", 0, err
	}
	defer resp.Body.Close()
	local := path.Base(resp.Request.URL.Path)
	if local == "/" {
		local = "index.html"
	}
	f, err := os.Create(local)
	if err != nil {
		return "", 0, err
	}
	n, err = io.Copy(f, resp.Body)
	// 关闭文件，并保留错误消息
	if closeErr := f.Close(); err == nil {
		err = closeErr
	}
	return local, n, err
}

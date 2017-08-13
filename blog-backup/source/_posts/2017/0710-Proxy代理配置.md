---
title: Proxy代理配置
categories: 笔记
date: 2017-07-10 11:31:05
updated:
tags: [网络,代理]
description: 为ssh、git等配置proxy
---

例如代理服务器为`http://proxy.example.com:8080`，`windows`给ie配置的代理会作为系统的默认代理，推荐使用5中介绍的`Proxifier`公司内网无脑全部代理，简单粗暴。

## 1. `ssh`设置代理
通过ssh的`ProxyCommand`实现，可以通过`man ssh_config | grep ProxyCommand`查看相关介绍，参考[SSH Through or Over Proxy][SSH Through or Over Proxy]
### 通过`http`使用`ssh`(SSH Over HTTP)
修改`~/.ssh/config`文件，windows下使用`Git for Windows`的路径是`c:/user/%username%/.ssh/config`，其中`Host *`表示通配所有的访问，可以改为针对部分域名

+ **Windows**下
```bash
Host *
ServerAliveInterval 60
ServerAliveCountMax 5
TCPKeepAlive yes
ProxyCommand "/C/Program Files/Git/mingw64/bin/connect.exe" -H proxy.example.com:8080 %h %p
```

+ **Linux**下  
```bash
Host *
ServerAliveInterval 60
ServerAliveCountMax 5
TCPKeepAlive yes
ProxyCommand /usr/local/bin/corkscrew -H proxy.example.com:8080 %h %p
```
### 通过`ssh`代理(SSH Over SSH)
同样修改`~/.ssh/config`文件，windows下使用`Git for Windows`的路径是`c:/user/%username%/.ssh/config`
+ **Windows**下
```bash
Host *
ServerAliveInterval 60
ServerAliveCountMax 5
TCPKeepAlive yes
ProxyCommand "/C/Program Files/Git/mingw64/bin/connect.exe" -S proxy.example.com:8080 %h %p
```

+ **Linux**下  
```bash
Host *
ServerAliveInterval 60
ServerAliveCountMax 5
TCPKeepAlive yes
ProxyCommand nc -x proxy.example.com:8080 %h %p
```
## 2. `git`设置代理
```bash
# 对所有访问使用http/https代理
git config --global https.proxy http://127.0.0.1:1080
git config --global https.proxy https://127.0.0.1:1080
# 对所有访问使用socks5代理
git config --global http.proxy socks5://127.0.0.1:1080
git config --global https.proxy socks5://127.0.0.1:1080
# 取消
git config --global --unset http.proxy
git config --global --unset https.proxy

# 针对特定网站
git config --global http.<要设置代理的URL>.proxy socks5://127.0.0.1:1080
git config --global http.https://github.com.proxy socks5://127.0.0.1:1080
# 取消
git config --global --unset http.https://github.com.proxy
```

## 3. `yum`设置代理
```bash
$ vim /etc/yum.conf
# 在配置文件最后添加
proxy=http://proxy.example.com:8080
```
## 4. `wget`设置代理
```bash
# http
$ wget http://www.baidu.com/ -e use_proxy=yes -e http_proxy=yourproxy.com:port
# https
$ wget --no-check-certificate https://www.google.com/ -e use_proxy=yes -e https_proxy=yourproxy.com:port
# 临时设置环境变量
$ export http_porxy=proxy.example.com:8080
$ export https_proxy=proxy.example.com:8080
```

## 5. windows下使用[proxifier][proxifier official]为所有应用代理
1. `Profile->Proxy Servers`添加代理服务器，配置address和port
2. `Profile->Proxification Rules`设置`Default`的A`Action`为步骤1的代理

***
[SSH Through or Over Proxy]:https://daniel.haxx.se/docs/sshproxy.html

## 参考文献
1. https://daniel.haxx.se/docs/sshproxy.html
2. https://kyonli.com/p/142
3. https://gist.github.com/laispace/666dd7b27e9116faece6
4. https://www.chenyudong.com/archives/wget-http-proxy-setting.html
5. [proxifier official][proxifier official]

[proxifier official]:https://www.proxifier.com/
***
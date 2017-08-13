---
title: Docker安装教程
categories: 工具
date: 2017-04-01 09:45:19
updated: 2017-04-11 20:40
tags: [Docker]
description: Docker安装
---

***
<!-- TOC -->

- [1. Docker简介](#1-docker简介)
- [2. 安装Docker](#2-安装docker)
- [3. 基本概念](#3-基本概念)
- [参考资料](#参考资料)

<!-- /TOC -->

***
## 1. Docker简介

- 参考资料
    + [Docker核心技术预览][5]  
    + [DOcker Offical Docs][7]  
    + [Docker从入门到实践][0]   

- 什么是docker

Docker 使用 Google 公司推出的 Go 语言 进行开发实现，基于 Linux 内核的 cgroup，namespace，以及 AUFS 类的 Union FS 等技术，对进程进行封装隔离，属于操作系统层面的虚拟化技术。由于隔离的进程独立于宿主和其它的隔离的进程，因此也称其为容器。最初实现是基于 LXC，从 0.7 以后开始去除 LXC，转而使用自行开发的 libcontainer，从 1.11 开始，则进一步演进为使用 runC 和 containerd。

Docker 在容器的基础上，进行了进一步的封装，从文件系统、网络互联到进程隔离等等，极大的简化了容器的创建和维护。使得 Docker 技术比虚拟机技术更为轻便、快捷。  

![Docker][p2]

下面的图片比较了 Docker 和传统虚拟化方式的不同之处。传统虚拟机技术是虚拟出一套硬件后，在其上运行一个完整操作系统，在该系统上再运行所需应用进程；而容器内的应用进程直接运行于宿主的内核，容器内没有自己的内核，而且也没有进行硬件虚拟。因此容器要比传统虚拟机更为轻便。

![Container vs. VMs][p1]

- docker解决的问题
 
    + 简化应用实例部署  
    + 降低成本  
    + 软件配置和管理  



## 2. 安装Docker

[官方安装教程][1]  
[中文版安装教程][2]  
以Ubuntu14.04为例，docker的两种安装方式分别是设置docker库和deb包安装，这里使用第一种。docker有社区版docker-ce和企业版docker-ee，这里使用社区版  
- 卸载旧版本`docker`或`docker-engine`

```bash
$ sudo apt-get remove docker docker-engine
```
- 推荐Ubuntu 14.04额外安装的包

```bash
#安装以允许Docker使用aufs存储驱动
$ sudo apt-get update

$ sudo apt-get install linux-image-extra-$(uname -r) linux-image-extra-virtual
```

- 配置docker-ce仓库  

```bash
# 安装包以允许apt通过https使用仓库
$  sudo apt-get install apt-transport-https ca-certificates curl software-properties-common

# 添加Docker官方GPG key
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

# 指纹校验 9DC8 5822 9FC7 DD38 854A E2D8 8D81 803C 0EBF CD88
$ sudo apt-key fingerprint 0EBFCD88

pub   4096R/0EBFCD88 2017-02-22
      Key fingerprint = 9DC8 5822 9FC7 DD38 854A  E2D8 8D81 803C 0EBF CD88
uid                  Docker Release (CE deb) <docker@docker.com>
sub   4096R/F273FCD8 2017-02-22

# 配置稳定版仓库
$ sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"

```

- 安装docker-ce

```bash
$ sudo apt-get install docker-ce
```

- 配置镜像加速  

国内访问 Docker Hub拉取镜像速度较慢，此时可以配置镜像加速器。这里选择网易蜂巢提供的[镜像加速][3] 

```bash
# vim /etc/default/docker 最后添加
$ sudo vim /etc/default/docker
DOCKER_OPTS="$DOCKER_OPTS --registry-mirror=http://hub-mirror.c.163.com"
$ service docker restart
```

- 检验是否正确安装  

```bash
$ sudo docker run --rm hello-world

#输出如下
                                                                              
Hello from Docker!                                                            
This message shows that your installation appears to be working correctly.    
                                                                              
To generate this message, Docker took the following steps:                    
 1. The Docker client contacted the Docker daemon.                            
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.     
 3. The Docker daemon created a new container from that image which runs the  
    executable that produces the output you are currently reading.            
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.                                                         
                                                                              
To try something more ambitious, you can run an Ubuntu container with:        
 $ docker run -it ubuntu bash                                                 
                                                                              
Share images, automate workflows, and more with a free Docker ID:             
 https://cloud.docker.com/                                                    
                                                                              
For more examples and ideas, visit:                                           
 https://docs.docker.com/engine/userguide/                                    
```

- 将用户`user`加`docker`组，这样使用`docker`指令时就不用`sudo`执行

```bash
$ sudo usermod -a -G docker user
```


## 3. 基本概念

Docker包括以下几个基本概念（以下内容摘自[Docker — 从入门到实践][0]）

- 镜像(Image) 
>Docker 镜像是一个特殊的文件系统，除了提供容器运行时所需的程序、库、资源、配置等文件外，还包含了一些为运行时准备的一些配置参数（如匿名卷、环境变量、用户等）。镜像不包含任何动态数据，其内容在构建之后也不会被改变。

- 容器(Container) 镜像的一个实例，类似于一个虚拟机

- 数据卷(Volume) 数据的持久化存储

- Registry与仓库(Repository) 镜像的仓库

- [Docker Compose][8] 定义和运行多容器Docker应用程序的工具。
- [NVIDIA-Docker][9] 使docker能够调用gpu，docker的镜像中需要cuda工具包，宿主机需要安装gpu驱动

***
[0]:https://yeasy.gitbooks.io/docker_practice/content/introduction/what.html
[1]:https://docs.docker.com/engine/installation/
[2]:https://yeasy.gitbooks.io/docker_practice/content/install/
[3]:https://c.163.com/wiki/index.php?title=DockerHub%E9%95%9C%E5%83%8F%E5%8A%A0%E9%80%9F
[5]:http://www.infoq.com/cn/articles/docker-core-technology-preview
[6]:http://coolshell.cn/articles/17061.html
[7]:https://docs.docker.com/
[8]:https://github.com/docker/compose "docker compose"
[9]:https://github.com/NVIDIA/nvidia-docker "Nvidia Docker"

[p1]:https://www.docker.com/sites/default/files/containers-vms-together.png "Containers vs. VMs"
[p2]:https://www.docker.com/sites/default/files/what_is_a_container.png "What is Docker"

## 参考资料

1. https://yeasy.gitbooks.io/docker_practice/content/introduction/what.html  
2. https://docs.docker.com/engine/installation/  
3. https://yeasy.gitbooks.io/docker_practice/content/install/  
4. https://c.163.com/wiki/index.php?title=DockerHub%E9%95%9C%E5%83%8F%E5%8A%A0%E9%80%9F  
5. http://www.infoq.com/cn/articles/docker-core-technology-preview  
6. http://coolshell.cn/articles/17061.html  
7. https://docs.docker.com/  
***
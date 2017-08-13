---
title: Docker常用命令
categories: 笔记
date: 2017-07-30 14:30:42
updated: 2017-08-11 20:57:22
tags: [Docker]
description: 记录常用的Docker命令
---
<!-- TOC -->

- [1. Docker命令列表](#1-docker命令列表)
- [2. 常用Docker命令](#2-常用docker命令)
    - [`docker images` 列出已有镜像](#docker-images-列出已有镜像)
    - [`docker search` 搜素`Docker Hub`上的镜像](#docker-search-搜素docker-hub上的镜像)
    - [`docker pull` 从`Docker Hub`上拉取镜像](#docker-pull-从docker-hub上拉取镜像)
    - [`docker run` 运行容器](#docker-run-运行容器)
    - [`docker ps` 列出容器](#docker-ps-列出容器)
    - [`docker stats` 显示容器资源使用统计](#docker-stats-显示容器资源使用统计)
    - [`docker start` 启动停止状态的容器](#docker-start-启动停止状态的容器)
    - [`docker stop` 停止运行状态的容器](#docker-stop-停止运行状态的容器)
    - [`docker restart` 重新启动容器](#docker-restart-重新启动容器)
    - [`docker attach` 链接容器的标准输入、输出等到宿主机](#docker-attach-链接容器的标准输入输出等到宿主机)
    - [`docker exec` 在容器中执行命令](#docker-exec-在容器中执行命令)
    - [`docker rm` 删除容器](#docker-rm-删除容器)
    - [`docker rmi` 删除镜像](#docker-rmi-删除镜像)
    - [`docker build` 根据`Dockerfile`构建容器](#docker-build-根据dockerfile构建容器)
    - [`docker save` 导出镜像](#docker-save-导出镜像)
    - [`docker load` 导入镜像](#docker-load-导入镜像)
    - [`docker export` 导出容器](#docker-export-导出容器)
    - [`docker import` 导入容器](#docker-import-导入容器)
- [3. Docker管理命令](#3-docker管理命令)
    - [3.1 `docker network`](#31-docker-network)
        - [`docker network connect` 链接容器到网络](#docker-network-connect-链接容器到网络)
        - [`docker network create` 创建网络](#docker-network-create-创建网络)
        - [`docker network disconnect` 将容器从某个网络退出](#docker-network-disconnect-将容器从某个网络退出)
        - [`docker network inspect` 显示一个或多个网络详情](#docker-network-inspect-显示一个或多个网络详情)
        - [`docker network ls` 列出所有网络](#docker-network-ls-列出所有网络)
        - [`docker network prune` 清理不使用的网络](#docker-network-prune-清理不使用的网络)
        - [`docker network rm` 清理一个或多个网络](#docker-network-rm-清理一个或多个网络)
    - [3.2 `docker system` 管理`Docker`](#32-docker-system-管理docker)
        - [`docker system df` 显示`docker`磁盘占用](#docker-system-df-显示docker磁盘占用)
        - [`docker system events` 从`docker`服务器获取实时事件](#docker-system-events-从docker服务器获取实时事件)
        - [`docker system info` 显示全系统信息](#docker-system-info-显示全系统信息)
        - [`docker system prune` 删除不用的数据](#docker-system-prune-删除不用的数据)
- [参考资料](#参考资料)

<!-- /TOC -->
## 1. Docker命令列表
[官方文档地址][官方文档]
```bash
$ docker --help

Usage:  docker COMMAND

A self-sufficient runtime for containers

Options:
      --config string      Location of client config files (default "/home/ubuntu/.docker")
  -D, --debug              Enable debug mode
      --help               Print usage
  -H, --host list          Daemon socket(s) to connect to (default [])
  -l, --log-level string   Set the logging level ("debug", "info", "warn", "error", "fatal") (default "info")
      --tls                Use TLS; implied by --tlsverify
      --tlscacert string   Trust certs signed only by this CA (default "/home/ubuntu/.docker/ca.pem")
      --tlscert string     Path to TLS certificate file (default "/home/ubuntu/.docker/cert.pem")
      --tlskey string      Path to TLS key file (default "/home/ubuntu/.docker/key.pem")
      --tlsverify          Use TLS and verify the remote
  -v, --version            Print version information and quit

Management Commands:
  container   Manage containers
  image       Manage images
  network     Manage networks
  node        Manage Swarm nodes
  plugin      Manage plugins
  secret      Manage Docker secrets
  service     Manage services
  stack       Manage Docker stacks
  swarm       Manage Swarm
  system      Manage Docker
  volume      Manage volumes

Commands:
  attach      Attach to a running container
  build       Build an image from a Dockerfile
  commit      Create a new image from a container's changes
  cp          Copy files/folders between a container and the local filesystem
  create      Create a new container
  diff        Inspect changes to files or directories on a container's filesystem
  events      Get real time events from the server
  exec        Run a command in a running container
  export      Export a container's filesystem as a tar archive
  history     Show the history of an image
  images      List images
  import      Import the contents from a tarball to create a filesystem image
  info        Display system-wide information
  inspect     Return low-level information on Docker objects
  kill        Kill one or more running containers
  load        Load an image from a tar archive or STDIN
  login       Log in to a Docker registry
  logout      Log out from a Docker registry
  logs        Fetch the logs of a container
  pause       Pause all processes within one or more containers
  port        List port mappings or a specific mapping for the container
  ps          List containers
  pull        Pull an image or a repository from a registry
  push        Push an image or a repository to a registry
  rename      Rename a container
  restart     Restart one or more containers
  rm          Remove one or more containers
  rmi         Remove one or more images
  run         Run a command in a new container
  save        Save one or more images to a tar archive (streamed to STDOUT by default)
  search      Search the Docker Hub for images
  start       Start one or more stopped containers
  stats       Display a live stream of container(s) resource usage statistics
  stop        Stop one or more running containers
  tag         Create a tag TARGET_IMAGE that refers to SOURCE_IMAGE
  top         Display the running processes of a container
  unpause     Unpause all processes within one or more containers
  update      Update configuration of one or more containers
  version     Show the Docker version information
  wait        Block until one or more containers stop, then print their exit codes
```

## 2. 常用Docker命令
### `docker images` 列出已有镜像
```bash
$ docker images
REPOSITORY              TAG                 IMAGE ID            CREATED             SIZE
alpine                  3.6                 7328f6f8b418        6 weeks ago         3.97 MB
ubuntu                  16.04               d355ed3537e9        7 weeks ago         119 MB
centos                  7                   3bee3060bfc8        2 months ago        193 MB
mysql                   latest              9546ca122d3a        4 months ago        407 MB
rabbitmq                latest              d452bc9055a1        4 months ago        178 MB
postgres                latest              4e18b2c30f8d        5 months ago        266 MB

$ docker images -f "dangling=true"
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
<none>              <none>              94a95453f9cb        About an hour ago   19 MB
ubuntu              <none>              0ef2e08ed3fa        5 months ago        130 MB

$ docker images --filter=reference='busy*:*libc'
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
busybox             uclibc              e02e811dd08f        5 weeks ago         1.09 MB
busybox             glibc               21c16b6787c6        5 weeks ago         4.19 MB
```
### `docker search` 搜素`Docker Hub`上的镜像
```bash
$ docker search ubuntu
NAME                       DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
ubuntu                     Ubuntu is a Debian-based Linux operating s...   6394      [OK]
rastasheep/ubuntu-sshd     Dockerized SSH service, built on top of of...   97                   [OK]
ubuntu-upstart             Upstart is an event-based replacement for ...   76        [OK]
neurodebian                NeuroDebian provides neuroscience research...   38        [OK]
32bit/ubuntu               Ubuntu for i386 (32bit)                         31
ubuntu-debootstrap         debootstrap --variant=minbase --components...   30        [OK]
```
### `docker pull` 从`Docker Hub`上拉取镜像
```bash
$ docker pull hello-world
Using default tag: latest
latest: Pulling from library/hello-world
b04784fba78d: Pull complete
Digest: sha256:9a4ec8dac439d00fff31bf41b23902bfd7f7465d4b4c8c950e572e7392f33c66
Status: Downloaded newer image for hello-world:latest
```
### `docker run` 运行容器

该指令可以限制容器的`CPU/IO/Memory`使用、端口映射、数据卷挂载等

```bash
# 绑定容器的80端口到宿主机127.0.0.1的8080端口
# 运行基于ubuntu:16.04镜像的容器，命名为test，挂载本机/home/ubuntu/data到容器/data
$ docker run --name test -it -v /home/ubuntu/data:/data -p 127.0.0.1:8080:80 ubuntu:16.04 bash
```
### `docker ps` 列出容器
```bash
# 列出所有容器，不加-a则列出运行状态的容器
$ docker ps -a
```
### `docker stats` 显示容器资源使用统计
```bash
$ docker stats
CONTAINER           CPU %       MEM USAGE / LIMIT     MEM %         NET I/O             BLOCK I/O
1285939c1fd3        0.07%       796 KiB / 64 MiB      1.21%         788 B / 648 B       3.568 MB / 512 KB
9c76f7834ae2        0.07%       2.746 MiB / 64 MiB    4.29%         1.266 KB / 648 B    12.4 MB / 0 B
d1ea048f04e4        0.03%       4.583 MiB / 64 MiB    6.30%         2.854 KB / 648 B    27.7 MB / 0 B
```
### `docker start` 启动停止状态的容器
```bash
$ docker start container_name1 container_name2 container_ID1
```
### `docker stop` 停止运行状态的容器
### `docker restart` 重新启动容器
### `docker attach` 链接容器的标准输入、输出等到宿主机
**注意**这个命令使用`ctrl+c`退出时，会使容器中的命令停止
```bash
$ docker run -d --name topdemo ubuntu /usr/bin/top -b

$ docker attach topdemo

top - 02:05:52 up  3:05,  0 users,  load average: 0.01, 0.02, 0.05
Tasks:   1 total,   1 running,   0 sleeping,   0 stopped,   0 zombie
Cpu(s):  0.1%us,  0.2%sy,  0.0%ni, 99.7%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:    373572k total,   355560k used,    18012k free,    27872k buffers
Swap:   786428k total,        0k used,   786428k free,   221740k cached
```
### `docker exec` 在容器中执行命令
**注意**该命令退出时不会导致容器停止运行，适用于容器运行时进入容器调试
```bash
$ docker run -d --rm --name ubuntu_bash ubuntu:16.04 top -b
bbda72fc94bd09592ba668296759e63bfd5a8cc86fbd9a485fab611ae9098804
$ docker exec -it ubuntu_bash bash
root@bbda72fc94bd:/# ls
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
root@bbda72fc94bd:/# exit
exit
```
### `docker rm` 删除容器
```bash
$ docker ps -a
CONTAINER ID        IMAGE           COMMAND     CREATED           STATUS              PORTS               NAMES
9669cb43685e        ubuntu:16.04    "top -b"    15 seconds ago    Exited (0) 4 seconds ago  distracted_minsky
$ docker rm 9669cb43685e
9669cb43685e
```
### `docker rmi` 删除镜像
```bash
$ docker images
REPOSITORY     TAG             IMAGE ID            CREATED             SIZE
test1          ubuntu          20f08c4102b0        24 hours ago        1.12 GB
test2          centos          f6f693e54861        24 hours ago        949 MB
<none>         <none>          2643d4e2a4b2        25 hours ago        1.12 GB
$ docker rmi 2643d4e2a4b2
Deleted: sha256:2643d4e2a4b2a2d0c105c0ab17017ca341e451b69a708e0efe7aa64b5b9f3dfc
Deleted: sha256:e1af11f977d7d904683040b14ee8a0ad07249b1f6ba93f22f1be395e8d4cdb71
Deleted: sha256:ff21afdcbb4a125db32389d7baa4cfebc32631c7dc0f1c6a3add5da857845726
Deleted: sha256:52103014ee5b237d2de1699b157b25c28fecf317045b0ba1a5c892e55d694dfa
Deleted: sha256:7e35b904e8751a5e7c8ef6a6bd7443614cd32026c9d2e2f64235fbb2ee82e3c1
Deleted: sha256:a01dc14799ffbd29600f14873502b6ec5cebb5a25cbe310b9e07f854da057158
Deleted: sha256:a540d4d1e21ddab0b2def028f9dd2467ada4a6f1ae0c35842445993285f02018
Deleted: sha256:e77fe66c0e544b6466d3892b931bef1d04a4615b07cb1951c5d598b764b204fd
```

### `docker build` 根据`Dockerfile`构建容器
```bash
# -f指定Dockerfile，否则默认在制定文件夹下寻找Dockerfile
# 最后指定构建的上下文环境为~/context
$ docker build -f Dockerfile-ubuntu -t my/test1:latest ~/context
```
### `docker save` 导出镜像
[`docker save`官方文档][docker save] 

```bash
# 导出alpine到alpine.tar文件，也可以使用镜像ID代替标签，但会导致标签丢失
$ docker save alpine > alpine.tar
# 使用gzip压缩导出镜像
$ gzip alpine.tar
```
### `docker load` 导入镜像
```bash
$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu              16.04               d355ed3537e9        9 days ago          119 MB
centos              7                   3bee3060bfc8        3 weeks ago         193 MB
$ ls 
alpine.tar.gz
# 解压缩
$ gzip -d alpine.tar.gz

$ docker load --input alpine.tar
5bef08742407: Loading layer [==================================================>]  4.221MB/4.221MB
160208089451: Loading layer [==================================================>]  13.96MB/13.96MB
90ffeeda5ab1: Loading layer [==================================================>]  3.708MB/3.708MB
8116d00e733d: Loading layer [==================================================>]  305.3MB/305.3MB
3fe193cda492: Loading layer [==================================================>]  3.072kB/3.072kB
c503fc697362: Loading layer [==================================================>]  8.192kB/8.192kB
Loaded image: alpine
```
### `docker export` 导出容器
### `docker import` 导入容器
[`docker import`官方说明][docker import] 

## 3. Docker管理命令
### 3.1 `docker network`
参考[官方文档](https://docs.docker.com/engine/reference/commandline/network/#usage)，[固定IP][容器固定ip]，[使用网络][Docker-net]
#### `docker network connect` 链接容器到网络
#### `docker network create` 创建网络
#### `docker network disconnect` 将容器从某个网络退出
#### `docker network inspect` 显示一个或多个网络详情
```bash
$ docker network inspect bridge
[
    {
        "Name": "bridge",
        "Id": "faf3dcbd6fdc42b644eaef287f0615d06bd6031d48c88c8d726b471ceaf7d4aa",
        "Created": "2017-08-11T11:00:18.740657655+08:00",
        "Scope": "local",
        "Driver": "bridge",
        "EnableIPv6": false,
        "IPAM": {
            "Driver": "default",
            "Options": null,
            "Config": [
                {
                    "Subnet": "172.17.0.0/16",
                    "Gateway": "172.17.0.1"
                }
            ]
        },
        "Internal": false,
        "Attachable": false,
        "Containers": {},
        "Options": {
            "com.docker.network.bridge.default_bridge": "true",
            "com.docker.network.bridge.enable_icc": "true",
            "com.docker.network.bridge.enable_ip_masquerade": "true",
            "com.docker.network.bridge.host_binding_ipv4": "0.0.0.0",
            "com.docker.network.bridge.name": "docker0",
            "com.docker.network.driver.mtu": "1500"
        },
        "Labels": {}
    }
]
```
#### `docker network ls` 列出所有网络
```bash
$ docker network ls
NETWORK ID          NAME                DRIVER              SCOPE
faf3dcbd6fdc        bridge              bridge              local
e37a8c94c5eb        host                host                local
1cf66a72742f        none                null                local
```
#### `docker network prune` 清理不使用的网络
#### `docker network rm` 清理一个或多个网络

### 3.2 `docker system` 管理`Docker`
#### `docker system df` 显示`docker`磁盘占用
```bash
$ docker system df
TYPE                TOTAL               ACTIVE              SIZE                RECLAIMABLE
Images              13                  0                   5.002 GB            5.002 GB (100%)
Containers          0                   0                   0 B                 0 B
Local Volumes       0                   0                   0 B                 0 B
```
#### `docker system events` 从`docker`服务器获取实时事件
#### `docker system info` 显示全系统信息
```bash
$ docker system info
Containers: 0
 Running: 0
 Paused: 0
 Stopped: 0
Images: 36
Server Version: 17.03.0-ce
Storage Driver: aufs
 Root Dir: /var/lib/docker/aufs
 Backing Filesystem: extfs
 Dirs: 78
 Dirperm1 Supported: true
Logging Driver: json-file
Cgroup Driver: cgroupfs
Plugins:
 Volume: local
 Network: bridge host macvlan null overlay
Swarm: inactive
Runtimes: runc
Default Runtime: runc
Init Binary: docker-init
containerd version: 977c511eda0925a723debdc94d09459af49d082a
runc version: a01dafd48bc1c7cc12bdb01206f9fea7dd6feb70
init version: 949e6fa
Security Options:
 apparmor
 seccomp
  Profile: default
Kernel Version: 4.4.0-71-generic
Operating System: Ubuntu 16.04.2 LTS
OSType: linux
Architecture: x86_64
CPUs: 2
Total Memory: 3.843 GiB
Name: hw-ubuntu
ID: FQHS:YJQT:JNAD:XIHA:2O2U:5JKJ:DELJ:6HA6:3U5W:IIIZ:WQIF:OZSS
Docker Root Dir: /var/lib/docker
Debug Mode (client): false
Debug Mode (server): false
Registry: https://index.docker.io/v1/
WARNING: No swap limit support
Experimental: false
Insecure Registries:
 127.0.0.0/8
Registry Mirrors:
 https://mirror.ccs.tencentyun.com
 http://hub-mirror.c.163.com
 https://registry.docker-cn.com
Live Restore Enabled: false
```
#### `docker system prune` 删除不用的数据
```bash
# 清理不用的容器、虚悬镜像、数据卷等
$ docker system prune
WARNING! This will remove:
        - all stopped containers
        - all volumes not used by at least one container
        - all networks not used by at least one container
        - all dangling images
Are you sure you want to continue? [y/N] y
Deleted Containers:
d254e2ff8e0b424c5481e40dab88c158f91b48269bfc3e1e17867e40f652081e
1f76bff235a15e7003028091aeb1511c2a49eb2a36219e6f65218c4d11ac7445
0cc36a5e483fa040113fd6d20a096612a6b34a4f281fc90bb412046f77db3861

Total reclaimed space: 2.867 GB
```

***
[docker save]:https://docs.docker.com/engine/reference/commandline/save/
[docker import]:https://docs.docker.com/engine/reference/commandline/import/
[容器固定ip]:https://yaxin-cn.github.io/Docker/docker-container-use-static-IP.html "容器固定ip"
[docker network]:https://yeasy.gitbooks.io/docker_practice/content/network/
[官方文档]:https://docs.docker.com/engine/reference/commandline/docker/#child-commands "官方文档"
## 参考资料
1. 官方文档 https://docs.docker.com/engine/reference/commandline/docker/#child-commands


---
title: CentOS7安装docker与devicemapper配置
categories: 笔记
date: 2017-07-15 21:06:45
updated:
tags: [Docker]
description: CentOS7安装docker（在线与离线）以及devicemapper配置
---

## 1. 安装Docker CE 17.06

以下内容全部来自于[官方文档][docker-docs]，主要介绍`CentOS`安装过程。
### 1. 在线安装
- 卸载旧版本
```bash
sudo yum remove docker \
                  docker-common \
                  docker-selinux \
                  docker-engine
```

- 安装依赖
```bash
$ sudo yum install -y yum-utils device-mapper-persistent-data lvm2
```

- 配置Docker仓库
```bash
$ sudo yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo
```

- 安装Docker CE
```bash
$ sudo yum makecache fast
# 这里默认安装最新版
$ sudo yum install -y docker-ce
# 生产系统应指定版本号安装 
$ sudo yum install -y docker-ce-17.06.0.ce-1.el7.centos.x86_64
```

### 2. 离线安装

- 使用`downloadonly`插件下载安装包以及依赖
```bash
# 安装插件
$ yum install -y yum-plugin-downloadonly

# 下载软件包
$ sudo yum install --downloadonly --downloaddir=/home/$USER/Downloads/docker yum-utils device-mapper-persistent-data lvm2

# 添加docker仓库
$ sudo yum-config-manager \
    --add-repo \
    https://download.docker.com/linux/centos/docker-ce.repo

# 更新yum索引
$ sudo yum makecache fast

# 下载Docker CE 17.06
$ sudo yum install --downloadonly --downloaddir=/home/$USER/Downloads/docker docker-ce-17.06.0.ce-1.el7.centos.x86_64

# 拷贝下载文件到离线的机器，进入软件包存放目录，执行安装
$ sudo rpm -ivh --force --nodeps *
```
- 启动Docker服务
```bash
# 启动
$ sudo systemctl start docker
# 关闭
$ sudo systemctl stop docker
# 重启
$ sudo systemctl restart docker
# 开启自动
$ sudo systemctl enable docker
# 取消开启自动
$ sudo systemctl disable docker
```

***************************************************
### 2. 相关配置
- 配置镜像加速（可选）
修改配置文件`/etc/docker/daemon.json`，如果不存在则创建，添加下面的内容
```bash
{
  "registry-mirrors": ["https://mirror.ccs.tencentyun.com","http://hub-mirror.c.163.com","https://registry.docker-cn.com"]
}
```

- 测试及用户加docker组（可选）

```bash
# 如果不存在docker用户组，则创建()
$ sudo groupadd docker
# 用户加组
$ sudo usermod -aG docker $USER

# 测试安装结果
$ docker run hello-world
# 输出下面的信息即为安装成功
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
b04784fba78d: Pull complete
Digest: sha256:9a4ec8dac439d00fff31bf41b23902bfd7f7465d4b4c8c950e572e7392f33c66
Status: Downloaded newer image for hello-world:latest

Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.
```

- 配置存储驱动（可选）

参考[Select a storage driver][storage-driver]选择存储驱动，由于写时复制(COW)存储驱动会影响容器内的写操作性能，如果是在数据卷中读写则没有影响。下面的配置将CentOS存储驱动修改为`devicemapper lvm-direct`，参考[这里][direct-lvm]。docker版本17.06之后能够管理块设备，只需要简单修改配置文件`/etc/docker/daemon.json`如下，然后重启docker即可，其中`dm.directlvm_device`是可用的块设备。
```bash
{
  "storage-driver": "devicemapper",
  "storage-opts": [
    "dm.directlvm_device=/dev/vdb",
    "dm.thinp_percent=95",
    "dm.thinp_metapercent=1",
    "dm.thinp_autoextend_threshold=80",
    "dm.thinp_autoextend_percent=20",
    "dm.directlvm_device_force=false"
  ]
}
```

- iptables警告 
```bash
$ docker info
# 如果输出信息含有下面警告
WARNING: bridge-nf-call-iptables is disabled
WARNING: bridge-nf-call-ip6tables is disabled
# 添加内核配置参数以启用这些功能
$ sudo tee -a /etc/sysctl.conf <<-EOF
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
# 重新加载 sysctl.conf
$ sudo sysctl -p
```

- docker无法启动
1. `direct-lvm`配置错误，检测`/etc/docker/daemon.json`格式,示例如下
```json
{
    "registry-mirrors": [
        "https://mirror.ccs.tencentyun.com",
        "http://hub-mirror.c.163.com",
        "https://registry.docker-cn.com"
    ],
    "storage-driver": "devicemapper",
    "storage-opts": [

            "dm.directlvm_device=/dev/vdb",
            "dm.thinp_percent=95",
            "dm.thinp_metapercent=1",
            "dm.thinp_autoextend_threshold=80",
            "dm.thinp_autoextend_percent=20",
            "dm.directlvm_device_force= false"
    ]
}
```
2. `directlvm_device`已经存在文件系统，修改配置文件中`"dm.directlvm_device_force=ture"`，则会格式化`directlvm_device`设备
3. 按照1、2处理后依旧无法启动，检查是否已有卷组
```bash
# 查看LVM卷
$ sudo lvdisplay
  --- Logical volume ---
  LV Name                thinpool
  VG Name                docker
  LV UUID                xcx2xB-iwdJ-sFJ7-9low-X6ib-nEvs-cUQuZ4
  LV Write Access        read/write
  LV Creation host, time VM_105_23_centos, 2017-07-06 21:16:37 +0800
  LV Pool metadata       thinpool_tmeta
  LV Pool data           thinpool_tdata
  LV Status              available
  # open                 0
  LV Size                19.00 GiB
  Allocated pool data    0.10%
  Allocated metadata     0.03%
  Current LE             4863
  Segments               1
  Allocation             inherit
  Read ahead sectors     auto
  - currently set to     8192
  Block device           252:2

# 删除LVM卷 
$ sudo lvremove thinpool

# 删除卷组
$ sudo vgremove docker
Do you really want to remove volume group "docker" containing 1 logical volumes? [y/n]: y
Do you really want to remove active logical volume docker/thinpool? [y/n]: y
  Logical volume "thinpool" successfully removed
  Volume group "docker" successfully removed 
```



***
[docker-docs]:https://docs.docker.com/
[storage-driver]:https://docs.docker.com/engine/userguide/storagedriver/selectadriver/
[direct-lvm]:https://docs.docker.com/engine/userguide/storagedriver/device-mapper-driver/#configure-direct-lvm-mode-for-production

***

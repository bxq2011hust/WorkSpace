---
title: Redis学习-基础命令
categories: 工具
date: 2017-05-14 16:03:27
updated:
tags: [Redis,数据库]
description: Redis入门-基础命令记录
---
<!-- TOC -->

- [1. 安装Redis](#1-安装redis)
    - [ubuntu](#ubuntu)
    - [windows](#windows)
- [2. [Redis命令][2.0]](#2-redis命令20)
    - [连接及配置](#连接及配置)
    - [Redis键(key)](#redis键key)
    - [Redis字符串(string)](#redis字符串string)
    - [Redis哈希(Hash)](#redis哈希hash)
    - [增量迭代命令](#增量迭代命令)
    - [Redis列表(List)](#redis列表list)
    - [Redis集合(Set)](#redis集合set)
    - [Redis有序集合(sorted set)](#redis有序集合sorted-set)
- [3. Redis Client](#3-redis-client)
- [4. Redis持久化](#4-redis持久化)
- [参考资料](#参考资料)

<!-- /TOC -->
***

***
## 1. 安装Redis
参考[这里][1.1]
### ubuntu
```bash
$ sudo apt-get update
$ sudo apt-get install redis-server
$ sudo apt-get install redis-tools
```

### windows
从[这里][1.2]下载`zip`包，然后使用`cmd`进入该文件夹下，例如我放在`D:\Software\Redis-x64-3.2.100`
```bash
$ cd /d d:/software/Redis-x64-3.2.100
$ redis-server.exe redis.windows.conf
                _._
           _.-``__ ''-._
      _.-``    `.  `_.  ''-._           Redis 3.2.100 (00000000/0) 64 bit
  .-`` .-```.  ```\/    _.,_ ''-._
 (    '      ,       .-`  | `,    )     Running in standalone mode
 |`-._`-...-` __...-.``-._|'` _.-'|     Port: 6379
 |    `-._   `._    /     _.-'    |     PID: 13392
  `-._    `-._  `-./  _.-'    _.-'
 |`-._`-._    `-.__.-'    _.-'_.-'|
 |    `-._`-._        _.-'_.-'    |           http://redis.io
  `-._    `-._`-.__.-'_.-'    _.-'
 |`-._`-._    `-.__.-'    _.-'_.-'|
 |    `-._`-._        _.-'_.-'    |
  `-._    `-._`-.__.-'_.-'    _.-'
      `-._    `-.__.-'    _.-'
          `-._        _.-'
              `-.__.-'

[13392] 15 May 11:22:55.620 # Server started, Redis version 3.2.100
[13392] 15 May 11:22:55.620 * The server is now ready to accept connections on port 6379

#另开一个cmd
$ redis-cli
127.0.0.1:6379>
```
## 2. [Redis命令][2.0]

redis命令不区分大小写，`Linux`下Redis的配置文件位于`/etc/redis/redis.conf`，配置文件中详细介绍了Redis的配置项

### 1. 连接及配置
```bash
# 链接
$ redis-cli -h 127.0.0.1 -p 6379 -a "password"
# 认证密码
127.0.0.1:6379> auth password
OK
# 查看服务是否运行
127.0.0.1:6379> ping
PONG
# 清空Redis
127.0.0.1:6379> flushall
OK
# 选择db 默认为0
127.0.0.1:6379> select 1
OK
127.0.0.1:6379[1]>
# 清空Redis某个数据库
127.0.0.1:6379> flushdb
OK
# info获取服务器信息
# 退出链接 quit/exit

# config get config_setting_name
127.0.0.1:6379>config get *
  1) "dbfilename"
  2) "dump.rdb"
  3) "requirepass"
  4) ""
  5) "masterauth"
  6) ""
  7) "unixsocket"
  8) ""
  9) "logfile"
 10) "/var/log/redis/redis-server.log"
 11) "pidfile"
 12) "/var/run/redis/redis-server.pid"
 13) "maxmemory"
 14) "0"
 15) "maxmemory-samples"
 16) "5"
 17) "timeout"
 18) "0"
 ...
```
### 2. Redis键(key)

```bash
# 添加 值为string
127.0.0.1:6379> set key1 redis
OK
# 删除
127.0.0.1:6379> del key1
(integer) 1
# 返回键的类型
127.0.0.1:6379> type key1
string
127.0.0.1:6379> type hash_key
hash
# 获取全部key
127.0.0.1:6379> keys *
(empty list or set)
# 键是否存在
127.0.0.1:6379> exists key1
(integer) 1
# 设置key1在2秒后过期，过期后自动删除
127.0.0.1:6379> expire key1 2
(integer) 1
# 移除过期时间，如果无过期时间则返回0
127.0.0.1:6379> persist key1
(integer) 1
# 查找符合给定模式的key
127.0.0.1:6379> keys k*
1) "key1"
# 移动key(key1)到指定的db(db1)
127.0.0.1:6379> move key1 1
(integer) 1
# 修改键的名称，不存在则返回(error) ERR no such key
127.0.0.1:6379> rename key1 key2
OK
# 仅当newkey不存在时改名
127.0.0.1:6379> renamenx key1 key2
(integer) 0
```

### 3. Redis字符串(string)
```bash
# 设置key的值
127.0.0.1:6379> set key1 key1
OK
# 获取key1的值
127.0.0.1:6379> get key1
"key1"
# 返回字符串中的子串
127.0.0.1:6379> getrange key1 0 1
"ke"
127.0.0.1:6379> getrange key1 0 -1
"key1"
# getset设置key的值，并返回旧值
127.0.0.1:6379> getset key1 redis
"key1"
# setbit/getbit设置或获取key对应的值的bit位
# 获取多个key
127.0.0.1:6379> mget key1 key2 key3
1) "redis"
2) "redis"
3) (nil)
# 只有键不存在时才设置值
127.0.0.1:6379> setnx key1 value
(integer) 0
127.0.0.1:6379> setnx key3 value3
(integer) 1
# 从offset开始覆盖字串
127.0.0.1:6379> setrange key3 6 value3
(integer) 12
127.0.0.1:6379> get key3
"value3value3"
# 获取字符串长度
127.0.0.1:6379> get key1
"redis"
127.0.0.1:6379> strlen key1
(integer) 5
# mset设置多个值/msetnx设置多个值仅当不存在时(有一个存在则失败)
127.0.0.1:6379> mset key3 value3 key4 value4
OK
127.0.0.1:6379> msetnx key4 value4 key5 vlaue5
(integer) 0
# 追加key的值
127.0.0.1:6379> append key2 value2
(integer) 11
127.0.0.1:6379> get key2
"redisvalue2"
# incr key将key的值加1
# incrby key value 将key的值加value
# incrbyfloat key value 将key的值加浮点value
# decr key将key的值减1
# decrby key value将key的值减value
127.0.0.1:6379> set key1 1
OK
127.0.0.1:6379> incr key1
(integer) 2
127.0.0.1:6379> incrby key1 3
(integer) 5
127.0.0.1:6379> incrbyfloat key1 -2.53
"2.47"
127.0.0.1:6379> incrby key1 1
(error) ERR value is not an integer or out of range
```

### 4. Redis哈希(Hash)
每个hash可以存储2^32-1键值对
```bash
# hset设置hash中一个键值对，hmset设置多个
127.0.0.1:6379> hset hash subkey1 value1
(integer) 1
127.0.0.1:6379> hmset hash subkey1 value01 subkey2 value2
OK
# hsetnx当值不存在时设置
127.0.0.1:6379> hsetnx hash subkey3 2
(integer) 1
127.0.0.1:6379> hsetnx hash subkey3 2
(integer) 0
# hget获取hash中的一个键的值，hmget获取多个
127.0.0.1:6379> hget hash subkey1
"value01"
127.0.0.1:6379> hmget hash subkey1 subkey2
1) "value01"
2) "value2"
# 获取hash中全部内容
127.0.0.1:6379> hgetall hash
1) "subkey1"
2) "value01"
3) "subkey2"
4) "value2"
5) "subkey3"
6) "2"
# 删除hash中多个键
127.0.0.1:6379> hdel subkey1 subkey2
(integer) 1
# 指定subkey是否存在
127.0.0.1:6379> hexists hash subkey1
(integer) 1
127.0.0.1:6379> hexists hash subkey4
(integer) 0
# hincrby/hincrbyfloat对subkey的值加上增量
127.0.0.1:6379> hincrby hash subkey3 1
(integer) 3
127.0.0.1:6379> hincrby hash subkey3 -1
(integer) 2
# 获取hash中所有subkey
127.0.0.1:6379> hkeys hash
1) "subkey1"
2) "subkey2"
3) "subkey3"
# 获取hash中所有subkey的值
127.0.0.1:6379> hvals hash
1) "value1"
2) "value2"
3) "2"
# 获取hash中subkey的数量
127.0.0.1:6379> hlen hash
(integer) 3
# 迭代hash中的键值对 
HSCAN key cursor [MATCH pattern] [COUNT count] 

```
### 5. 增量迭代命令
`SCAN SSCAN HSCAN ZSCAN`使用，参考[这里](http://redisdoc.com/key/scan.html)

### 6. Redis列表(List)  
`list`中的元素可以重复
```bash
# LPUSH从左边添加元素 RPUSH从右边添加元素
# LPUSHX从左边添加元素到已经存列表 RPUSHX从右边添加元素已经存列表
127.0.0.1:6379> lpush list lvalue1 lvalue2
(integer) 2
127.0.0.1:6379> rpush list rvalue1 rvalue2
(integer) 4
# LRANGE key start stop (start:0 stop取-1表示结尾)
127.0.0.1:6379> lrange list 0 -1
1) "lvalue2"
2) "lvalue1"
3) "rvalue1"
4) "rvalue2"
# LPOP从左边删除元素 RPOP从右边删除元素
127.0.0.1:6379> lpop list
"lvalue2"
127.0.0.1:6379> rpop list
"rvalue2"
127.0.0.1:6379> lrange list 0 -1
1) "lvalue1"
2) "rvalue1"
# LLEN获取列表长度
127.0.0.1:6379> llen list
(integer) 2
# LINDEX key index通过索引获取列表中元素
127.0.0.1:6379> lindex list 1
"rvalue1"
# LSET key index value通过索引设置列表元素的值
127.0.0.1:6379> lset list 2 rvalue2
(error) ERR index out of range
127.0.0.1:6379> lset list 1 rvalue2
OK
# LREM key count value根据count的值移除与value相等的元素，返回移除个数
# count = 0，移除所有与value相等的值
# count > 0，从头开始，移除count个与value相等的元素
# count < 0，从尾开始，移除|count|个与value相等的元素
127.0.0.1:6379> lrem list 5 value
(integer) 0
127.0.0.1:6379> lrem list 5 rvalue2
(integer) 1
# LTRIM key start end只保留区间[start,end]内的元素，其余删除
127.0.0.1:6379> lpush list 1 2 3 4 5
(integer) 6
127.0.0.1:6379> ltrim list 0 4
OK
127.0.0.1:6379> lrange list 0 -1
1) "5"
2) "4"
3) "3"
4) "2"
5) "1"
# RPOPLPUSH source destination 
# BLPOP key1 [key2 ] timeout 移出并获取列表第一个元素， 如果列表没有元素会阻塞到超时或有可弹出元素为止。
# BRPOP key1 [key2 ] timeout 移出并获取列表尾元素， 如果列表没有元素会阻塞到超时或有可弹出元素为止。
# LINSERT key BEFORE|AFTER pivot value 在列表的元素前或者后插入元素
```

### 7. Redis集合(Set)
集合中的元素唯一，哈希表实现，添加删除查找的复杂度都是$O(1)$，每个集合最大存储$2^32 -1$个成员
```bash
# 添加元素sadd key member1 member2...
127.0.0.1:6379> sadd set mem1 mem2
(integer) 2
# scard 获取集合成员数
127.0.0.1:6379> scard set
(integer) 2
# SREM key mem1 mem2...删除集合中成员
# 集合空时，自动被删除
127.0.0.1:6379> srem set mem1 mem2
(integer) 2
# SISMEMBER key member 判断member是否是key的成员
127.0.0.1:6379> sismember set mem1
(integer) 1
# SMEMBERS key 返回集合中所有成员
127.0.0.1:6379> smembers set
1) "mem1"
2) "mem2"
# SDIFF key1 [key2] 返回给定所有集合的差集
# SDIFFSTORE destination key1 [key2] 返回给定所有集合的差集并存储在 destination 中
# SINTER key1 [key2] 返回给定所有集合的交集
# SINTERSTORE destination key1 [key2] 返回给定所有集合的交集并存储在 destination 中
# SUNION key1 [key2] 返回所有给定集合的并集
# SUNIONSTORE destination key1 [key2] 返回所有给定集合的并集并存储在 destination 中
# SMOVE source destination member 将 member 元素从 source 集合移动到 destination 集合
# SRANDMEMBER key [count] 返回集合中count个随机元素
127.0.0.1:6379> SRANDMEMBER set 2
1) "mem2"
2) "mem3"
# SPOP key count移除并返回集合中的count个随机元素，默认count=1
127.0.0.1:6379> spop set 1
"mem1"
127.0.0.1:6379> smembers set
1) "mem2"
```

### 8. Redis有序集合(sorted set)
成员唯一，但是每个成员关联一个`double`的分数，分数可以重复，按从小到大的顺序排列
```bash
# 添加元素
127.0.0.1:6379> zadd zset 0.2 mem1 0.2 mem2 0.3 mem3 -0.1 mem0
(integer) 4
# ZRANGE key start stop [WITHSCORES] 获取索引范围内的元素，从小到大
127.0.0.1:6379> zrange zset 0 -1
1) "mem0"
2) "mem1"
3) "mem2"
4) "mem3"
# ZREVRANGE key start stop [WITHSCORES] 获取索引范围内的元素，从大到小
127.0.0.1:6379> zrevrange zset 0 -1
1) "mem3"
2) "mem2"
3) "mem1"
4) "mem0"
# ZCARD key 获取有序集合的成员数
127.0.0.1:6379> zcard zset
(integer) 4
# ZCOUNT key min max 计算在指定区间分数的成员数
127.0.0.1:6379> zcount zset 0.2 0.3
(integer) 3
# ZSCORE key member返回成员的分数值
127.0.0.1:6379> zscore zset mem0
"-0.10000000000000001"
# zincrby key increment member加
127.0.0.1:6379> zincrby zset 1 mem0
"0.90000000000000002"
# ZRANK key member 返回指定成员的排名，从小到大
# ZREVRANK key member 返回指定成员的排名，从大到小
127.0.0.1:6379> zrank zset mem0
(integer) 3
127.0.0.1:6379> zrevrank zset mem0
(integer) 0
# ZLEXCOUNT key min max 计算指定字典区间内成员数量
127.0.0.1:6379> ZLEXCOUNT zset [a [z
(integer) 4
127.0.0.1:6379> ZLEXCOUNT zset - +
(integer) 4
# ZRANGEBYLEX key min max [LIMIT offset count] 通过字典区间返回有序集合的成员
# ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT] 返回分数在指定区间内的成员，从小到大
# ZREVRANGEBYSCORE key max min [WITHSCORES] [LIMIT] 返回分数在指定区间内的成员，从大到小
127.0.0.1:6379> ZRANGEBYSCORE zset 0.3 1
1) "mem3"
2) "mem0"
127.0.0.1:6379> ZREVRANGEBYSCORE zset 1 0.3
1) "mem0"
2) "mem3"
# ZINTERSTORE destination numkeys key [key ...] 计算给定的有序集的交集并存储在destination
# ZUNIONSTORE destination numkeys key [key ...] 计算给定的有序集的并集并存储在destination
# ZREM key member [member ...] 移除一个或多个成员
127.0.0.1:6379> zrem zset mem0
(integer) 1
# ZREMRANGEBYLEX key min max 移除给定的排名区间的所有成员
# ZREMRANGEBYSCORE key min max 移除给定分数区间的所有成员
# ZSCAN key cursor [MATCH pattern] [COUNT count] 迭代有序集合中元素（包括成员和分值）
```

****
## 3. Redis Client
参考[这里][3.1]，C/C++主要使用[`hiredis`][3.2]，关于`hiredis`的使用参考[这里][3.3]

## 4. Redis持久化

Redis有两种持久化方式，分别是`RDB`和`AOF`。参考[Redis 设计与实现-RDB][3.4]和[Redis持久化][3.5]。

`RDB`:在Redis运行时，`RDB`根据配置的策略每隔一段时间，保存数据库的快照，重启时通过载入RDB文件来恢复数据，Redis默认设置采用RDB模式，配置中默认生成的`dump.rdb`，路径在`/var/lib/redis`，保存策略是`save 900 1`表示900秒后至少有一个键变动  

`AOF`:记录每个操作的命令，重启后逐条执行命令重建，默认关闭
```cpp
# The filename where to dump the DB
dbfilename dump.rdb

# The working directory.
#
# The DB will be written inside this directory, with the filename specified
# above using the 'dbfilename' configuration directive.
#
# The Append Only File will also be created inside this directory.
#
# Note that you must specify a directory here, not a file name.
dir /var/lib/redis
################################ SNAPSHOTTING  ################################
#
# Save the DB on disk:
#
#   save <seconds> <changes>
#
#   Will save the DB if both the given number of seconds and the given
#   number of write operations against the DB occurred.
#
#   In the example below the behaviour will be to save:
#   after 900 sec (15 min) if at least 1 key changed
#   after 300 sec (5 min) if at least 10 keys changed
#   after 60 sec if at least 10000 keys changed
#
#   Note: you can disable saving completely by commenting out all "save" lines.
#
#   It is also possible to remove all the previously configured save
#   points by adding a save directive with a single empty string argument
#   like in the following example:
#
#   save ""

save 900 1
save 300 10
save 60 10000
```

***
[1.1]:http://www.runoob.com/redis/redis-install.html "install redis"
[1.2]:https://github.com/MSOpenTech/redis/releases "win Redis"
[2.0]:https://redis.io/commands "redis official"
[3.1]:https://redis.io/clients "Redis client"
[3.2]:https://github.com/redis/hiredis "hiredis github"
[3.3]:https://www.zybuluo.com/LIUHUAN/note/364481 "hiredis接口"
[3.4]:http://redisbook.readthedocs.io/en/latest/internal/rdb.html "Redis设计与实现"
[3.5]:https://segmentfault.com/a/1190000002906345 "Redis 持久化 segmentfault"
## 参考资料
1. Redis实战 http://redisinaction.com/preview/chapter1.html 
2. Redis入门 http://www.runoob.com/redis/redis-tutorial.html
3. Redis持久化 https://segmentfault.com/a/1190000002906345
4. hiredis介绍 https://www.zybuluo.com/LIUHUAN/note/364481
5. Redis配置项 http://ckl893.blog.51cto.com/8827818/1770766
6. Redis 设计与实现 http://redisbook.readthedocs.io/en/latest/index.html

***

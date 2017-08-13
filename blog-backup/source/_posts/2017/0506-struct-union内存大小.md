---
title: struct union enum内存大小
categories: 笔记
date: 2017-05-06 16:05:53
updated:
tags: [C++]
description: 结构体、位域、枚举类型内存大小，编译器gcc
---

***

***
## 1. struct内存大小--默认情况下

- 结构体中成员的偏移量是其大小的整数倍
- 结构体大小是所有成员大小的最小公倍数
- 嵌套时，嵌套在内层的结构体的第一个成员的偏移量是其最大元素的整数倍
- 数组元素展开计算

```cpp
// 对齐到4+4+对齐到8+8=24
struct stu1
{
    char c1;
    int int1;
    char c2;
    long long l1;
};

// 对齐到2+2+4+8=16
struct stu2
{
    char c1;
    short s2;
    int int1;
    long long l1;
};

// 24+8+对齐到8=40
 struct stu3
{
    string t; //24 
    long long l;  
    int i;  
};
// 对齐到4+对齐到4+4+4=16
struct stu4
{
    short s1;
    struct
    {
        char c1;//偏移应是4的倍数
        int int1;
    }stu;
    int int2;
};

// 4+4+3*4=20
struct stu5 
{  
    float f1;  
    char c1;  
    char arr[3];  
};  
// 4+补齐到4+3*4=20
struct stu6 
{  
    float f1;  
    char c1;  
    int arr[3];  
};  
// 补齐到4+2=6
struct stu7 
{  
    char arr[3];
    short s1; 
};  
```

****

## 2. #pragma pack(n)//设定为n字节对齐

- 偏移量为n的倍数
- n大于所有成员变量的大小，那么是最大变量的整数倍，否则整体大小为n的整数倍

```cpp
// 8+8+8=24
struct stu7
{
    char c1;
    double d1;
    int int1;
};

// 4+8+4=16
#pragma pack(push)
#pragma pack(4)
struct stu7
{
    char c1;
    double d1;
    int int1;
};
#pragma pack(pop)

// 8+8+8=24
#pragma pack(push)
#pragma pack(8)
struct stu7
{
    char c1;
    double d1;
    int int1;
};
#pragma pack(pop)

// 大小为2
#pragma pack(push) 
#pragma pack(4)
struct stu7
{
    char c1;
    char c2;
};
#pragma pack(pop)
```

****

## 3. 位域
关于位域的存储，复制操作参考[这里][3.1]，当相邻元素相同时，会连续存储直到不能容纳，
当不同时，gcc会压缩存储，vs不会压缩存储，结构体大小对齐到最大类型整数倍。当设置某个位域超出其数据结构所占bit位数时，多出的bit位是用不到的。

```cpp
#include <iostream>
#include <bitset>
using namespace std;
struct bs
{
    unsigned a : 6;
    unsigned b : 12;
    unsigned c : 14;
};

int main(int argc, char **argv)
{
    cout << sizeof(bs) << endl;
    bs b;
    b.a = 1;
    b.b = 1;
    b.c = 1;
    // 当bs不足32字节时，没用的字节是垃圾值
    unsigned int *up = (unsigned int *)&b;
    cout << bitset<sizeof(bs) * 8>(*up) << endl;
    cout << *up << endl;

    char str[] = "1234";
    memcpy(&b, str, sizeof(b));
    cout << bitset<sizeof(bs) * 8>(*up) << endl;
    cout << *up << endl;
    return 0;
}
```

输出结果如下
```bash
4
00000000000001000000000001000001
262209
00110100001100110011001000110001
875770417
```
|字符|ASCII|
|:---|:--:|
|1|`0011 0001`|
|2|`0011 0010`|
|3|`0011 0011`|
|4|`0011 0100`|

当第一次输出b的二进制时其低位到高位依次是`a:0000 01`、`b:0000 0000 0001`、`c:0000 0000 0000 01`也就是`0000 0000 0000 01 00 0000 0000 01 00 0001`，第二次从低到高就是`1234`的ASCII码

```cpp
//占4个字节，e多出2个bit位，用不到
struct bs
{
    unsigned char e:10;
    unsigned d : 2;
};

```

****

## 4. union类型与enum枚举类型
union类型参考[这里][4.1]  
enum枚举类型参考[这里][4.2]，枚举类型的值是个常整型，但不能将浮点数复制给枚举标识符。
```cpp
// 占用大小采用成员最大长度的对齐
//最大长度是short的2字节，而arr需要3字节，故对齐到4字节
union u1
{
    char arr[3];
    short s1;
};
```

***
[1.1]:http://www.cnblogs.com/0201zcr/p/4789332.html
[3.1]:http://c.biancheng.net/cpp/html/102.html
[4.1]:http://c.biancheng.net/cpp/html/2932.html
[4.2]:http://c.biancheng.net/cpp/html/99.html
## 参考资料
1. http://www.cnblogs.com/0201zcr/p/4789332.html


***

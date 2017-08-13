---
title: bitset使用总结
categories: 笔记
date: 2017-05-17 10:52:22
updated:
tags: [C++]
description: bitset使用
---

***

***
##  1. bieset构造
详细介绍参考[这里][url1]，`template <size_t N> class bitset;`其中N必须是一个常量表达式。构造函数接受`unsigned long long`、`string`和`char *`来生成对象，默认无参构造函数产生的对象所有位都为0。其二进制位从0开始编号，称为低位(low-order)，编号结束的二进制位称为高位。
> 当使用整型来初始化`bitset`时，值被转换为`unsigned long long`类型当作位模式类处理，`bitset`中的二进制位是此模式的一个副本。如果`bitset`的大小大于一个`undigned long long`中的二进制位数，则剩余的高位被置为0。如果`bitset`的大小小于一个`unsigned long long`中的位数，则只使用给定值中的地位，超出的高位被丢弃。


```cpp
// bitvec1比初始值小，初始值中的高位被丢弃
bitset<13> bitvec1(0xbeef); //二进制位为1111 0111 0111 1
cout<<sizeof(bitvec1)<<endl; //4 bytes

// bitvec2比初始值大，高位置零
bitset<20> bitvec2(0xbeef); //二进制位为0000 1011 1110 1110 1111

// 64位机器中，0ULL是64个0，因此~0ULL是64个1
bitset<13> bitvec3(~0ULL); // 0-63为1，63-127为0

// string中下标最大的字符用来初始化bitset中下标为0的字符
// 字符串中下标最小的字符对应高位
bitset<32> bitvec4("1100"); //2、3位为1，其余位为0

string str("1111111000000011001101");
bitset<32> bitvec5(str,5,4); //从str[5]开始的四个二进制位，1100
bitset<32> bitvec6(str,str.size()-4); //使用最后四个字符

```

****
## 2. biset操作

|成员函数|解释|
|:-----|:----|
|b.any()|b中是否存在置位的二进制位|
|b.to_ulong|返回一个`unsigned long`值，放不下则返回`overflow_error`异常|
|b.to_ullong|返回一个`unsigned long long`值|
|b.to_string(char zero,char one)|返回一个`string`，表示b中位模式，`zero`和`one`分别默认为0和1|

## 3. 输出数的二进制表示

```cpp
// 输出浮点数二进制
float t = -15.5;
cout<<bitset<sizeof(float) * 8>(*(unsigned int *)&t)<<endl;

// 输出整型的二进制
int tint=2;
cout << bitset<sizeof(int) * 8>(*(unsigned int *)&tint)<<endl;

// bitset内存占用
bitset<33> v;
cout<<sizeof(v)<<endl; //8bytes
```

## 4. 特征向量哈希转换

- 特征向量序列化与反序列化
```cpp
string featureToString(vector<float> &feature)
{
    stringstream ss;
    // 保留小数点后两位
    ss << fixed;
    ss.precision(2);
    for (auto num : feature)
        ss << num << " ";
    return ss.str();
}

vector<float> stringToFeature(string &value)
{
    stringstream ss(value);
    float tmp;
    vector<float> res;
    for (int i = 0; i < bits; ++i)
    {
        ss >> tmp;
        res.push_back(tmp);
    }

    return res;
}
```
- 特征向量转为按bit位存储的哈希码
```cpp
#define BITSET_LENGTH 64
typedef unsigned long long ullong;

string toHash(vector<float> &feature)
{
    if (feature.empty())
        return string("");
    
    size_t n=(feature.size()+BITSET_LENGTH-1)/BITSET_LENGTH;
    ullong *p=new ullong[n];

    bitset<BITSET_LENGTH> bitvec;
    size_t j = 0,k=0;
    for (size_t i = 0; i < feature.size(); ++i)
    {
        bitvec[j] = feature[i] > 0 ? 1 : 0;
        ++j;
        if (j == BITSET_LENGTH)
        {
            p[k]=bitvec.to_ullong();
            bitvec.reset();
            ++k;
            j = 0;
        }
    }
    if(k<n) p[k]=bitvec.to_ullong();
    string res((char*)p,n*sizeof(ullong));
    delete [] p;
    return res;
}

// 反向转换 将toHash转换的string中的bit转为01的字符串
string SearchTool::codeToBinaryString(string &code)
{
    int n=code.size()/8;
    string res;
    ullong *p=(ullong *)&code[0]; 
    for(int i=0;i<n;++i)
    {
        bitset<BITSET_LENGTH> bitvec(p[i]);
        res+=bitvec.to_string();
    }
    return res;
}
```

```

***
[url1]:http://www.cplusplus.com/reference/bitset/bitset/ "bitset"
## 参考资料
1. 《C++ Primer 5th》
***

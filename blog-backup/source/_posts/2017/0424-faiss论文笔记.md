---
title: faiss论文笔记-Billion-scale similarity search with GPUs
categories: 笔记
date: 2017-04-24 10:14:40
updated: 2017-04-26
tags: [论文阅读,GPU]
description:
---

***

***
##  摘要

作者认为当前的`GPU`上进行的工作在并行度或内存分级的利用上不够高效，于是提出了一种`GPU`上执行的`k-selection`的方法能够达到理论性能峰值的 55%，最近邻搜索的速度是之前顶尖的`GPU`上执行的[算法][1]的8.5倍。通过优化设计基于乘积量化的暴力搜索、近似搜索和压缩域搜索，该方法可以适用于不同的相似性搜索场景，在左右这些场景中作者提出的方法都超出了之前顶尖的工作。使用文中的方法和4台` Maxwell Titan X GPU `在YFCC100M数据集的95M图像上构建一个高准确率的k-NN图只需要35分钟，构造一个链接十亿向量的图不超过12小时。然后作者把代码开源在了[这里][2]。

## 1. 简介

作者首先分析了多媒体数据的穷举搜索或者基于非穷举的精确的索引在十亿级别的数据集上是不可行的。为了处理大数据集无法装入RAM，许多方法使用编码方法对向量内部压缩重表示，这对GPU这样内存首先的设备尤其方便。流行的向量压缩方法可以被分类为二值码和量化的思想。这篇论文专注于`Product Quantization(PQ)`码，作者认为 PQ 相比于哈希码更高效，而且哈希码在处理非穷尽搜索时有额外的开销。
原始的`PQ`想法[IVFADC][3]提出后，有许多研究者做了改进，但大部分的改进很难在 GPU 上高效的执行。也有许多在GPU上相似搜索的方法，但是大部分使用哈希码、小数据集或者做穷尽搜索。作者了解到的当前适合在十亿级别数据集的GPU方法是Wieschollek的工作[Product Quantization Tree][1]。

作者认为这篇论文的主要贡献是
- 一种 GPU 的`k-selection`算法(operating in fast register memory and exible enough to be fusable with other kernels, for which we provide a complexity analysis)
- 一种近似最优的 GPU 上执行精确和近似 k 近邻搜索的算法布局
- 一系列实验表明在中型到大型数据集的近邻搜索任务中，这些改进比先前的工作有很大的提升，无论是单 GPU 还是多 GPU 

文章的组织结构
|章节号|内容|
|:---|:---|
|2|内容和符号|
|3|回顾GPU结构和讨论在GPU上做相似搜索的问题|
|4|介绍作者提出的`k-selection`算法|
|5|算法计算布局的细节|
|6|实验及对比|

## 2. 问题陈述

k最近邻搜索问题：给一个查询向量$x \in R^d$和数据集$[y_i] (y_i \in R^d, i=0:l)$需要搜索： 
  
$$ 
L= {k-argmin}_{i=0:l} {|| x-y_i ||}_2 
$$

也就是根据L2查询计算数据集中与查询向量最近的k个向量。

### Batching
将数据集按$n_q$批量并行查询，查询出$n_q \times k$个元素和其对应的索引。
### Exact search
精确搜索$n_q$个查询向量的k最近邻，计算全部 pair 的距离矩阵 $D = [||x_j-y_i|| _2 ^2] \;(j=0:n_q,i=0:l \in R^{n_q \times l})$实际中使用下面式子计算   

$$
||x_j - y_i|| _2 ^2 = ||x_j||^2 + ||y_i||^2 -2< x_j, y_i >
$$

前两个元素可以预先计算，后一个元素等于$XY^T$，其中X的每一行是一个查询向量，Y的每一行是数据集中一个向量，则k最近邻就是每一行选k个最小值。

### Compressed-domain search
考虑[IVFADC][3]索引结构，其依赖两层量化，并且数据库中存储编码后的向量，数据库中的向量`y`使用下式近似

$$ 
y \approx q(y)=q_1(y) +q_2(y-q_1(y))
$$

其中$q_1:R^d \to C_1 \subset R^d \; and \; q_2:R^d \to C_2 \subset R^d$是两个量化器，将向量映射到一个有限集中，由于集合有限，$q(y)$被编码为$q_1(y)$和$q_2(y-q_1(y))$的索引，$q_1$是一个粗糙量化器，$q_2$是一个精细量化器，将第一级粗糙量化后的残差向量编码。

ADC(Asymmetric Distance Computation):

$$
L_{ADC}=k-argmin_{i=0:l}||x-q(y_i)||_2
$$
    
因为IVFADC不是穷举搜索，计算距离的向量是依赖第一级量化器预先选择的：

$$
L_{IVF}=\tau-argmin_{c \in C_1}||x-c||_2
$$
其中参数$\tau$是粗糙量化的中心点个数，粗糙量化器做一个准确距离的最近邻搜索，然后IVFADC计算：

$$
L_{IVFADC}=k-argmin_{i=0:l \, s.t. q_1(y_i) \in L_{IVF}}||x-q(y_i)||_2
$$

### The quantizers
量化器$q_1$的结果应该有相对低数量的值，防止倒排列表爆炸，一般使用$|C_1| \approx \sqrt l$，使用k-means训练。对于$q_2$，为了更多的重表示，我们可以接受花费更多的内存，向量ID一般是(4或8字节的整数)也被存储在倒排列表中，所以编码少于ID是没有意义的，也就是$log_2|C_2|>4 \times 8$

### Product quantizer
作者使用乘积量化器作为$q_2$的量化，乘积量化通过将向量`y`分为`b`个子向量，$y=[y^0 \ldots y^{b-1}]$其中`b`是维数d的偶数因子，每一个子向量使用他自己的量化器量化，产生元祖$(q^0(y^0), \ldots ,q^{b-1}(y^{b-1}))$，子量化器一般有256个reproduction值，能够存储在一个字节（个人理解量化之后是中心点的编号或者索引）。所以乘积量化的值$q_2(y)=q^0(y^0)+256 \times q^1(y^1)+ \cdots +256^{b-1} \times q^{b-1}(y^{b-1})$，从存储的角度来看就相当于是子量化器量化的结果连在一起。因此乘积量化器产生`b`字节的码和$|c_2|=256^b$个reproduction values.

## 3. GPU：Overview And K-Selection
### 3.1 Architecture
- GPU lanes and warps
Nvidia GPU是一种通用计算器，使用一个32-wide向量 CUDA threads(the warp);在warp中独立的线程称为lane，ID从0-31。忽略线程这个术语，warp在CPU中最好的类比是一个分离的CPU硬件线程(hardware thread)，由于warp共享一个指令计数器。每个lane在共享寄存器文件中有255个32bit的寄存器，CPU类比是有255个32位的向量寄存器，warp lanes类比于SIMD向量lanes.

- Collections of warps

***
[1]:https://github.com/cgtuebingen/Product-Quantization-Tree
[2]:https://github.com/facebookresearch/faiss
[3]:https://lear.inrialpes.fr/pubs/2011/JDS11/jegou_searching_with_quantization.pdf


## 参考资料
1. 
***

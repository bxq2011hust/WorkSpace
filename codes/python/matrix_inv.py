# coding=utf-8
import numpy as np

a = np.array([[1, 0, 0, 0, 1], [0, 1, 0, 0, 1], [0, 0, 1, 0, 1],
              [0, 0, 0, 1, 1], [1, 0, 0, 1, 1]])  # 初始化一个非奇异矩阵(数组)

# print(np.linalg.inv(a))  # 对应于MATLAB中 inv() 函数

aa = np.dot(a.T, a)

Y = np.array([5, 6, 7, 8, 11])
print(aa)
aaa = np.dot(np.linalg.inv(aa), a.T)
print(aaa)

w = np.dot(aaa, Y)
print(w)
# 矩阵对象可以通过 .I 更方便的求逆
# A = np.matrix(a)
# print(A.I)
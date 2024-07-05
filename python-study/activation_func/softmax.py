#!/usr/bin/python3
#coding=utf-8

# 归一化指数函数
# 假设我们有一个数组，V，Vi表示V中的第i个元素，那么这个元素的softmax值就是
# softmax(i) = math.exp(i) / sum([math.exp(j) for j in z])
# Sigmoid ：适用于二分类问题，输出一个代表正类概率的值。
# Softmax ：扩展到多类分类，为每个类别提供概率分布。


import math
import numpy as np

z = [1.0, 2.0, 3.0, 4.0, 1.0, 2.0, 3.0]
z_exp = [math.exp(i) for i in z]

print(z_exp)
# Result: [2.72, 7.39, 20.09, 54.6, 2.72, 7.39, 20.09]

sum_z_exp = sum(z_exp)
print(sum_z_exp)
# Result: 114.98

softmax = [round(i / sum_z_exp, 3) for i in z_exp]
print(softmax)
# Result: [0.024, 0.064, 0.175, 0.475, 0.024, 0.064, 0.175]
# 4是最大值
# 也显示了这个函数通常的意义：对向量进行归一化，凸显其中最大的值并抑制远低于最大值的其他分量。

def softmax(x):
    """
    计算输入x的Softmax。
    参数x可以是x的向量或矩阵。
    返回与x形状相同的Softmax向量或矩阵。
    """
    # 防止溢出，通过减去x的最大值来进行数值稳定性处理
    e_x = np.exp(x - np.max(x))
    return e_x / e_x.sum(axis=0)

# 向量示例
x = np.array([2.0, 1.0, 0.1])
print("向量Softmax:", softmax(x))

# 矩阵示例
x_matrix = np.array([[1, 2, 3], [3, 2, 1], [0, 0, 0]])
print("矩阵Softmax:\n", softmax(x_matrix))

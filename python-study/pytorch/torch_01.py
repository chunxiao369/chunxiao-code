import numpy as np
import torch
import torchvision
import matplotlib.pyplot as plt
from time import time
from torchvision import datasets, transforms
from torch import nn, optim
a=torch.Tensor([[1,2,3],[4,5,6]])
b=torch.Tensor([1,2,3,4,5,6])

print(a.shape)
print(b.shape)

print(a.shape[0])
print(b.shape[0])

print(a)
print(a.view(1,6))
print(a.view(2,-1))

print(b)
print(b.view(1,6))
print(b.view(6,-1))

print(a.numpy())
print(a.numpy()[0])

print(b.numpy())
print(b.numpy()[0])
#就是子啊fc全连接之前，
#比如最终生成(50,16,4,4)的特征图
#我们将16张7*7特征图铺平为一个一维向量。因为batch_size设置为50. 那么img.shape[0]=50。
#我们就生成了50 个 一位向量。输进全连接层。

import numpy as np
import math
import torch
from time import time
from torchvision import datasets, transforms
from torch import nn, optim

'''
tensor[:, None]：在张量的最后一个维度后面添加一个新的维度。
tensor[:, None, :]：在倒数第二个维度后面添加一个新的维度。
tensor[None, :]：在张量的第一个维度前面添加一个新的维度。
'''
x = torch.randn(3,4)
print(x)
y = x[:, None, None, :] #y.shape (3,1,1,4)
print(y.shape, y)
y = x[0, None, None, :] #y.shape (1,1,4) 选择第一个维度的第一个元素，并在第二和第三维度之前增加新的维度
print(y.shape, y)
y = x[1, None, None, :] #y.shape (1,1,4) 选择第一个维度的第二个元素，并在第二和第三维度之前增加新的维度
print(y.shape, y)

x = torch.randn(2, 3, 4)
y = x[:,None,None]     # y.shape torch.Size([2, 1, 1, 3, 4])
print(y.shape)
y = x[:,:,:,None,None] # y.shape torch.Size([2, 3, 4, 1, 1])
print(y.shape)



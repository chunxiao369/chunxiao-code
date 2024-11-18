import numpy as np
import math
import torch
from time import time
from torchvision import datasets, transforms
from torch import nn, optim

# ----------float example----------------
q = torch.randn(3, 2)
k = torch.randn(2, 3)
qk = torch.matmul(q,k)
qk_soft = nn.functional.softmax(qk,dim=0)
#print(qk)
#print(qk_soft)
q = torch.randn(4, 5, 3, 2)
k = torch.randn(4, 5, 3, 2)
v = torch.randn(4, 5, 3, 2)
z = torch.transpose(k, -2, -1)
qk = torch.matmul(q,z)
print(qk[0][0])
qk_soft = nn.functional.softmax(qk, dim=-1)
print("after sofmax", qk_soft[0][0])

print("q size is: ", q.size(), q.size()[-1])
qk = qk / math.sqrt(q.size()[-1])
print("after sqrt", qk[0][0])
qk_soft = nn.functional.softmax(qk, dim=-1)
print("after sqrt and sofmax", qk_soft[0][0])

#print(z)
#print(qk.size(), qk.size()[-1])
# ----------float example----------------

# ----------int example----------------
t = torch.randint(0, 8, (4,3,2))
z = torch.transpose(t, -2, -1)
#print(t)
#print(z)
a = torch.randint(0, 8, (3,2))
b = torch.randint(0, 8, (2,3))
ab = torch.matmul(a,b)
#print(a)
#print(b)
fab=ab.float()
#print(fab)
absoft = nn.functional.softmax(fab,dim=0)
#print(absoft)
# ----------int example----------------

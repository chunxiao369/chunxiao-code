import torch
import torch.nn.functional as F

# 张量相加的广播机制
# 如果一个张量在某个维度是 1，而另一个张量该维度是N，则自动扩展 1->N。

# 假设最大序列长度为 10，隐藏维度为 8
max_seq_len = 10
d_model = 8  # 词嵌入的维度

# 生成位置索引 (1, max_seq_len, 1)
position = torch.arange(max_seq_len).unsqueeze(1)  # 形状: [10, 1]

# 计算分母 10000^(2i/d_model)
div_term = torch.exp(torch.arange(0, d_model, 2) * (-torch.log(torch.tensor(10000.0)) / d_model))

# 计算位置编码
pe = torch.zeros(max_seq_len, d_model)  # 初始化
pe[:, 0::2] = torch.sin(position * div_term)  # 偶数位置
pe[:, 1::2] = torch.cos(position * div_term)  # 奇数位置


# 使得 pe 形状变为 (1, max_seq_len, d_model)，用于广播
pe = pe.unsqueeze(0)  # 形状: [1, 10, 8]
#pe = pe.unsqueeze(1)  # 形状: [10, 1, 8]
print(pe)

# 生成一个 batch_size=4, seq_len=6 的随机输入张量
batch_size = 4
seq_len = 6
x = torch.rand(batch_size, seq_len, d_model)  # [4, 6, 8]
print(x)

# 位置编码自动广播到 batch_size
x = x + pe[:, :seq_len, :]  # 这里 pe[:, :seq_len, :] 形状变为 [1, 6, 8]，通过广播匹配 [4, 6, 8]
#x = x + pe[:batch_size, :, :]  # 这里 pe[:batch_size, :, :] 形状变为 [4, 1, 8]，通过广播匹配 [4, 6, 8]
print(x)

print("Position Encoding shape:", pe.shape)  # [1, 10, 8]
print("Input tensor shape:", x.shape)  # [4, 6, 8]


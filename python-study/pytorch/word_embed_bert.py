from transformers import AutoTokenizer, AutoModel
import torch

# 加载预训练模型和分词器
model_name = "./bert-base-uncased"  # 可以选择其他模型，如 "gpt2", "roberta-base" 等
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModel.from_pretrained(model_name)

# 输入单词
word = "example"

# 将单词转换为 token
inputs = tokenizer(word, return_tensors="pt")

# 获取词向量
with torch.no_grad():
    outputs = model(**inputs)

# 提取词向量（取最后一层的隐藏状态）
word_vectors = outputs.last_hidden_state

# 打印词向量
print(f"词向量形状: {word_vectors.shape}")
print(f"词向量: {word_vectors}")

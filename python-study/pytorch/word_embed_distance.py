from transformers import AutoTokenizer, AutoModel
import torch
import torch.nn.functional as F

# 加载预训练模型和分词器
model_name = "./bert-base-uncased"  # 可以选择其他模型，如 "gpt2", "roberta-base" 等
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModel.from_pretrained(model_name)

# 输入单词
word = "example"

def get_word_embedding(word):
    inputs = tokenizer.encode(word, return_tensors='pt')
    with torch.no_grad():
        outputs = model(inputs)
        embeddings = outputs.last_hidden_state
    return embeddings[0][0]

# 获取各个词的词向量
woman_embedding = get_word_embedding("woman")
man_embedding = get_word_embedding("man")
girl_embedding = get_word_embedding("girl")
boy_embedding = get_word_embedding("boy")
example_embedding = get_word_embedding("example")

def cosine_similarity(a, b):
    return torch.dot(a, b) / (torch.norm(a) * torch.norm(b))

def euclidean_distance(a, b):
    return torch.norm(a - b)

# 计算余弦相似度
similarity_example_girl  = cosine_similarity(example_embedding, girl_embedding)
similarity_boy_girl  = cosine_similarity(boy_embedding, girl_embedding)
similarity_man_woman = cosine_similarity(man_embedding, woman_embedding)

# 计算欧几里得距离
distance_example_girl  = euclidean_distance(example_embedding, girl_embedding)
distance_boy_girl  = euclidean_distance(boy_embedding, girl_embedding)
distance_man_woman = euclidean_distance(man_embedding, woman_embedding)

print("Cosine similarity between example and girl:", similarity_example_girl)
print("Cosine similarity between boy and girl:", similarity_boy_girl)
print("Cosine similarity between man and woman:", similarity_man_woman)
print("Euclidean distance between example and girl:", distance_example_girl)
print("Euclidean distance between boy and girl:", distance_boy_girl)
print("Euclidean distance between man and woman:", distance_man_woman)

# 将单词转换为 token
#inputs = tokenizer(word, return_tensors="pt")

# 获取词向量
#with torch.no_grad():
#    outputs = model(**inputs)

# 提取词向量（取最后一层的隐藏状态）
#word_vectors = outputs.last_hidden_state

# 打印词向量
#print(f"词向量形状: {word_vectors.shape}")
#print(f"词向量: {word_vectors}")

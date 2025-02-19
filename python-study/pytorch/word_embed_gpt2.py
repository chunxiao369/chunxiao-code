#from transformers import AutoTokenizer, AutoModelForCausalLM
#from transformers import AutoTokenizer, GPT2LMHeadModel
from transformers import AutoTokenizer, GPT2Model
import torch

# 加载 GPT-2 的分词器和模型
model_name = "./gpt2"  # 可以选择其他 GPT-2 变体，如 "gpt2-medium", "gpt2-large" 等
tokenizer = AutoTokenizer.from_pretrained(model_name)
#model = AutoModelForCausalLM.from_pretrained(model_name)
model = GPT2Model.from_pretrained(model_name)

# 输入单词
#word = "Hello, my dog is cute"
word = "example"

# 将单词转换为 token
inputs = tokenizer(word, return_tensors="pt")  # 返回 PyTorch 张量

# 默认情况下，所有的张量操作都会被记录，以便在反向传播时计算梯度。然而，在类似模型推理的场景中，我们不需要梯度，不需要更新模型参数。
# 获取词向量
with torch.no_grad():
    outputs = model(**inputs)

print(outputs.keys())
# 提取词向量（取最后一层的隐藏状态）
word_vectors = outputs.last_hidden_state

# 打印词向量
print(f"词向量形状: {word_vectors.shape}")
print(f"词向量: {word_vectors}")

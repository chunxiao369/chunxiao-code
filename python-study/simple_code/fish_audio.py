#!/usr/bin/python3.10
from gradio_client import Client, handle_file

client = Client("http://10.17.8.207:7860/")
result = client.predict(
#		text="Hello!!",
		text="这份代码是一个纯C语言实现的 Llama-2 Transformer 模型推理程序。它能够加载预先训练好的 Llama-2 模型权重，并根据用户输入的提示（prompt）生成文本，或者进行交互式聊天。代码设计精炼，不依赖任何大型深度学习框架（如 PyTorch 或 TensorFlow），仅使用标准C库和一些基本的系统调用，实现了完整的 Transformer 推理流程。",
		reference_id="boy2",
		reference_audio=handle_file("./T0001G0049S0310.wav"),
		reference_text="这份代码是一个纯C语言实现的 Llama-2 Transformer 模型推理程序。它能够加载预先训练好的 Llama-2 模型权重，并根据用户输入的提示（prompt）生成文本，或者进行交互式聊天。代码设计精炼，不依赖任何大型深度学习框架（如 PyTorch 或 TensorFlow），仅使用标准C库和一些基本的系统调用，实现了完整的 Transformer 推理流程。",
		max_new_tokens=0,
		chunk_length=200,
		top_p=0.8,
		repetition_penalty=1.2,
		temperature=0.7,
		seed=124,
		use_memory_cache="off",
		api_name="/partial"
)
print(result)

# whisper
# whisper ./dehua_zh.wav --model base --language zh

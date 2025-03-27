import requests
import json
import argparse
from langchain_core.prompts import PromptTemplate

url = "http://10.17.8.201:11434/api/chat"
llm_model = "deepseek-r1:32b"
version = "V1.0.0"

def main():
    #parser.add_argument("--version", type=int, help="Number of training epochs.", default=num_epochs)
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--version", action="version", version=f"%(prog)s {version}", help="show version and exit.")
    parser.add_argument("--file", type=str, help="file for review.", default="code_view.py")
    argv = parser.parse_args()

    review_file = argv.file
    # read your code
    with open(review_file, 'r') as file:
        # Read the entire content of the file
        code_snippet = file.read()

    prompt_template = PromptTemplate.from_template(
        """
        你是一个资深的软件开发者.
        你的工作是读懂一段代码，并且给出这段代码的概述\n\n

        ```python
        {code_snippet}
        ```

        给出这段代码中的重要的{functions}个函数，对这五个函数的作用和函数的流程作出说明。
        Between functions leave {new_line} new line space.
        """
    )

    prompt_input = prompt_template.format(
        code_snippet=code_snippet,
        functions=5, new_line=1)

    data = {
        "model": llm_model,
        "messages": [
            {
                "role": "user",
                "content": prompt_input
            }
        ],
        "stream": False
    }
    try:
        response = requests.post(url, json=data)
        #print(response.text)
        # 将 JSON 字符串解析为 Python 字典
        data = json.loads(response.text)

        # 提取 message 中的 content 字段
        content = data["message"]["content"]

        # 输出提取的内容
        print(content)
    except requests.exceptions.RequestException as e:
        print(f"Request failed: {e}")

if __name__ == "__main__":
    main()

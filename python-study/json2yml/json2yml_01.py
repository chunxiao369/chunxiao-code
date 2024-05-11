import json
import yaml

json_data = '{"name": "John", "age": 30, "city": "New York"}'
data = json.loads(json_data)
yaml_data=yaml.dump(data, default_flow_style=False)
print(yaml_data)

json_data = '{"name": "John", "age": 30, "place":{"city": "New York", "streat": "No4"}, "bookList": [{"BookName": "python"}, {"BookName": "c lang"}]}'
data = json.loads(json_data)
yaml_data=yaml.dump(data, default_flow_style=False)
print(yaml_data)

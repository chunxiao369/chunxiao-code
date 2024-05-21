import json
import yaml

# json.dumps 将python字典dict对象转为json格式
# json.dump  将python字典dict对象转为文件
# json.load  将文件句柄转为python dict
# json.loads 针对内存对象，将string转换为python dict 
json_data = '{"name": "John", "age": 30, "city": "New York"}'
data = json.loads(json_data)
yaml_data=yaml.dump(data, default_flow_style=False)
print(yaml_data)

#json_data = '{"name": "John", "age": 30, "place":{"city": "New York", "streat": "No4"}, "bookList": [{"BookName": "python"}, {"BookName": "c lang"}]}'
json_data = '{ \
       "name" : "NodeWarning",\
       "expr" : "nvi_cpu_cpu_usage > 60", \
       "for" : "5s", \
       "severity" : "warning", \
       "summary": "Node {{ $labels.instance }} cpu overflow" \
}'
dict_init = {'alert': '', 'expr': '', 'for': '', 'labels': {'severity': ''}, 'annotations': {'summary': ''}}
data = json.loads(json_data)
yaml_data=yaml.dump(data, default_flow_style=False)
print(yaml_data)

yaml.warnings({'YAMLLoadWarning':False})
f=open('./alert2.rules','r',encoding='utf-8')
cfg=f.read()
#print(cfg)
d=yaml.safe_load(cfg)
f.close()
print(type(d), d)
#print(json.dumps(d))
#print(d.keys())
#print("value len: ", len(d.values()))
#for vl in d.values():
#    vvl = vl[0]
##print(type(vvl), vvl)
#print(type(vvl))
#print(vvl["rules"])
cfg_init = {'groups': [{'name': 'userdefine', 'rules': []}]}
print(cfg_init['groups'][0]['rules'])
core_rules = cfg_init['groups'][0]['rules']
core_rules.append(dict_init)
cfg_init['groups'][0]['rules'] = core_rules
print("type: ", type(core_rules), "test core rule len is: ", len(core_rules))
print(cfg_init)
print("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx")
print(d['groups'][0]['rules'][0])
core_rules = d['groups'][0]['rules']
print("type: ",type(core_rules), "core rule len is: ", len(core_rules))
dict_init['alert'] = data['name']
dict_init['expr']  = data['expr']
dict_init['for']   = data['for']
dict_init['labels']["severity"]     = data['severity']
dict_init['annotations']["summary"] = data['summary']
core_rules.append(dict_init)
print("type: ",type(core_rules), "after append core rule len is: ", len(core_rules))
d['groups'][0]['rules'] = core_rules
#print(d)

#with open('alert.json','w') as json_file:
#    json.dump(d, json_file)
#output = json.load(open('alert.json'))
#json_file.close()

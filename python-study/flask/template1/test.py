# -*- coding:utf-8 -*-
from flask import Flask,render_template_string

app = Flask(__name__)

@app.route('/')
def v_index():
    uname = 'xu.chunxiao@embedway.com'
    tpl = '<h1>Welcome, {{username}}</h1>'
    return render_template_string(tpl,username=uname)
    
app.run(host='0.0.0.0',port=5000)

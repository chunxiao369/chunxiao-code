# -*- coding:utf-8 -*-
import sys
from flask import Flask, render_template, request
import forms

app = Flask(__name__)
app.config.from_object('config')

@app.route('/login',methods = ['GET' , 'POST'])
def login():
    #接收参数
    form = forms.LoginForm()
    print form.validate_on_submit()
    print form.validate()
    print form.is_submitted()
    #提交验证
    if request.method == 'POST':
        print request.form.get('username')
        print request.form.get('password')
    if form.is_submitted():
        #打印(ps:验证逻辑自行补上)
        print('username:' + form.username.data + ', password:' + form.password.data)
        #返回到index.html页面
        return render_template('index.html',form = form)
    #未提交
    return render_template('login.html',form = form)    
app.run(host='0.0.0.0',port=5000)

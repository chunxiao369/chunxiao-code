#!/usr/bin/env python
#coding=utf-8

from flask import Flask
app = Flask(__name__)

##hello world
#@app.route("/")
#def hello():
#    return "Hello World!"

#index请求
@app.route("/index")
def index():
    return 'index'

##拦截Post或者Get方法
#@app.route("/index",methods = ['GET'])
#def index():
#    return 'index'

##Restful支持
#@app.route("/index/<username>")
#def index(username):
#    return username

if __name__ == "__main__":
    app.run(host='0.0.0.0')

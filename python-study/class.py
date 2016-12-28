#coding: UTF-8
import sys #  模块，sys指向这个模块对象
import inspect
def foo(): pass # 函数，foo指向这个函数对象
 
class Cat(object): # 类，Cat指向这个类对象
    '''aaaaaaaaaaaaa'''
    def __init__(self, name='cat_1024', abc="abc", aaa="aaa"):
        self.name = name
    def sayHi(self): #  实例方法，sayHi指向这个方法对象，使用类或实例.sayHi访问
        print self.name, 'says Hi!' # 访问名为name的字段，使用实例.name访问
 
cat = Cat() # cat是Cat类的实例对象
 
print Cat.sayHi # 使用类名访问实例方法时，方法是未绑定的(unbound)
#print cat.sayHi # 使用实例访问实例方法时，方法是绑定的(bound)
cat.sayHi() # 使用实例访问实例方法时，方法是绑定的(bound)

print "-----------------------------------------"
cat = Cat('kitty')
print cat.name # 访问实例属性
cat.sayHi() # 调用实例方法

print dir(cat) # 获取实例的属性名，以列表形式返回
if hasattr(cat, 'name'): # 检查实例是否有这个属性
    setattr(cat, 'name', 'tiger') # same as: a.name = 'tiger'
print getattr(cat, 'name') # same as: print a.name
getattr(cat, 'sayHi')() # same as: cat.sayHi()
print "-----------------------------------------"
print Cat.__doc__           # None
print Cat.__name__          # Cat
print Cat.__module__        # __main__
print Cat.__bases__         # (<type>,)
print Cat.__dict__          # {'__module__': '__main__', ...}</type>
print "-----------------------------------------"
import fnmatch as m
print m.__doc__.splitlines()[0] # Filename matching with shell patterns.
print m.__name__                # fnmatch
print m.__file__                # /usr/lib/python2.6/fnmatch.pyc
print m.__dict__.items()[0]     # ('fnmatchcase', <function>)</function>
print "-----------------------------------------"
print cat.__dict__
print cat.__class__
print cat.__class__ == Cat # True
print "-----------------------------------------"
im = cat.sayHi
im()
print im.im_func
print im.im_self # cat
print im.im_class # Cat
print "-----------------------------------------"
print cat.__class__.__init__.im_func
print cat.__class__.__init__.im_func.func_code
print cat.__class__.__init__.im_func.func_code.co_varnames[0:]
print cat.__class__.__init__.im_func.func_code.co_varnames[1]
print "-----------------------------------------"
def gen():
    for n in xrange(5):
        yield n
g = gen()
print g             # <generator object gen at 0x...>
print g.gi_code     # <code object gen at 0x...>
print g.gi_frame    # <frame object at 0x...>
print g.next()      # 0
print g.next()      # 1
print "is running", g.gi_running  # 0
for n in g:
    print n,        # 2 3 4
print "-----------------------------------------"


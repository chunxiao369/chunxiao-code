#将类方法转换为类属性，可以用.直接获取属性值或者对属性进行赋值
#使用property类来实现，也可以使用property装饰器实现，二者本质是一样的。多数情况下用装饰器实现。

class Student(object):
    @property
    def score(self):
        return self._score

    @score.setter
    def score(self, value):
        if not isinstance(value ,int):
            raise ValueError("must integer")
        if value < 0 or value>100:
            raise ValueError("must between 0 and 100")
        self._score = value

student = Student()
student.score = 65.2
student.score = 101
student.score = 65
print(student.score)

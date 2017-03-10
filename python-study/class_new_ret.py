
class positiveinteger(int):
    def __init__(self, value):
        super(positiveinteger,self).__init__(self, abs(value))
    def __new__(cls, value):
        return super(positiveinteger,cls).__new__(cls, abs(value))
i = positiveinteger(-3)
print i

class positiveinteger2(int):
    def __init__(self, value):
        super(positiveinteger2, self).__init__(self, abs(value))
    def __new__(cls, value):
        super(positiveinteger2, cls).__new__(cls, abs(value))
i = positiveinteger2(-3)
print i


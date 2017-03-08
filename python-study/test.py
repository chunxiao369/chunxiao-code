
class positiveinteger(int):
#   def __init__(self, value):
#       super(positiveinteger,self).__init__(self, abs(value))
    def __new__(cls, value):
        return super(positiveinteger,cls).__new__(cls, abs(value))
i = positiveinteger(-3)
print i

# http://python.jobbole.com/80956/
# This is our decorator
def simple_decorator(f):
    # This is the new function we're going to return
    # This function will be used in place of our original definition
    def wrapper():
        print "Entering Function"
        f()
        print "Exited Function"
 
    return wrapper
 
@simple_decorator 
def hello():
    print "Hello World"
 
hello()

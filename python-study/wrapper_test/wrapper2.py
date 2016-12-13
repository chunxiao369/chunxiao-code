#!/usr/bin/python
import functools
import sys

debug_log = sys.stderr

def trace(func):
    if debug_log:
        @functools.wraps(func)
        def callf(*args, **kwargs):
            """A wrapper function."""
            debug_log.write('Calling function: {}\n'.format(func.__name__))
            res = func(*args, **kwargs)
            debug_log.write('Return value: {}\n'.format(res))
            return res
        return callf
    else:
        return func

@trace
def square(x):
    """Calculate the square of the given number."""
    return x * x

def _square(x):
    """Calculate the square of the given number, xu.chunxiao"""
    return x * x

if __name__ == '__main__':
    square(3)
    print(square.__doc__)
    print(square.__name__)

    square2 = trace(_square)
    square2(3)
    print(square2.__doc__)
    print(square2.__name__)
    print square2.__globals__

'''
from wrapper import square
help(square)
print(square.__name__)
'''

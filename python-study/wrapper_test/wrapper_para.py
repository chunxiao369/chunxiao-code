#!/usr/bin/python

import functools
import sys

def trace(log_level):
    def impl_f(func):
        print(log_level, 'Implementing function: "{}"'.format(func.__name__))
        return func
    return impl_f

@trace('[INFO]')
def print_msg(msg):
    print(msg)

@trace('[DEBUG]')
def assert_(expr):
    assert expr

print_msg('Hello, world!')
assert_('Hello, world!')

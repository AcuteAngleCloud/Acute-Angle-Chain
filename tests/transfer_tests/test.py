#!/usr/bin/python
import os
import time
import platform

from timeit import timeit

def func():
    s = 0
    for i in range(1001):
        s += i
        print(str(s-i)+'+'+str(i)+'='+str(s))

t = timeit('func()', 'from __main__ import func', number=1)
print(t)

strsystem = platform.system()
print(strsystem)


import platform

print(os.name)

print platform.uname()



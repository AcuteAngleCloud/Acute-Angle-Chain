#!/usr/bin/python
import sys
import os

for line in open('walt_pin'):
    l = line.strip().split(',')
    os.system('claac wallet unlock -n ' + l[0] + ' --password ' + l[1])


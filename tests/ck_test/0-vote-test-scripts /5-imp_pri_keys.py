#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

for line in open('acc_keys'):
    l = line.split(',')
    f('claac wallet import ' + l[1])
    f('claac wallet import ' + l[3])

    




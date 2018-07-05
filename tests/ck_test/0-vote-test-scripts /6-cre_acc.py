#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

for line in open('acc_keys'):
    l = line.strip().split(',')
    f('''claac system newaccount --transfer --stake-net "1000.0000 AAC" --stake-cpu "1000.0000 AAC" --buy-ram "1000.0000 AAC" aacio ''' + l[0] + ' ' + l[2] + ' ' + l[4])
    print("create account "+ l[0] +" ok!")
    time.sleep(0.05)



    




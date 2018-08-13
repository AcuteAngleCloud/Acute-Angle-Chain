#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print


voterList = []

for line in open('vo_keys'):
    accName = line[:line.find(',')]
    if accName[0:2] == 'vo':
        voterList.append(accName)


#all voter stake

n = 0
for voter in voterList:
    amount = str(4500000+n)
    f('claac system delegatebw '+ voter +' ' + voter+''' "'''+amount + ''' AAC" "'''+amount + ''' AAC"''')
    n+=10000
    print("voter: "+voter+ " stake token ok!")
    time.sleep(0.02)


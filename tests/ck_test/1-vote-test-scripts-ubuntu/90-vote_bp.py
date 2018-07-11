#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

voterList = []
producerList = []
for line in open('acc_keys'):
    accName = line[:line.find(',')]
    if accName[0:2] == 'bp':
        producerList.append(accName)
    else:
        voterList.append(accName)
      
for i in range(0,49):  
   f('claac system voteproducer prods '+ voterList[i] + ' ' + producerList[i])
   print("voter: "+voterList[i]+ " vote "+ producerList[i] +" ok" )
   time.sleep(0.02)
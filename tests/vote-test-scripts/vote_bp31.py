#!/usr/bin/python

import os
import time

PATH = '~/Acute-Angle-Chain/build/programs/claac/'
PUSH_ACTION = PATH + 'claac push action '


voterList = []
producerList = []
for line in open('acc_keys'):
    accName = line[:line.find(',')]
    if accName[0:2] == 'bp':
        producerList.append(accName)
       
    else:
        voterList.append(accName)
       

for i in range(0,31):  
   os.system(PUSH_ACTION + '''aacio voteproducer '{"voter":"''' + voterList[i] + '''","proxy":"","producers":["''' + producerList[i] + '''"]}' --permission ''' + voterList[i] + '@active')
   
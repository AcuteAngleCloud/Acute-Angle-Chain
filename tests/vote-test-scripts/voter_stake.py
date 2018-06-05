#!/usr/bin/python

import os
import time

PATH = '~/AcuteAngleChain/build/programs/claac/'
PUSH_ACTION = PATH + 'claac push action '


voterList = []

for line in open('acc_keys'):
    accName = line[:line.find(',')]
    if accName[0:2] == 'vo':
        voterList.append(accName)

#all voter stake

n = 0
for voter in voterList:
    amount = str(100+n)+".0000 AAC"
    os.system(PUSH_ACTION + '''aacio delegatebw '{"from":"''' + voter + '''","receiver":"''' + voter + '''","stake_net":"''' + amount + '''","stake_cpu":"''' + amount + '''","stake_storage":"0.0000 AAC"}' --permission ''' + voter + '@active')
    n+=1
    time.sleep(0.1)

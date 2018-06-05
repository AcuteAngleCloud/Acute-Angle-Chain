#!/usr/bin/python

import os
import time

PATH = '~/AcuteAngleChain/build/programs/claac/'

accList = []
for line in open('acc_keys'):
    pos = line.find(',')
    accList.append(line[:pos])

max = len(accList)
for i in range(max):
    #os.system(PATH + '''claac push action currency issue '{"to":"''' + accList[i] + '''","quantity":"''' + str(10000 + i) + '''.0000 CUR","memo":""}' --permission currency@active''')
    print(accList[i])
    os.system(PATH + 'claac get currency balance aacio ' + accList[i])
    #print(PATH + 'claac get currency balance aacio ' + accList[i])
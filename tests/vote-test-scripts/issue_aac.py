#!/usr/bin/python

import os
import time

PATH = '~/AcuteAngleChain/build/programs/claac/'
PUSH_ACTION = PATH + 'claac push action '


accList = []

for line in open('acc_keys'):
    accName = line[:line.find(',')]
    accList.append(accName)


#issue AAC to all account
#n = 0
amount = "1000000.0000 AAC"
for account in accList:
    #amount = str(10000000.0000+n*1000.0000)+" AAC"
   
    os.system(PUSH_ACTION + '''aacio issue '{"to":"''' + account + '''","quantity":"'''+ amount +'''"}' --permission aacio@active''')
    #print(PUSH_ACTION + '''aacio issue '{"to":"''' + account + '''","quantity":"'''+ amount +'''"}' --permission aacio@active''')
    #n += 1
    time.sleep(0.1)
#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print


bpaccList = []

for line in open('bp_keys'):
    bpaccName = line[:line.find(',')]
    bpaccList.append(bpaccName)


#transfer AAC to bp accounts from aacio

amount = "20000.0000 AAC"
for bpaccount in bpaccList:
    f('claac transfer aacio '+ bpaccount +''' "'''+ amount+'''"''')
    time.sleep(0.02)



voaccList = []

for line1 in open('vo_keys'):
    voaccName = line1[:line1.find(',')]
    voaccList.append(voaccName)



# transfer AAC to voters account from aacio

amount = "20000000.0000 AAC"
for voaccount in voaccList:
    f('claac transfer aacio '+ voaccount +''' "'''+ amount+'''"''')
    time.sleep(0.02)
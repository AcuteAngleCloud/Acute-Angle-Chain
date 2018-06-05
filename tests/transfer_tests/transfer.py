#!/usr/bin/env python
import os
import time
from timeit import timeit
#import timeit

PATH = '~/Acute-Angle-Chain/build/programs/claac/'

accListfrom = []
for line in open('acc_from'):
    pos = line.find(',')
    accListfrom.append(line[:pos])

accListto = []
for line in open('acc_to'):
    pos = line.find(',')
    accListto.append(line[:pos])

max = len(accListfrom)

# transfer
def func():
  amount = 10000
  for j in range(1000):
     for i in range(max):
         os.system(PATH + 'claac transfer '+accListfrom[i]+' '+accListto[i]+' ' + str(amount))
         #time.sleep(0.001)
         #print(str(j)+' : '+str(i))
     amount += 10000
         
t = timeit('func()', 'from __main__ import func', number=1)

print('use time ='+str(t))



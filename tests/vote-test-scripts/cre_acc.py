#!/usr/bin/python

import os

PATH = '~/Acute-Angle-Chain/build/programs/claac/'

for line in open('acc_keys'):
    l = line.strip().split(',')
    os.system(PATH + 'claac create account aacio ' + l[0] + ' ' + l[2] + ' ' + l[4])
    #print(PATH + 'cleos create account aacio ' + l[0] + ' ' + l[2] + ' ' + l[4])



    




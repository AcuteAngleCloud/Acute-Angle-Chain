#!/usr/bin/python

import os

PATH = '~/Acute-Angle-Chain/build/programs/claac/'

for line in open('acc_keys'):
    l = line.split(',')
    os.system(PATH + 'claac wallet import ' + l[1])
    os.system(PATH + 'claac wallet import ' + l[3])

    




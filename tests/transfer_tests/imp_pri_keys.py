#!/usr/bin/python

import os

PATH = '~/AcuteAngleChain/build/programs/claac/'

for line in open('acc_from'):
    l = line.split(',')
    os.system(PATH + 'claac wallet import ' + l[1])
    os.system(PATH + 'claac wallet import ' + l[3])

    




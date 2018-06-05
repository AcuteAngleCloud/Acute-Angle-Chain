#!/usr/bin/python
import sys
import os

PATH = '~/AcuteAngleChain/build/programs/claac/'

for line in open('walt_pin'):
    l = line.strip().split(',')
    os.system(PATH + 'claac wallet unlock -n ' + l[0] + ' --password ' + l[1])

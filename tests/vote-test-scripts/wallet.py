#!/usr/bin/python
import sys
import os
import platform
strsystem = platform.system()
PATH = '~/Acute-Angle-Chain/build/programs/claac/'


if len(sys.argv) == 1:
    PATH = PATH + 'claac wallet create'
    waltname = 'default'
if len(sys.argv) == 2:
    PATH = PATH + 'claac wallet create -n '+ sys.argv[1]
    waltname = sys.argv[1]
for s in os.popen(PATH).readlines():
       line = s
line1 = line[1:-2]
waltpin = waltname + ',' + line1 +'\n' 
if strsystem == "Linux":    
    f = open('~/.local/share/aacio/nodaac/data/walt_pin', 'a')
else if strsystem == "Darwin":
    f = open('~/Library/Application\ Support/aacio/nodaac/data/walt_pin', 'a')
f.writelines(waltpin)
f.close()



print(strsystem)
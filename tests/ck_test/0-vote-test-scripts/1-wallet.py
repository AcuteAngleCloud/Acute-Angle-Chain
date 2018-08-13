#!/usr/bin/python3
import sys
import os

#command
CMD = ''
line = ''
waltname = ''
if len(sys.argv) == 1:
   CMD = 'claac wallet create'
   waltname = 'default'
if len(sys.argv) == 2:
   CMD = 'claac wallet create -n '+ sys.argv[1]
   waltname = sys.argv[1]
#print(CMD)

for s in os.popen(CMD).readlines():
   line = s
line1 = line[1:-2]
waltpin = waltname + ',' + line1 +'\n'
f = open('walt_pin','a')
f.writelines(waltpin)
f.close()


# import aacio keypair
os.system('claac wallet import --private-key 5KXyqBjYphm3LUu89Sd2SztXRahjr7sF8tprXF1ZrfhqLzxSkGf')

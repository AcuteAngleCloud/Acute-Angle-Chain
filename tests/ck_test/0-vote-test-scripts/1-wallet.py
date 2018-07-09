#!/usr/local/bin/python3
import sys
import os

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
os.system('claac wallet import 5KWg9t3fkjSsVavTMuMcS4H5jexieMpVjMPxKtD9i1X7pLpegnY')
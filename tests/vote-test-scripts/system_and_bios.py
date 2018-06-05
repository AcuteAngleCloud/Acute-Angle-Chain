#!/usr/bin/python
import sys
import os
import time

PATH = '~/AcuteAngleChain/build/programs/claac/'

os.system(PATH + 'claac set contract aacio ~/AcuteAngleChain/build/contracts/aacio.bios -p aacio')

time.sleep(0.1)

os.system(PATH + 'claac set contract aacio ~/AcuteAngleChain/build/contracts/aacio.system')

time.sleep(0.1)

os.system(PATH + '''claac push action aacio issue '{"to":"aacio","quantity":"100000000.0000 AAC"}' --permission aacio@active''')

time.sleep(0.1)

print("importing keys for currency...")
os.system(PATH + '''claac wallet import 5K3zNxatoLscz5FugqYBebHwyNnGBh3pNXLatUyrqSCqiKjC6N7''')
os.system(PATH + '''claac wallet import 5KiYUiUp9UkQv8tombsuJjUokx2DweuEHsUEMYwfjg3P8d5eVve''')

print("create account currency")
os.system(PATH + 'claac create account aacio currency AAC8gRM5BUu1DmggPXvF4EA6dPtqe86WpMEuUrUe2Xw7NXmB7YFec AAC6y2tCtteidJqhzk9PLiWhuH4P3qXZFHgeyNcdBz9CMTRttcLLS')

time.sleep(0.1)

print("set contract currency")
os.system(PATH + 'claac set contract currency ~/AcuteAngleChain/build/contracts/currency')

time.sleep(0.1)

print("Issuer CUR")
os.system(PATH + '''claac push action currency create '{"issuer":"currency","maximum_supply":"1000000.0000 CUR","can_freeze":0,"can_recall":0,"can_whitelist":0}' --permission currency@active''')



#!/usr/bin/python3

import os
import time

debug = 1
if debug == 1:
    f = os.system
else:
    f = print

FIRM = 'aacio'
CONTRACT_PATH = ' ~/1-aac/Acute-Angle-Chain/build/contracts/'
CONTRACT_MSIG = FIRM + '.msig'
CONTRACT_TOKEN = FIRM + '.token'

f('claac set contract ' + CONTRACT_TOKEN + CONTRACT_PATH + CONTRACT_TOKEN + '/')
f('claac set contract ' + CONTRACT_MSIG + CONTRACT_PATH + CONTRACT_MSIG + '/')
f('claac push action ' + CONTRACT_TOKEN + ''' create '["''' + FIRM + '''", "10000000000.0000 AAC"]' -p ''' + CONTRACT_TOKEN)
f('claac push action ' + CONTRACT_TOKEN + ''' issue '["''' + FIRM + '''", "1000000000.0000 AAC", "memo"]' -p ''' + FIRM)
time.sleep(0.5)
f('claac set contract ' + FIRM + CONTRACT_PATH + FIRM + '.system/')
time.sleep(0.5)
f('claac push action ' + FIRM + ''' setpriv '["''' + CONTRACT_MSIG + '''", 1]' -p ''' + FIRM + '@active')

f('claac push action ' + CONTRACT_TOKEN + ''' create '["''' + FIRM + '''", "10000000000.0000 SN"]' -p ''' + CONTRACT_TOKEN)
f('claac push action ' + CONTRACT_TOKEN + ''' issue '["''' + FIRM + '''", "1000000000.0000 SN", "memo"]' -p ''' + FIRM)

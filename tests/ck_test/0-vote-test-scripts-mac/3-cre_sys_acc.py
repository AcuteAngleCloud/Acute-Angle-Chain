#!/usr/local/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

FIRM = 'aacio'

systemAccounts = ['.bpay', '.msig', '.names', '.ram', '.ramfee', '.saving', '.stake', '.token', '.vpay']
for name in systemAccounts:
    f('claac create account ' + FIRM + ' ' + FIRM + name + ' AAC8VzFaGUEZBUXhEa6gkYhUgTifNfgx2XU9mFQbWz41MtTaZMyQD')
    time.sleep(0.01)
#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

FIRM = 'aacio'

systemAccounts = ['.bpay', '.msig', '.names', '.ram', '.ramfee', '.saving', '.stake', '.token', '.vpay', '.uvpay','.bchain']
for name in systemAccounts:
    f('claac create account ' + FIRM + ' ' + FIRM + name + ' AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD')
    time.sleep(0.1)

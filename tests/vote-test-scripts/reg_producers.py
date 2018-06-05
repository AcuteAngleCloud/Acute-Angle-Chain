#!/usr/bin/python

import os
import time

PATH = '~/AcuteAngleChain/build/programs/claac/'
PUSH_ACTION = PATH + 'claac push action '

prefs = '''","prefs":{"base_per_transaction_net_usage":102,"base_per_transaction_cpu_usage":102,"base_per_action_cpu_usage":102,"base_setcode_cpu_usage":102,"per_signature_cpu_usage":102,"per_lock_net_usage":102,"context_free_discount_cpu_usage_num":3,"context_free_discount_cpu_usage_den":102,"max_transaction_cpu_usage":1000002,"max_transaction_net_usage":1000002,"max_block_cpu_usage":10000002,"target_block_cpu_usage_pct":12,"max_block_net_usage":10000002,"target_block_net_usage_pct":12,"max_transaction_lifetime":3602,"max_transaction_exec_time":9902,"max_authority_depth":8,"max_inline_depth":6,"max_inline_action_size":4098,"max_generated_transaction_count":12,"percent_of_max_inflation_rate":52,"storage_reserve_ratio":102}}' --permission aacio@active '''

producerList = []
for line in open('acc_keys'): 
    accName = line[:line.find(',')]  
    if accName[0:2] == 'bp':
        producerList.append(accName)

keys = {}
for line in open('bp_keys'):
    pos = line.find(',')
    keys[line[0:pos]] = line[pos + 1:-1]


#register producer
for producer in producerList:
    os.system(PUSH_ACTION + '''aacio regproducer '{"producer":"''' + producer + '''","producer_key":"''' + keys[producer] + prefs + producer + '@active')
   #print(PUSH_ACTION + '''aacio regproducer '{"producer":"''' + producer + '''","producer_key":"''' + keys[producer] + prefs + producer + '@active')
    time.sleep(0.1)



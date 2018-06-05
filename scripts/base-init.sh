#!/bin/bash

./claac wallet create
sleep 1
./claac set contract aacio ../../contracts/aacio.bios -p aacio -j
sleep 1
./claac set contract aacio ../../contracts/aacio.system -j
sleep 1
./claac push action aacio issue '{"to":"aacio","quantity":"1000000000.0000 AAC"}' --permission aacio@active -j

sleep 1
./claac create account aacio currency AAC84snobCGtpQvTTfVaxvMuxGDd4p2LhtQhbWX1yWAU5yt4tiB55 AAC84snobCGtpQvTTfVaxvMuxGDd4p2LhtQhbWX1yWAU5yt4tiB55 -j


sleep 1
./claac set contract currency ../../contracts/currency -j
sleep 1

./claac push action currency create '{"issuer":"currency","maximum_supply":"1000000000.0000 CUR","can_freeze":0,"can_recall":0,"can_whitelist":0}' --permission currency@active -j
sleep 1
./claac push action currency issue '{"to":"currency","quantity":"10000000.0000 CUR","memo":""}' --permission currency@active -j
sleep 1
./claac push action currency transfer '{"from":"currency","to":"aacio","quantity":"20.0000 CUR","memo":"my first transfer"}' --permission currency@active -j
sleep 1

./claac transfer aacio currency 10000000000 -j
sleep 1
#./claac transfer currency aacio 100 -j
#sleep 1

./claac get currency balance aac aacio
sleep 1
./claac get currency balance aac currency


./claac push action aacio regproducer '{"producer":"currency","producer_key":"AAC84snobCGtpQvTTfVaxvMuxGDd4p2LhtQhbWX1yWAU5yt4tiB55","prefs":{"base_per_transaction_net_usage":102,"base_per_transaction_cpu_usage":102,"base_per_action_cpu_usage":102,"base_setcode_cpu_usage":102,"per_signature_cpu_usage":102,"per_lock_net_usage":102,"context_free_discount_cpu_usage_num":3,"context_free_discount_cpu_usage_den":102,"max_transaction_cpu_usage":1000002,"max_transaction_net_usage":1000002,"max_block_cpu_usage":10000002,"target_block_cpu_usage_pct":12,"max_block_net_usage":10000002,"target_block_net_usage_pct":12,"max_transaction_lifetime":3602,"max_transaction_exec_time":9902,"max_authority_depth":8,"max_inline_depth":6,"max_inline_action_size":4098,"max_generated_transaction_count":12,"percent_of_max_inflation_rate":52,"storage_reserve_ratio":102}}' --permission aacio@active -j
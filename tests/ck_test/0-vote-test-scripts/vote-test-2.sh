#!/bin/bash


echo -e "\033[32m ---------马上执行第1个脚本：打开并解锁钱包：1-wallet_unlock.py--------------------------- \033[0m" 
sleep 3
./2-wallet_unlock.py

echo -e "\033[32m ---------马上执行第2个脚本：创建系统账户：3-cre_sys_acc.py--------------------------------- \033[0m" 
sleep 3
./3-cre_sys_acc.py

echo -e "\033[32m ---------马上执行第3个脚本：部署系统合约并发行token：4-set_contracts_and_issue_token.py--------------------------------- \033[0m" 
sleep 3
./4-set_contracts_and_issue_token.py
 

echo -e "\033[32m -----------马上执行第4个脚本：创建49个prodcuer账户他49个voter账户 ：6-cre_acc.py------------------------------ \033[0m" 
sleep 3
./6-cre_acc.py


echo -e "\033[32m -----------马上执行第5个脚本：给新创建的98个账户发行EOS ： 7-issue_eos.py------------------------------ \033[0m" 
sleep 3
./7-issue_eos.py


echo -e "\033[32m ------------马上执行第6个脚本：49个投票账户进行投票前押注：8-voter_stake.py----------------------------- \033[0m" 
sleep 3
./8-voter_stake.py


echo -e "\033[32m ------------马上执行第7个脚本：49个producers进行注册：9-reg_producers.py--------------------------- \033[0m" 
sleep 3
./9-reg_producers.py


echo -e "\033[32m ------------马上执行第8个脚本：49个voters分别对49个producers进行一对一投票,每10秒投出一个：90-vote_bp.py------------------------------ \033[0m" 
sleep 3
./90-vote_bp.py


echo -e "\033[32m --------------脚本执行完毕，您可以观察producers 工作情况和打块顺序的变化------------- \033[0m" 

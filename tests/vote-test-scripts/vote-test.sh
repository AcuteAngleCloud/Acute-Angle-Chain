#!/bin/bash

echo -e "\033[32m ---------马上执行第1个脚本：创建钱包：start wallet.py--------------------------- \033[0m" 
sleep 3
./wallet.py

echo -e "\033[32m ---------马上执行第2个脚本：基础配置合约：start system_and_bios.py--------------------------- \033[0m" 
sleep 3
./system_and_bios.py
  
echo -e "\033[32m ----------马上执行第3个脚本：导入提前生成好的62对密钥：start imp_pri_keys.py--------------------------- \033[0m" 
sleep 3
./imp_pri_keys.py

echo -e "\033[32m ---------马上执行第4个脚本：创建 62个账户：start cre_acc.py--------------------------------- \033[0m" 
sleep 3
./cre_acc.py

echo -e "\033[32m -----------马上执行第5个脚本：给新创建的62个账户发行AAC ：start issue_aac.py------------------------------ \033[0m" 
sleep 3
./issue_aac.py

echo -e "\033[32m ------------马上执行第6个脚本：31个producers进行注册：start reg_producers.py--------------------------- \033[0m" 
sleep 3
./reg_producers.py

echo -e "\033[32m ------------马上执行第7个脚本：31个投票者(voters)进行投票前押注：start voter_stake.py----------------------------- \033[0m" 
sleep 3
./voter_stake.py

echo -e "\033[32m ------------马上执行第8个脚本：31个voters分别对23个producers进行一对一投票：start vote_bp31.py------------------------------ \033[0m" 
sleep 3
./vote_bp31.py


一、从AAC申请部署合约的账户(开发者账户)
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" aacio dkb.token AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD

二、部署蛋壳币合约
### claac set contract aacio ~/aac-mainet-test/aac/build/contracts/mycon1/
claac transfer aacio dkb.token  "10000000.0000 AAC"

三、创建币管理账户
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkb.token  dkbmangeracc AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD

claac transfer dkb.token  dkbmangeracc  "1000000.0000 AAC"

四、创建和发行蛋壳币
claac push action aacio.token create '["dkbmangeracc", "10000000000.0000 DKB"]' -p aacio.token
claac push action aacio.token issue '["dkbmangeracc", "1000000000.0000 DKB", "memo"]' -p dkbmangeracc

claac get account dkbmangeracc
claac get currency balance aacio.token dkbmangeracc

五、创建普通账户

claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkbmangeracc tangtangtang AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkbmangeracc caokunkevin1 AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkbmangeracc dkbuser11111 AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkbmangeracc dkbuser22222 AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac system newaccount --transfer --stake-net "100.0000 AAC" --stake-cpu "100.0000 AAC" --buy-ram "100.0000 AAC" dkbmangeracc dkbuser33333 AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD

六、蛋壳币转账
claac push action aacio.token transfer '["dkbmangeracc","tangtangtang","100000.0000 DKB","transfer"]' -p dkbmangeracc
claac push action aacio.token transfer '["dkbmangeracc","caokunkevin1","20000.0000 DKB","transfer"]' -p dkbmangeracc

claac push action  aacio.token transfer '["tangtangtang","dkbuser11111","1000.0000 DKB","transfer"]' -p tangtangtang
claac push action  aacio.token transfer '["tangtangtang","dkbuser22222","2000.0000 DKB","transfer"]' -p tangtangtang
claac push action  aacio.token transfer '["caokunkevin1","dkbuser33333","3000.0000 DKB","transfer"]' -p caokunkevin1

七、蛋壳币查余
claac get currency balance aacio.token dkbmangeracc
claac get currency balance aacio.token tangtangtang
claac get currency balance aacio.token caokunkevin1
claac get currency balance aacio.token dkbuser11111
claac get currency balance aacio.token dkbuser22222
claac get currency balance aacio.token dkbuser33333

八、账户状态
claac get account dkbmangeracc
claac get account tangtangtang
claac get account caokunkevin1
claac get account dkbuser11111
claac get account dkbuser22222
claac get account dkbuser33333
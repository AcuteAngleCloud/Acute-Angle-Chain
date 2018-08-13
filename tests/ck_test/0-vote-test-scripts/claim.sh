#create system accounts
claac create account aacio aacio.bpay AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.msig AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.names AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.ram AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.ramfee AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.saving AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.stake AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.token AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.vpay AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.uvpay AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
claac create account aacio aacio.bchain AAC6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD

#set token contract,and msig contract
claac set contract aacio.token ~/eos/build/contracts/aacio.token/
claac set contract aacio.msig ~/eos/build/contracts/aacio.msig/
#create and issue AAC token
claac push action aacio.token create '["aacio", "10000000000.0000 AAC"]' -p aacio.token
claac push action aacio.token issue '["aacio", "1000000000.0000 AAC", "memo"]' -p aacio
#get balance for aacio
claac get account aacio
claac get currency balance aacio.token aacio

sleep 0.5
#set system contract and 
claac set contract aacio ~/eos/build/contracts/aacio.system/
sleep 0.5

#set msig priv
claac push action aacio setpriv '["aacio.msig", 1]' -p aacio@active
#create and issue SN token
claac push action aacio.token create '["aacio", "10000000000.0000 SN"]' -p aacio.token
claac push action aacio.token issue '["aacio", "1000000000.0000 SN", "memo"]' -p aacio

#get balance for aacio
claac get account aacio
claac get currency balance aacio.token aacio

#create accounts
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio cktest1  AAC7tSY5od1Hu7hMvParp3VfWzXyhvhiCPZjq1XMDWZb7s84C1n1F
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio cktest2  AAC6cmDXn5mqxe9L5n1jB4TSfgZd8b2jbtNibe8EHrygjrmLf4oRN
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio cktest3  AAC7CRLbXJNsQvUCcGggAqoqbco8tydpMYZiPLv8A1v3FFV8rwauL
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio cktest4  AAC6oxALYnrJveHMTBYPq31oZ7Hwye1Pa1PvdYgJgCDkWm167GeAn
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio voter1  AAC6537XnYata5quZyhz71QgkSmZ7h9nSR5gw15jFJppZVBavZrKq
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio voter2  AAC7kKW1pwnoQVBakuHjRNm9yHo2ovsnswqnLjod8agbQycXTL2yP
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio voter3  AAC6CV1CV1sLrp5qSMFnzznybTYaE4rqQSNQcQziVDZcW36MDjxE3
claac system newaccount --transfer --stake-net "10.0000 AAC" --stake-cpu "10.0000 AAC" --buy-ram "10.0000 SN" aacio voter4  AAC6fXvaDopGHMAPqFdqDqjnyufTbLn7wAiBTFrAyPVsE5sSbz6LD


#get balance for aacio
claac get currency balance aacio.token cktest1
claac get currency balance aacio.token cktest2
claac get currency balance aacio.token cktest3
claac get currency balance aacio.token cktest4
claac get currency balance aacio.token voter1
claac get currency balance aacio.token voter2
claac get currency balance aacio.token voter3
claac get currency balance aacio.token voter4

#transfer tokens from aacio to test accounts
claac transfer aacio voter1 "100000000.0000 AAC"
claac transfer aacio voter2 "100000000.0000 AAC"
claac transfer aacio voter3 "100000000.0000 AAC"
claac transfer aacio voter4 "100000000.0000 AAC"
claac transfer aacio cktest1 "10000.0000 AAC"
claac transfer aacio cktest2 "10000.0000 AAC"
claac transfer aacio cktest3 "10000.0000 AAC"
claac transfer aacio cktest4 "10000.0000 AAC"

#get account test
claac get account aacio
claac get account cktest1
claac get account cktest2
claac get account cktest3
claac get account cktest4
claac get account voter1
claac get account voter2
claac get account voter3
claac get account voter4

#get balance
claac get currency balance aacio.token cktest1
claac get currency balance aacio.token cktest2
claac get currency balance aacio.token cktest3
claac get currency balance aacio.token cktest4
claac get currency balance aacio.token voter1
claac get currency balance aacio.token voter2
claac get currency balance aacio.token voter3
claac get currency balance aacio.token voter4

#regprocuer
claac system listproducers
claac system regproducer cktest1 AAC7tSY5od1Hu7hMvParp3VfWzXyhvhiCPZjq1XMDWZb7s84C1n1F
claac system regproducer cktest2 AAC6cmDXn5mqxe9L5n1jB4TSfgZd8b2jbtNibe8EHrygjrmLf4oRN
claac system regproducer cktest3 AAC7CRLbXJNsQvUCcGggAqoqbco8tydpMYZiPLv8A1v3FFV8rwauL
claac system regproducer cktest4 AAC6oxALYnrJveHMTBYPq31oZ7Hwye1Pa1PvdYgJgCDkWm167GeAn
claac system listproducers
#stake
claac system delegatebw voter1 voter1 '10000000 AAC' '10000000 AAC'
claac system delegatebw voter2 voter2 '20000000 AAC' '20000000 AAC'
claac system delegatebw voter3 voter3 '30000000 AAC' '30000000 AAC'
claac system delegatebw voter4 voter4 '40000000 AAC' '40000000 AAC'
claac get currency balance aacio.token cktest1
claac get currency balance aacio.token cktest2
claac get currency balance aacio.token cktest3
claac get currency balance aacio.token cktest4
claac get currency balance aacio.token voter1
claac get currency balance aacio.token voter2
claac get currency balance aacio.token voter3
claac get currency balance aacio.token voter4

#proxy voting
claac system voteproducer prods voter1 cktest1
claac system voteproducer prods voter2 cktest2
claac system voteproducer prods voter3 cktest3
claac system voteproducer prods voter4 cktest4

#select listproducers and listbw
claac system listproducers

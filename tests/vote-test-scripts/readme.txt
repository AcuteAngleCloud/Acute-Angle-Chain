测试Dawn3.0，主测投票功能
注意执行脚本前，把配置文件config.ini先行替换
注意执行脚本前，把数据和钱包删除
注意，执行脚本前，请先打开脚本文件查看路径配置，如路径不一致，请先修改路径
可以只执行批量处理脚本：vote-test.sh

也可以分步执行下面9个脚本,按照以下顺序
1、walt_and_bois.py  
创建钱包和系统智能合约并进行初始化配置

2、imp_pri_keys.py
导入acc_keys文件中的全部私钥

3、cre_acc.py
创建52个账户（26个producer,26个用来给producer投票的账户），账户和密钥信息在acc_keys文件中读取

4、issue_eos.py
发行EOS，给新创建的52个账户发放EOS，每个账户1000000.0000 AAC

5、reg_producers.py
为26个producer账户进行注册,账户信息在acc_keys文件读取和16进制公钥信息在bp_keys文件中读取

6、voter_stake.py
投票账户进行押注操作，账户信息在acc_keys文件中读取，押注金额vo.a到vo.z 26个投票账户的金额是（100.0000AAC---125.0000AAC）

7、vote_bp21.py
21个voter账户（vo.a...vo.u）为21个producer账户(bp.a...bp.u)进行一对一投票,投票成功后，producer打块顺序为bp.u...bp.a

8、vote_bp22-26.py
5个voter账户（vo.v...vo.z）为5个producer账户（bp.v...bp.z）进行一对一投票,投票成功后，bp.v...bp.z会把bp.a...bp.e挤掉，
producer的出块顺序为:bp.z,bp.y,bp.x,bp.w,bp.v,bp.u......bp.g,bp.f

9、vote_bp27-31.py 
5个producer账户（bp.1...bp.5）进行一对一投票,投票成功后，bp.1...bp.5会把bp.f...bp.j挤掉，
producer的出块顺序为:bp.5,bp.4...bp.z,bp.y...bp.k

10、config.ini文件放到配置文件应该放到的位置：
   macos ~/Library/Application Support/aacio/nodeos/config
   Linux:~/.local/share/aacio/noedeos/config
11、acc_keys文件是生成好的账户和密钥信息，31个producers,31个voters
12、bp_keys文件存放的是producers的账户的active公钥信息，16进制编码，regproducer操作使用。
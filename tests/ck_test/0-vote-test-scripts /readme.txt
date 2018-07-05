测试 aacio mainnet-1.0，主测投票功能
注意执行脚本前，把配置文件config.ini先行替换
注意执行脚本前，把数据和钱包删除
注意，执行脚本前，请先打开脚本文件查看路径配置，如路径不一致，请先修改路径
全新状态下可以只执行批量处理脚本：vote-test.sh
如果钱包在，只数据了清除，可以执行vote-test-2.sh
也可以分步执行下面11个脚本,按照以下顺序

1、0-deldata.sh
清楚链数据的脚本
./0-deldata .sh all   清除数据和钱包以及kill kaacd  并删除钱包pin文件
./0-deldata.sh wallet  只删除钱包和kill kaacd 并且删除钱包pin文件
./0-deldata.sh data 只删除数据

2、1-wallet.py
创建钱包，并保存pin到 walt_pin文件
不加参数创建default.wallet
加参数 xxxx,创建xxxx.wallet

3、2-wallet_unlock.py
打开所有本地钱包并解锁

4、3-cre_sys_acc.py
创建必须得系统账户

5、4-set_contracts_and_issue_token.py
创建系统合约并发行代币

6、5-imp_pri_keys.py
导入所有账户的私钥,在文件acc_keys中

7、6-cre_acc.py
创建acc_keys文件中得账户

8、7-issue_aac.py
给acc_keys中得账户转账 AAC 

9、8-voter_stake.py
投票的账户抵押token

10、9-reg_producers.py
矿工账户进行注册

11、90-vote_bp.py
49个voters账户对49个procucers账户进行一对一投票，10秒钟投出一票

12、config.ini文件放到配置文件应该放到的位置：
   macos ~/Library/Application Support/aacio/nodaac/config
   Linux:~/.local/share/aacio/noedaac/config

13、acc_keys文件是生成好的账户和密钥信息，49个producers,49个voters

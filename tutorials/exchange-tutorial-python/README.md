The following steps must be taken for the example script to work.

0. Create wallet
0. Create account for aacio.token
0. Create account for scott
0. Create account for exchange
0. Set token contract on aacio.token
0. Create AAC token
0. Issue initial tokens to scott

**Note**:
Deleting the `transactions.txt` file will prevent replay from working.


### Create wallet
`claac wallet create`

### Create account steps
`claac create key`

`claac create key`

`claac wallet import  --private-key <private key from step 1>`

`claac wallet import  --private-key <private key from step 2>`

`claac create account aacio <account_name> <public key from step 1> <public key from step 2>`

### Set contract steps
`claac set contract aacio.token /contracts/aacio.token -p aacio.token@active`

### Create AAC token steps
`claac push action aacio.token create '{"issuer": "aacio.token", "maximum_supply": "100000.0000 AAC", "can_freeze": 1, "can_recall": 1, "can_whitelist": 1}' -p aacio.token@active`

### Issue token steps
`claac push action aacio.token issue '{"to": "scott", "quantity": "900.0000 AAC", "memo": "testing"}' -p aacio.token@active`

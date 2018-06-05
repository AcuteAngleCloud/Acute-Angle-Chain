# AACIO Genesis

Tools used to initialize a genesis block for the AAC Platform

## Tools

- Snapshot Generator ([readme](https://github.com/AACIO/genesis/tree/master/tools/snapshot)) - Generates a snapshot from the AAC Crowdsale and Token contracts (saves a snapshot.csv and snapshot.json [metadata] into root directory of project)
- [Genesis Block Generator](https://aacio.github.io/genesis/) ([readme](https://github.com/AACIO/genesis/tree/master/tools/genesis)) - Enables configuration of a well-formed genesis.json file from a snapshot.csv. 
- [Account Name Lookup](https://aacio.github.io/genesis/tools/account-name/index.html) Look up the generated account name that was included in the genesis block with either your ETH address or AAC key
- AAC Genesis Key Utility ([readme](https://github.com/AACIO/genesis/tree/master/tools/keys)) - Offline AAC key gen and validator
- _Snapshot Frontend (planned)_

## Useful Files
- **snapshot.csv** Comma-delimited distribution list
- **snapshot.json** Meta data for the snapshot in the repo, describes the parameters of snapshot, some statistics and data for debugging

# Acute-Angle-Chain - The Most Powerful Infrastructure for Decentralized Applications


Welcome to the Acute-Angle-Chain source code repository! This software enables businesses to rapidly build and deploy high-performance and high-security blockchain-based applications.

Some of the groundbreaking features of Acute-Angle-Chain include:

1. Free Rate Limited Transactions 
1. Low Latency Block confirmation (1 seconds)
1. Low-overhead Byzantine Fault Tolerant Finality
1. Designed for optional high-overhead, low-latency BFT finality 
1. Smart contract platform powered by Web Assembly
1. Designed for Sparse Header Light Client Validation
1. Scheduled Recurring Transactions 
1. Time Delay Security
1. Hierarchical Role Based Permissions
1. Support for Biometric Hardware Secured Keys (e.g. Apple Secure Enclave)
1. Designed for Parallel Execution of Context Free Validation Logic
1. Designed for Inter Blockchain Communication 

Acute-Angle-Chain is released under the open source MIT license and is offered “AS IS” without warranty of any kind, express or implied. Any security provided by the Acute-Angle-Chain software depends in part on how it is used, configured, and deployed. Acute-Angle-Chain is built upon many third-party libraries such as Binaryen (Apache License) and WAVM  (BSD 3-clause) which are also provided “AS IS” without warranty of any kind. Without limiting the generality of the foregoing, Block.one makes no representation or guarantee that Acute-Angle-Chain or any third-party libraries will perform as intended or will be free of errors, bugs or faulty code. Both may fail in large or small ways that could completely or partially limit functionality or compromise computer systems. If you use or implement Acute-Angle-Chain, you do so at your own risk. In no event will Acute Angle cloud be liable to any party for any damages whatsoever, even if it had been advised of the possibility of damage.  

There is a public testnet running currently.

## Supported Operating Systems
Acute-Angle-Chain currently supports the following operating systems:  
1. Amazon 2017.09 and higher
2. Centos 7
3. Fedora 25 and higher (Fedora 27 recommended)
4. Mint 18
5. Ubuntu 16.04 (Ubuntu 16.10 recommended)
6. Ubuntu 18.04
7. MacOS Darwin 10.12 and higher (MacOS 10.13.x recommended)

# Resources
1. [Acute Angle Cloud website](http://www.acuteangle.com/)
2. [Acute Angle Cloud Blog](https://weibo.com/AcuteAngleCloud)
3. [Acute-Angle-Chain API Documentation](http://www.acuteangle.com/test/html/)
4. [White Paper](https://aacdn.3jiaoxing.com/doc/AcuteAngleCloud%2012.14_z_zh.pdf)


# Get code and build

## Getting the Code
To download all of the code, clone the Acute-Angle-Chain repository and its submodules.
```bash
git clone https://github.com/AcuteAngleCloud/Acute-Angle-Chain
```


## Building Acute-Angle-Chain
The simple approach to building Acute-Angle-Chain is to use the automated build script. The automated approach is documented next. Instructions to build Acute-Angle-Chain manually are described in the section Manually Building Acute-Angle-Chain, farther down in this document.

The build places content in the Acute-Angle-Chain/build folder. The executables can be found in subfolders within the Acute-Angle-Chain/build/programs folder.


#Automated build script
There is an automated build script that can install all dependencies and build Acute-Angle-Chain. The script supports the following operating systems. We are working on supporting other Linux/Unix distributions in future releases.

```
Amazon 2017.09 and higher
Centos 7
Fedora 25 and higher (Fedora 27 recommended)
Mint 18
Ubuntu 16.04 LTS (Ubuntu 16.10 recommended)
Ubuntu 18.04 LTS
MacOS Darwin 10.12 and higher (MacOS 10.13.x recommended)
Windows 10
```

#System Requirements (all platforms)
```
8GB RAM free required
20GB Disk free required
Run the build script
Run the build script from the Acute-Angle-Chain folder.
```

```bash
cd Acute-Angle-Chain
./aacio_build.sh
```

Install the executables
For ease of contract development, content can be installed in the /usr/local folder using the make install target. This step is run from the build folder. Adequate permission is required to install, so we use the sudo command with make install.

```bash
cd build
sudo make install
```

To run the node

```bash
cd build/programs/nodaac<br>
./nodaac
```
To run the command line

```bash
cd build/programs/claac
./claac get info
```
Refer to claac --help for detailed command-line instructions

```
Mac OS: ~/Library/Application Support/aacio/nodaac/config
Linux: ~/.local/share/aacio/nodaac/config
```
The build seeds this folder with a default genesis.json file. A configuration folder can be specified using the --config-dir command line argument to nodeos. If you use this option, you will need to manually copy a genesis.json file to your config folder.

nodaac will need a properly configured config.ini file in order to do meaningful work. On startup, nodeos looks in the config folder for config.ini. If one is not found, a default config.ini file is created. If you do not already have a config.ini file ready to use, run nodeos and then close it immediately with Ctrl-C. A default configuration (config.ini) will have been created in the config folder. Edit the config.ini file, adding/updating the following settings to the defaults already in place:
```
   # Load the testnet genesis state, which creates some initial block producers with the default key
   genesis-json = genesis.json
   # Enable production on a stale chain, since a single-node test chain is pretty much always stale
   enable-stale-production = true
   # Enable block production with the testnet producers
   producer-name = aacio
   # Load the block producer plugin, so you can produce blocks
   plugin = aacio::producer_plugin
   # Wallet plugin
   plugin = aacio::wallet_api_plugin
   # As well as API and HTTP plugins
   plugin = aacio::chain_api_plugin
   plugin = aacio::http_plugin
   # This will be used by the validation step below, to view account history
   plugin = aacio::account_history_api_plugin
```
Now it should be possible to run nodaac and see it begin producing blocks.

programs/nodaac/nodaac
nodeos stores runtime data (e.g., shared memory and log content) in a custom data folder. The location of this folder is determined by your system.

```
Mac OS: ~/Library/Application Support/aacio/nodaac/data
Linux: ~/.local/share/aacio/nodaac/data
```
A data folder can be specified using the --data-dir command line argument to nodaac.
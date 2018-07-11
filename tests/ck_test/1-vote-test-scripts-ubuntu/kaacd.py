#!/usr/local/bin/python3
import sys
import os
os.system('ps -ef | grep kaacd')
os.system('kill -9 pid')
os.system('kaacd --http-server-address=127.0.0.1:8900')

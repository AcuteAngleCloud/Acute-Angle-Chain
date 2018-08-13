#!/bin/bash

if [ $# == 0 ]; then
  echo "Lack of parameter \"all\" or \"data\" or \"wallet\""
  exit
fi  
if [ $1 == "all" ] || [ $1 == "data" ]; then
  ARCH=$( uname )
  if [ $ARCH == "Linux" ]; then
    rm -r ~/.local/share/aacio/nodaac/data/
  fi
  
  if [ $ARCH == "Darwin" ]; then
    rm -r ~/Library/Application\ Support/aacio/nodaac/data/
  fi
fi

if [ $1 == "all" ] || [ $1 == "wallet" ]; then
  rm -r ~/aacio-wallet
  rm walt_pin
  NAME=kaacd
  ID=`ps -ef | grep "$NAME" | grep -v "$0" | grep -v "grep" | awk '{print $2}'`
  echo $ID
  echo "---------------"
  for id in $ID
  do
    kill -9 $id
    echo "killed $id"
  done
  echo "---------------"
fi

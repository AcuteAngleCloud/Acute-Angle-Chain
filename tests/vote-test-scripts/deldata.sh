#!/bin/bash

  ARCH=$( uname )
  if [ $ARCH == "Linux" ]; then
  	rm -r ~/.local/share/aacio/nodaac/data/blocks
    rm -r ~/.local/share/aacio/nodaac/data/shared_mem
    if [ $1 == "wallet" ]; then
       rm -r ~/.local/share/aacio/nodaac/data
    fi
  fi
  
  if [ $ARCH == "Darwin" ]; then
     rm -r ~/Library/Application\ Support/aacio/nodaac/data/blocks
     rm -r ~/Library/Application\ Support/aacio/nodaac/data/shared_mem
     if [ $1 == "wallet" ]; then
       rm -r ~/Library/Application\ Support/aacio/nodaac/data
     fi
  fi
  
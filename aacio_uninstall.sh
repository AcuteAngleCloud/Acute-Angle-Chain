#! /bin/bash

binaries=(claac
          nodaac
          aacio-kaacd
          aacio-abigen
          aacio-applesdemo
          aacio-launcher
          aacio-s2wasm
          aacio-wast2wasm)

if [ -d "/usr/local/aacio" ]; then
   printf "\tDo you wish to remove this install? (requires sudo)\n"
   select yn in "Yes" "No"; do
      case $yn in
         [Yy]* )
            if [ "$(id -u)" -ne 0 ]; then
               printf "\n\tThis requires sudo, please run ./aacio_uninstall.sh with sudo\n\n"
               exit -1
            fi

            pushd /usr/local &> /dev/null
            rm -rf aacio
            pushd bin &> /dev/null
            for binary in ${binaries[@]}; do
               rm ${binary}
            done
            popd &> /dev/null
            break;;
         [Nn]* ) 
            printf "\tAborting uninstall\n\n"
            exit -1;;
      esac
   done
fi

#!/bin/bash

if [ ! -d emsdk ];then
    git clone https://github.com/emscripten-core/emsdk.git
fi

cd emsdk
# https://emscripten.org/docs/getting_started/downloads.html
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
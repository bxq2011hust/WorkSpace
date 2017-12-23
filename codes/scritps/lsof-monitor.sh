#!/bin/bash
set -e
ProgramName='verifier'
pid=$(ps -aux | grep ${ProgramName} | grep -v grep| awk '{print $2}')

echo "pid=${pid}"
nums=0
while :
do 
    if [ $nums == 0 ];then
        pid=$(ps -aux | grep ${ProgramName} | grep -v grep| awk '{print $2}')
    fi
    if [ -z "$pid" ];then
        echo "verifier isn't running."
    else
        nums=$(lsof -p ${pid} | wc -l) 
        echo "file descriptors: ${nums}"
    fi
    sleep 2
done

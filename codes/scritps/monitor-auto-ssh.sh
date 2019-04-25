#! /bin/bash

# check every 30 seconds
# crontab -e
# */1 * * * * bash /home/bxq/auto-ssh/auto-ssh.sh
# * * * * * sleep 30; bash /home/bxq/autossh/monitor-auto-ssh.sh

CHECK_STRING='$HOME/.ssh/auto'
PROCESS_PATH=$PWD
REMOTE_SERVER="123.207.14.190"
PORT=55664
REMOTE_USER="autossh"
REMOTE_USER_PRIVATE_KEY="$HOME/.ssh/autossh_rsa"
START_PROCESS="autossh -M 55665 -NR *:${PORT}:localhost:22 ${REMOTE_USER}@${REMOTE_SERVER} -i ${REMOTE_USER_PRIVATE_KEY}"

echo "command : ${START_PROCESS}"

proc_num()                      #查询进程数量
{
    num=`ps -ef | grep ${CHECK_STRING} | grep -v grep | wc -l`
    return $num
}

proc_num
number=$?
if [ $number -eq 0 ]            #如果进程数量为0
then                            #重新启动服务
    echo "Restarting ${CHECK_STRING} ..."
    echo $(date '+%Y-%m-%d %T') >> ${PROCESS_PATH}/restart.log
    (${START_PROCESS}) &
    echo "over"
else
    echo "autossh is already running."
fi
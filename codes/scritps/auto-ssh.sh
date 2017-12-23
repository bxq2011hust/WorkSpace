#! /bin/bash

# check every 30 seconds
# crontab -e
# */1 * * * * bash /home/bxq/auto-ssh/auto-ssh.sh
# * * * * * sleep 30; bash /home/bxq/autossh/monitor-auto-ssh.sh

PROCESS_NAME='autossh'
PROCESS_PATH=$PWD
RemoteServer="192.168.1.1"
START_PROCESS="autossh -M 55667 -NR *:55666:localhost:22 autossh@${RemoteServer} -i ${HOME}/.ssh/autossh_rsa"

proc_num()                      #查询进程数量
{
    num=`ps -ef | grep ${PROCESS_NAME} | grep -v grep | wc -l`
    return $num
}

proc_num
number=$?
if [ $number -eq 0 ]            #如果进程数量为0
then                            #重新启动服务
    echo "Restarting ${PROCESS_NAME} ..."
    echo $(date '+%Y-%m-%d %T') >> ${PROCESS_PATH}/restart.log
    (${START_PROCESS}) &
    echo "over"
fi
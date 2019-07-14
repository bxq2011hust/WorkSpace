#!/bin/bash

# This script download latest tars of FISCO-BCOS and console
# bash download_tar.sh output_path
# */20 * * * * bash /data/app/cdn/download_tar.sh -p /data/app/cdn/ -f > /data/app/cdn/download.log 2>&1
# */20 * * * * bash /data/app/cdn/download_tar.sh -p /data/app/cdn/ -c > /data/app/cdn/download.log 2>&1

output_dir=./
download_repo=FISCO-BCOS
timeout=250
console_timeout=1000

#set -e

failed_clean()
{
    local path=${1}
    echo "failed, clean ${path}"
    rm -rf ${path}
    exit 1
}

help() {
    echo $1
    cat << EOF
Usage:
    -p <Download Path>                  [Required] 
    -f <Download fisco-bcos>            Include fisco-bcos.tar.gz fisco-bcos-gm.tar.gz fisco-bcos-macOS.tar.gz
    -c <Download console>               Include console.tar.gz
    -h Help
e.g 
    $0 -p ~/data -f -c
EOF
exit 0
}

LOG_INFO()
{
    local content=${1}
    echo -e "\033[32m[INFO] ${content}\033[0m"
}

parse_params()
{
while getopts "p:cfwh" option;do
    case $option in
    p) output_dir=$OPTARG;;
    f) download_repo=('FISCO-BCOS');;
    c) download_repo+=('console');;
    h) help;;
    esac
done
}

download_fisco_artifacts()
{
    local version=${1}
    local from=https://github.com/FISCO-BCOS/FISCO-BCOS/releases/download/v${version}
    local to=${output_dir}/fisco-bcos/releases/download/v${version}
    mkdir -p ${to}
    local tars=(fisco-bcos.tar.gz fisco-bcos-gm.tar.gz fisco-bcos-macOS.tar.gz)
    for file in ${tars[*]}
    do
        curl -Lo ${to}/${file} ${from}/${file} -m ${timeout} || failed_clean ${to}
    done
}

download_fisco()
{
    local latest_version=$(curl -s https://api.github.com/repos/FISCO-BCOS/FISCO-BCOS/releases | grep "tag_name" | grep "\"v2\.[0-9]\.[0-9]\"" | sort -u | tail -n 1 | cut -d \" -f 4 | sed "s/^[vV]//")
    mkdir -p ${output_dir}/fisco-bcos/releases/download/
    local local_latest=$(ls -1 ${output_dir}/fisco-bcos/releases/download/ | sort -u | tail -n 1)
    if [ "v${latest_version}" != "${local_latest}" ];then
        LOG_INFO "latest fisco-bcos is v${latest_version}, downloading fisco-bcos v${latest_version} ..."
        download_fisco_artifacts ${latest_version}
    else
        LOG_INFO "latest fisco-bcos is v${latest_version}, local has ${output_dir}/fisco-bcos/releases/download/v${latest_version}"
    fi
}

download_console()
{
    local repo=FISCO-BCOS/console
    local latest_version=$(curl -s https://api.github.com/repos/${repo}/releases/latest | grep "tag_name" | sort -u | tail -n 1 | cut -d \" -f 4 | sed "s/^[vV]//")
    mkdir -p ${output_dir}/console/releases/download/
    local local_latest=$(ls -1 ${output_dir}/console/releases/download/ | sort -u | tail -n 1)
    local dist_dir=${output_dir}/console/releases/download/v${latest_version}
    if [ "v${latest_version}" != "${local_latest}" ];then
        LOG_INFO "latest console is v${latest_version}, downloading console v${latest_version} ..."
        mkdir -p ${dist_dir}
        curl -Lo ${dist_dir}/console.tar.gz https://github.com/${repo}/releases/download/v${latest_version}/console.tar.gz -m ${console_timeout} || failed_clean ${dist_dir}
    else
        LOG_INFO "latest console is v${latest_version}, local has ${dist_dir}"
    fi
}

main()
{
    for repo in ${download_repo[*]} 
    do
        case $repo in 
        FISCO-BCOS)
            download_fisco
            ;;
        console)
            download_console
            ;;
        *)
            echo "unknow repo"
            ;;
        esac
    done
}

echo "======== start at $(date +"%Y-%m-%d %H:%M:%S") ========"
parse_params $@
main
echo "======== exit at $(date +"%Y-%m-%d %H:%M:%S") ========"
#!/bin/bash

#echo "dataset directory"
#echo "./dataset|individual1|img1"
#echo "./dataset|individual1|..."

#./genImgList.sh mscelebv1_align_96_128

imgListDir=$PWD/imgList-$1
imgNameList=idList-$1.txt
datasetDir=$PWD/$1
echo "${datasetDir} ${imgListDir}"

if [ ! -d $datasetDir ]
then
    echo "Path ${datasetDir} doesn't exist"
    exit -1
else
    if [ ! -d $imgListDir ]
    then
        mkdir -p $imgListDir
        echo "Create ${imgListDir}"
    fi
    allDir=$(ls $datasetDir)
    echo "Processing..."
    for dir in $allDir
    do
        imgdir=$datasetDir/$dir
        if [ -d $imgdir ]
        then
           output=$imgListDir/$dir.txt
           touch "${output}"
           for img in $imgdir/*
           do
                echo "$img" >>$output
           done
           #ls $imgdir >$output
           echo "${output}" >>$imgNameList
        fi
    done
fi

exit 0

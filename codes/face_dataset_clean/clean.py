#!/usr/bin/env python
# -*- encoding: utf-8 -*-

# python clean.py disresult0.txt
import numpy as np
import os
import sys

if __name__ == '__main__':
    length = len(sys.argv)  
    if length != 2:  
        print '参数不正确'  
        sys.exit(0)    
    else:
        listPath = sys.argv[1]
        resultPath = './clean-result'

    if not os.path.exists(resultPath):
        os.makedirs(resultPath)
    else:
        print sys.argv
        print 'processing...'
    #imgList filename
    fname=open(listPath,'r')
    imgListPath=list(fname)
    fname.close()   
    
    fresult=open(resultPath+'/'+listPath,'w')
    for i in range(len(imgListPath)):
        disFile=imgListPath[i].strip()
        featMat=np.loadtxt(disFile)
        row_sum=featMat.sum(axis=1) #计算行和
        maxId = row_sum.argmax(axis=0)
        maxId_row = featMat[maxId,:]
        idName=disFile.split('/')[-1].split('-')[0]
        idPath='/home/bxq/dataset/mscelebv1_align_96_128-imgList/'+idName+'.txt'
        f=open(idPath,'r')
        imgNameList=list(f)
        f.close
        j=0
        for i in maxId_row:
            if i>0.66:
                fresult.write(imgNameList[j])
            j+=1
        print disFile+'\tdone'
    fresult.close()
    print '处理完成\n结果目录:'+resultPath


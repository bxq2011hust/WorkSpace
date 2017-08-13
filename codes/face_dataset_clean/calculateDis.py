#!/usr/bin/env python
# -*- encoding: utf-8 -*-

# python calculateDis.py feature-result10000.txt

import os
import sys
import numpy as np
from numpy import linalg as la


def cosSimilar(inA, inB):
    inA = np.mat(inA)
    inB = np.mat(inB)
    num = float(inA * inB.T)
    denom = la.norm(inA) * la.norm(inB)
    dis = 0.5 + 0.5 * (num / denom)
    return round(dis, 2)

if __name__ == '__main__':
    length = len(sys.argv)
    if length != 2:
        print '参数不正确'
        sys.exit(0)
    else:
        listPath = sys.argv[1]
        resultPath = './dis-result'

    if not os.path.exists(resultPath):
        os.makedirs(resultPath)
    else:
        print sys.argv
        print 'processing...'
    #imgList filename
    fname = open(listPath, 'r')
    imgListPath = list(fname)
    fname.close()
    # start calculate
    disResult = 'dis-' + listPath.split('-')[-1]
    fresult = open(disResult, 'w')
    for i in range(len(imgListPath)):
        imgClassSplit = imgListPath[i].split('/')[-1]
        resultFileName = imgClassSplit.split('-feat.txt')[0] + '-result.txt'
        resultFile = resultPath + '/' + resultFileName
        if(os.path.isfile(resultFile)):
            # os.remove(resultFile)
            print resultFile+'\tdone already'
            fresult.write(resultFile + '\n')
            continue
        featMat = np.loadtxt(imgListPath[i].strip())
        disMat = []
        rowNum = featMat.shape[0]
        if rowNum < 3:
            print '少于3张图'
            continue
        for j in range(rowNum):
            disMat.append([])
            feat_1 = featMat[j, :]
            for k in range(rowNum):
                if k <= j:
                    disMat[j].append(0)
                    continue
                feat_2 = featMat[k, :]
                dis = cosSimilar(feat_1, feat_2)
                disMat[j].append(dis)
        npdata = np.array(disMat)
        npdata += npdata.T
        npdata += np.eye(rowNum)
        np.savetxt(resultFile, npdata, fmt='%.2f')
        fresult.write(resultFile + '\n')
        print resultFile + '\tdone'
    fresult.close()
    print '处理完成\n结果目录:' + resultPath

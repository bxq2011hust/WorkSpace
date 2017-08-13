#!/usr/bin/env python
# -*- encoding: utf-8 -*-

# python getFeature.py ../dataset/mscelebv1_align_96_128-txtList.txt 40000 10000
# python getFeature.py ../dataset/mscelebv1_align_96_128-txtList.txt 90000
# 10000

caffe_root = '/home/txm/caffe/'
import os
import sys
sys.path.insert(0, caffe_root + 'python')
import caffe
import numpy as np
from numpy import linalg as la

net = []
transformer = []


def init(model_file, deploy_file, device):
    global net,  transformer
    net = caffe.Net(deploy_file, model_file, caffe.TEST)
    net.blobs['data'].reshape(1, 1, 128, 128)

    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2, 0, 1))
    # caffe.set_mode_cpu()
    caffe.set_mode_gpu()
    caffe.set_device(device)


def cosSimilar(inA, inB):
    inA = np.mat(inA)
    inB = np.mat(inB)
    num = float(inA * inB.T)
    denom = la.norm(inA) * la.norm(inB)
    dis = 0.5 + 0.5 * (num / denom)
    return round(dis, 2)


def getfeature(img):
    net.blobs['data'].data[...] = transformer.preprocess('data', img)
    net.forward()
    feature = net.blobs['eltwise_fc1'].data.copy()
    return feature


def verification(path1, path2):
    img1 = caffe.io.load_image(path1, False)
    img2 = caffe.io.load_image(path2, False)

    feat_1 = getfeature(img1)
    feat_2 = getfeature(img2)

    dis = cosSimilar(feat_1, feat_2)
    return dis

if __name__ == '__main__':
    length = len(sys.argv)
    if (length != 4):
        print '参数不正确'
        sys.exit(0)
    else:
        startNum = int(sys.argv[2])
        endNmu = startNum + int(sys.argv[3])
        dataName = sys.argv[1].split('/')[-1]
        dataName = dataName[:5] + '-clean'
        resultPath = dataName + '-feat'

    imgListPath = []
    fname = open(sys.argv[1], 'r')
    # imgList filename
    # for line in fname:
    #     line=line.strip()
    #     imgListPath.append(line)
    imgListPath = list(fname)
    fname.close()
    if startNum >= len(imgListPath):
        print '超出计算范围'
        print startNum, len(imgListPath)
        sys.exit(0)

    init('LightenedCNN_C.caffemodel', 'LightenedCNN_C_deploy.prototxt', 0)
    if not os.path.exists(resultPath):
        os.makedirs(resultPath)
    else:
        print sys.argv
        print 'get features...'
    # start calculate
    featFileList = 'feature-result' + str(startNum) + '.txt'
    fresult = open(featFileList, 'w')
    for i in range(startNum, endNmu):
        if i >= len(imgListPath):
            break
        imgClassSplit = imgListPath[i].split('/')[-1]
        resultFileName = imgClassSplit.split('.txt')[0] + '-feat.txt'
        resultFile = resultPath + '/' + resultFileName
        if(os.path.isfile(resultFile)):
            print resultFile + '\tdone already'
            fresult.write(resultFile + '\n')
            continue
        # get img feature
        f = open(imgListPath[i].strip(), 'r')
        imgList = list(f)
        f.close()
        if len(imgList) < 3:
            continue
        disMat = []
        for line in imgList:
            line = line.strip()
            img = caffe.io.load_image(line, False)
            imgFeature = getfeature(img)
            disMat.append(imgFeature[0].tolist())
        # save feature
        npdata = np.array(disMat)
        np.savetxt(resultFile, npdata, fmt='%.4f')
        fresult.write(resultFile + '\n')
        print resultFile + '\tdone'
    fresult.close()
    # fresList=open(dataName+'/txtlist.txt','a')
    # fresList.write(featFileList+'\n')
    # fresList.close()
    print '计算结束\n结果目录:' + resultPath

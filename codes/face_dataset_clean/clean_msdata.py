#!/usr/bin/env python
# -*- encoding: utf-8 -*-

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
    if (length < 2 or length > 3):
        print '参数不正确'
        sys.exit(0)
    elif length == 3:
        resultPath = sys.argv[2]
    else:
        resultPath = './result'

    init('LightenedCNN_C.caffemodel', 'LightenedCNN_C_deploy.prototxt', 0)
    if not os.path.exists(resultPath):
        os.makedirs(resultPath)
    else:
        print sys.argv
        print 'processing...'
    fname = open(sys.argv[1], 'r')
    imgListPath = []
    # imgList filename
    for line in fname:
        line = line.strip()
        imgListPath.append(line)
    fname.close()
    # start calculate
    fresult = open('dis-result.txt', 'w')
    for i in range(len(imgListPath)):
        f = open(imgListPath[i], 'r')
        imgClassSplit = imgListPath[i].split('/')[-1]
        resultFile = imgClassSplit.split('.txt')[0]
        resultFile = resultPath + '/' + resultFile + '-result.txt'
        if(os.path.isfile(resultFile)):
            # os.remove(resultFile)
            print resultFile + '\tdone already'
            fresult.write(resultFile + '\n')
            continue
        imgList = []
        # read img name
        for line in f:
            line = line.strip()
            imgList.append(line)
        f.close()
        disMat = []
        if len(imgList) < 10:
            continue
        for j in range(len(imgList)):
            disMat.append([])
            path1 = imgList[j]
            for k in range(len(imgList)):
                if k <= j:
                    disMat[j].append(0)
                    continue
                path2 = imgList[k]
                dis = verification(path1, path2)
                disMat[j].append(dis)
        npdata = np.array(disMat)
        npdata += npdata.T
        npdata += np.eye(len(imgList))
        np.savetxt(resultFile, npdata, fmt='%.2f')
        fresult.write(resultFile + '\n')
        print resultFile + '\tdone'
    fresult.close()
    print '结果目录:' + resultPath

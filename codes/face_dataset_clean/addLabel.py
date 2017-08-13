#!/usr/bin/env python
# -*- encoding: utf-8 -*-

# python addLabel.py mscelebv1.txt
import numpy as np
import os
import sys

if __name__ == '__main__':
    length = len(sys.argv)  
    if length != 2:  
        print '参数不正确'  
        sys.exit(0)    
    else:
        processPath = 'label-'+sys.argv[1]

    label_old=''
    i=-1
    j=-1
    sameClass=[]
    fresult=open(processPath,'w')
    fsrc=open(sys.argv[1],'r')
    for line in fsrc:
        line=line.strip()
        patch=line.split('/')
        label_new=patch[-2]
        imgname=patch[-2]+'/'+patch[-1]
        if label_new!=label_old:
            label_old=label_new  
            if j<10 and j>0:
                j=0
                sameClass=[]
                continue
            fresult.writelines(sameClass)
            i+=1
            j=0
            sameClass=[]
        j+=1
        sameClass.append(imgname+' '+str(i)+'\n')
        
    fsrc.close()
    fresult.close()
    print 'done!'
            



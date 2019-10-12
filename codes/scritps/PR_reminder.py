# !/bin/python
# -*- coding: utf-8 -*-

import requests
import json

input_data = {"vita-dounai": "catli(李陈希)", "wheatli": "wheatli(李辉忠)", "JimmyShi22": "jimmyshi(石翔)",
              "qwdarrenyin": "darrenyin(尹强文)", "bxq2011hust": "xingqiangbai(白兴强)", "ywy2090": "octopuswang(王章)",
              "fqliao": "caryliao(廖飞强)", "morebtcg": "ancelmo(莫楠)", "chaychen2005": "chaychen(陈宇)",
              "cyjseagull": "yujiechen(陈宇杰)", "HaoXuan40404": "asherli(李昊轩)", "qyan-dev": "qyan(严强)"}


# def getPRs(repo):
#     response = requests.get(
#         'https://api.github.com/repos/FISCO-BCOS/'+repo+'/pulls')
#     responseJson = response.json()
#     result = []
#     for pr in responseJson:
#         info = {}
#         info['user'] = pr['user']['login']
#         info['url'] = pr['url']
#         info['number'] = pr['number']
#         info['title'] = pr['title']
#         info['target'] = pr['base']['label']
#         info['created_at'] = pr['created_at']
#         info['statuses_url'] = pr['statuses_url']
#         requested_reviewers = []
#         for reviewer in pr['requested_reviewers']:
#             if input_data.get(reviewer['login']):
#                 requested_reviewers.append(input_data[reviewer['login']])
#             else:
#                 requested_reviewers.append(reviewer['login'])

#         info['requested_reviewers'] = requested_reviewers
#         result.append(info)
#     return result

# repos = ['FISCO-BCOS', 'web3sdk', 'FISCO-BCOS-DOC']
# res = {}
# for repo in repos:
#     res[repo] = getPRs(repo)
# return res

def getPRInfoString(repo):
    response = requests.get(
        'https://api.github.com/repos/FISCO-BCOS/'+repo+'/pulls')
    responseJson = response.json()
    title = u'### '+repo+'\n'
    i = 0
    resultMD = u''
    for pr in responseJson:
        i += 1
        resultMD += str(i)+'. @'
        if input_data.get(pr['user']['login']):
            resultMD += input_data[pr['user']['login']]
        else:
            resultMD += pr['user']['login']
        resultMD += u' 于' + pr['created_at']+u'，提的PR-'+str(pr['number']) + \
            u'，标题：'+pr['title']+u'，目标分支'+pr['base']['label']+u'，还没合入，请'
        # info['statuses_url'] = pr['statuses_url']
        for reviewer in pr['requested_reviewers']:
            name = ''
            if input_data.get(reviewer['login']):
                name = input_data[reviewer['login']]
            else:
                name = reviewer['login']
            resultMD += '@'+name+' '
        resultMD += u' 抽时间Review！[请点击这里](' + pr['html_url']+'/files)\n'
        print('[请点击这里](' + pr['html_url']+')\n')
    if not resultMD:
        return '', False
    return str(title+resultMD), True


repos = ['FISCO-BCOS', 'web3sdk', 'FISCO-BCOS-DOC',
         'console', 'python-sdk', 'nodejs-sdk', 'generator']
res = u''
for repo in repos:
    result, suc = getPRInfoString(repo)
    if suc:
        res += result
#res += str('\n 吃饭时间到啦，冲啊！')
# 'sendkey': '13484-9179826be256fef9db1615b86859da1e',
bot_url = 'https://sc.ftqq.com/SCU44120Tc6722bccf12e4993420c030db3d127a35c5a480c44225.send?text='+u'吃饭了&desp='+res
send = {'text': '吃饭提醒', 'desp': str(res)}
raw_data = 'text='+u'吃饭提醒&desp='+res
r = requests.post(bot_url, data=raw_data.encode(
    'utf-8'), headers={'Content-Type': 'application/x-www-form-urlencoded'})
# return send

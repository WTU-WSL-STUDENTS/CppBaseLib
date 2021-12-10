'''
Description: 
Version: 1.0
Autor: like
Date: 2021-12-08 15:02:05
LastEditors: like
LastEditTime: 2021-12-08 16:18:53
'''
#-*-coding:utf-8-*-
import os
import sys
import requests
import json
import pandas as pd
import  re
from bs4 import BeautifulSoup

def DownloadWether(cityCode, year, month):
    headers = {}
    headers['user-agent'] = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.108 Safari/537.36' #http头大小写不敏感
    headers['accept'] = '*/*'
    headers['Connection'] = 'keep-alive'
    headers['Pragma'] = 'no-cache'

    yearmonth = "%4s%2s"%(year, month)
    url = "http://tianqi.2345.com/t/wea_history/js/%s/%d_%s.js"%(yearmonth, cityCode, yearmonth)  
    res = requests.get(url)
    res.encoding = 'gb2312'
    a=res.text
    data=json.dumps(a, indent=2,ensure_ascii=False)

    print("Download Wether From >>>> " + url)
    b=a.split('[')
    c=b[1].replace('"','')
    f=re.findall(r'\{(.*?)\}', str(c))
    tianqi=[]
    print("---------------------------------------------------------")
    print('日期	最高气温	最低气温	天气	风向风力	空气质量指数')
    for i in f[:-1]:
        i={i.replace("'",'')}
        xx= re.sub("[A-Za-z\!\%\[\]\,\。]", " ", str(i))
        yy=xx.split(' ')
        tianqi.append([data[24:26], yy[3][1:], yy[10][1:-1], yy[17][1:-1], yy[24][1:], yy[34][1:],yy[41][1:], yy[45][1:],yy[53][1:]])    
        print(tianqi[len(tianqi) - 1])

    isExists=os.path.exists("..\\resources\\%d"%(cityCode))
    if not isExists:
        os.makedirs("..\\resources\\%d"%(cityCode)) 
    weather=pd.DataFrame(tianqi)
    weather.columns=['城市',"日期","最高气温","最低气温","天气","风向",'风力','空气质量指数','空气质量']
    weather.to_csv("..\\resources\\%d\\%s_%s.csv"%(cityCode, str(data[24:26]), yearmonth), encoding="utf_8_sig")
    return tianqi

if(3 == len(sys.argv)):
    DownloadWether(sys.argv[1], sys.argv[2])
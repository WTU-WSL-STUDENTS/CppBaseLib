'''
Description: 
Version: 1.0
Autor: like
Date: 2021-12-08 15:20:02
LastEditors: like
LastEditTime: 2021-12-08 17:33:42
'''
import os
import datetime
import downloadWether
import pandas as pd
import matplotlib.pyplot as plt

currentYear = datetime.datetime.now().year
recentYear  = 5
total = []

isExists=os.path.exists("..\\resources")
if not isExists:
    os.makedirs("..\\resources") 

# 对杨春湖景苑二期 风向及风力设定权值
windPower_2     = {"微风": 1, "1-2级": 2, "2-3级": 3, "3-4级": 4, "4-5级": 5, "5-6级": 6, "6-7级": 7}
windDirection_2 = {"南风": 1, "西南风": 2, "西风": 1}
total_2         = []
# 对杨春湖景苑二期 风向及风力设定权值
windPower_3     = {"微风": 1, "1-2级": 2, "2-3级": 3, "3-4级": 4, "4-5级": 5, "5-6级": 6, "6-7级": 7}
windDirection_3 = {"西北风": 1, "西风": 2, "西南风": 1}
total_3         = []

for year in range(currentYear - recentYear + 1, currentYear + 1):
    for month in range(1,13):
        list = downloadWether.DownloadWether(57494, year, str(month).rjust(2,'0'))#  57494  武汉, 71832 青山
        if(0 == len(list)):
            continue
        total.extend(list)

        for i in range(0, len(list)):
            # 对 二期 南风 & 西风 & 西南风  进行加权运算 
            if(list[i][5] in windDirection_2 and list[i][6] in windPower_2):
                kernel = windDirection_2[list[i][5]] * windPower_2[list[i][6]]
                total_2.append(kernel)
            else:
                total_2.append(0)
                
            # 对 三期 西北风 & 西风 & 西南风  进行加权运算 
            if(list[i][5] in windDirection_3 and list[i][6] in windPower_3):
                kernel = windDirection_3[list[i][5]] * windPower_3[list[i][6]]
                total_3.append(kernel)
            else:
                total_3.append(0)
weather=pd.DataFrame(total)
weather.columns=['城市',"日期","最高气温","最低气温","天气","风向",'风力','空气质量指数','空气质量']
weather.to_csv("..\\resources\\total_武汉.csv", encoding="utf_8_sig")

plt.bar(range(len(total_2)), total_2, width = 2, label = "2", fc = "r")
plt.bar(range(len(total_3)), total_3, width = 2, label = "3", fc = "b")
plt.xlabel("day flow")
plt.ylabel("result")
plt.title("wind power & direction influence")
plt.show()

os.system("pause")
import csv
from numpy import *
def fileread():
    file = open('trainset.csv','r')
    Lines = csv.reader(file)
    Features = []                    #特征向量初始化，逐行
    Lables = []     #标签
    for line in Lines:
        Features.append(line[0:5])
        Lables.append(line[-1])
    return Features,Lables

Mat,Lable=fileread()
for item in Lable:
    print(item)



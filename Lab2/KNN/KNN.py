import csv
from numpy import *
def fileread(filename):
    file = open(filename,'r')
    Lines = csv.reader(file)
    Features = []                   #特征向量初始化，逐行
    Lables = []     #标签
    i=0
    for line in Lines:
        if(i!=0):           #第一行是0，1，2，3，4，5，6，无用
            Features.append(line[0:6])
            Lables.append(line[-1])
        i = i + 1
    return array(Features),Lables

def Classfy(InputVector,DataSet,Lables,K):        #用于分类的输入向量,,TrainSet和K
    LineCount = DataSet.shape[0]     #行数
    Diff = tile(InputVector,(LineCount,1))-DataSet
    SQDiff = Diff**2
    SQsum = SQDiff.sum(axis=1)    #各行求平方和
    Distance = SQsum**0.5       #欧式距离，每行一个
    Indexs=Distance.argsort()       #argsort返回从小到大的索引值
    Count = {}      #存储lable数量的字典,待完善
    for i in range(K):        #选择k个最近邻
        lable = Lables[Indexs[i]]
        Count[lable]=Count[lable]+1



TrainData,TrainLables=fileread('trainset.csv')
TestData,TrainLables=fileread('testset.csv')
TestCount = TestData.shape[0]
fileread('trainset.csv')

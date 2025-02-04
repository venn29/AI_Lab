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
            for i in range (6):
                line[i]=ord(line[i])
                if(line[i]>=ord('0') and line[i]<=ord('9')):
                    line[i]=line[i]-ord('0')
                else:
                    line[i]=line[i]-ord('a')+10
            #Kingdis = max(abs(line[0]-line[4]),abs(line[1]-line[5]))
            #Cardis = 1
            #if(line[2]!=line[4] and line[3]!=line[5]):
             #   Cardis=2
            #else:  #不考虑三者在同一行并且白国王在中间的情况试试看
            #feact = []  # 降维以后的特征，分别是：白手国王行棋距离，车的行棋距离，
            #feact.append(Kingdis)
            #feact.append(Cardis)
            #feact.append(line[4])
            #feact.append(line[5])
            Features.append(line[0:6])
            #Features.append(feact)
            Lables.append(line[-1])
        i = i + 1
    return array(Features),Lables

def Classfy(InputVector,DataSet,Lables,K):        #用于分类的输入向量,,TrainSet和K
    #InputVector=IVector.astype(int)
    #DataSet=IDataSet.astype(int)
    LineCount = DataSet.shape[0]     #行数
    Diff = tile(InputVector,(LineCount,1))-DataSet      #提示可能类型不统一，但是明明是统一的，就很怪
    SQDiff = Diff**2
    SQsum = SQDiff.sum(axis=1)    #各行求平方和
    Distance = SQsum**0.5       #欧式距离，每行一个
    Indexs=Distance.argsort()       #argsort返回从小到大的索引值
    Count = {}      #存储lable数量的字典,待完善
    for i in range(K):        #选择k个最近邻
        lable = Lables[Indexs[i]]
        Count[lable]= Count.get(lable,0)+1
    sortedCount=sorted(Count.items(),key=lambda x:x[1],reverse=True)
    return sortedCount[0][0]



TrainData,TrainLables=fileread('trainset.csv')
TestData,TestLables=fileread('testset.csv')
TestCount = TestData.shape[0]
ResultLables=[]     #计算出的标签
error=0
for i in range(TestCount):
    ResultLables.append(Classfy(TestData[i],TrainData,TrainLables,7))
    if(ResultLables[i]!=TestLables[i]):
        error+=1
print(error)
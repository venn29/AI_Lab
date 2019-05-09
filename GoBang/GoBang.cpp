#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

enum ChessNodeType { None,L1, D1, L2, D2, L3, D3, L4, D4 };		//单向类型,
//活1，死1，活2，死2，活3，死3，活4，死4
//数字表示个数，死活表示终结点的类型，活为空，死为对方点
enum MultiType{None,Chl,H4,SC4,C4H3,SH3,H3M3,M4,H3,SH2,M3,H2M2,H2,M2};		//包括组合棋型在内的单点棋型
//长连,活4，双冲4，冲4活3，双活3，活3眠3，眠4，活3，双活2，眠3，活2眠2，活2，眠2，无
//单点棋型的定义：例如，单点棋形为活4：表示这个点是活4的一个连5点,单点棋形为双活三，表示在这个点下子以后就可以成为双活三

int PositionMap[15][15];		//每个位置的固定分，

int Point[10]=
{

};
//每种棋型得分，下标与Patter一一对应
string Patter[10]=
{

};
//模式串，也就是棋型,注意，AI的棋子都是1，玩家是2，同一种棋型，得分互为相反数，但是仍然应该写出


int evaluate(string s)		//对棋盘上的一行(4种角度)作为string输入，判断得分
{

}

class Map		//棋盘
{
public:
	Map()
	{

	}
	void Pick()
	{

	}
};

//问题，如何处理玩家？新建一个类？还是作为负数存储？

class AI		//AI的当前评分，以及各行情况
{
private:
	int score;		//分数
	int row[15];
	int col[15];
	int pos[21];
	int neg[21];		//横行，纵列，正45°，负45°的各个行的值,其中，斜线29行中有8行是0没有统计必要的
	int maxrow;
	int maxcol;
	int maxpos;
	int maxneg;		//各行的最大值
public:
	AI()
	{
		AIStatus = new StatusBoard;			//初始化，创建评估棋盘
		memset(row, 0, sizeof(int)*15);
		memset(col, 0, sizeof(int) * 15);
		memset(pos, 0, sizeof(int) * 21);
		memset(neg, 0, sizeof(int)*21);
		maxrow = maxcol = maxpos = maxneg = score = 0;
	}
	StatusBoard* AIStatus;
	int GetScore() { return score; }
	int Update(int r, int c)//原理同下，我只需要更新米字型上的4个方向,所以需要输入点对
	{

	}
};

//////////////////////////////////////
AI* myAI;		//AI是全局并且唯一的，可以方便调用
////////////////////////////////


class ChessNode		//棋盘上的点结构，主要是为了评估空位点的启发函数值，非空位点的分为-1
{
private:
	int r, c;
	int score;
	int flag[8];			//0表示旁边是空位，1表示旁边有棋子我方活棋子，2表示有我方死棋子个数，3表示紧邻敌方棋子
	int num[8];			//若flag为0，这个数值表示最近的我方棋子在这个方向上距离我多远，若为-n，表示最近的敌方棋子在这个方向上距离我多远	
						//若flag为1，这个数值表示紧邻的地方或者我方评估棋形(只考虑个数是不够的，需要考虑类型，是死的还是活的，很不一样,可以考虑把flag扩大为int，方便表示死活)
						//可以考虑用type表示空位，比如活四点：在这一点下了，就生成一个新活四,比较方便
	//八个方向顺序是：左右上下，左上，右下，左下，右上
	MultiType Mtype[8];			//八个方向上的单向类型
	ChessNodeType Ctype;		//本棋子的类型			
	int Target(int newr, int newc)		//方向判断
	{
		if (newr == r)		//同一行
		{
			if (newc < r)		//新点在左
				return 0;
			else
				return 1;
		}
		else if (newc == c)		//同一列、
		{
			if (newr < r)
				return 2;		//上
			else
				return 3;		//下 
		}
		else if (newr - r == newc - c)		//负45度线
		{
			if (newr < r)		//左上
				return 4;
			else
				return 5;
		}
		else		//正45度线
		{
			if (newr < r)
				return 6;
			else
				return 7;
		}
	}

	int Distance(int i, int newr, int newc)		//判断同方向上的距离
	{
		switch (i)
		{
		case 0:
		case 1:
			return abs(newr-r);
		case 2:
		case 3:
			return abs(newc-c);
		default:
			return abs(newr-r);		//斜线情况，
		}
	}

	void Update_DirectionType(int target,int player)	//更新单向棋形			//为了保证每一个空位点可以使用旁边空位点的棋形结果，主程序中需要从落子点向四周发散开来
	{
		int ry=r;
		int cx=c;
		int ChessLen=0;
		int End_type;		
		if(player==1)
		{	

			int deltac,deltar;		//每次的增量
			switch(target)
			{
				case 0:			//往左
					deltac=-1;
					deltar=0;
					break;
				case 1:		//往右
					deltac =1;
					deltar=0;
					break;
				case 2:		//往上
					deltac = 0;
					deltar=-1;
					break;
				case 3:		//往下
					deltac=0;
					deltar=1;
					break;
				case 4:		//左上
					deltac=-1;
					deltar=-1;
					break;
				case 5:		//右下
					deltac=1;
					deltar=1;
					break;
				case 6:		//左下
					deltac=-1;
					deltar=1;
					break;
				case 7:		//右上
					deltac=1;
					deltar=-1;
					break;
				default:
					deltac=20;
					deltar=20;
			}
			if(flag[target]==0)
				Mtype[target]= myAI->AIStatus->ACMap[r+deltar][c+deltac];
			else			//统计邻近连珠数量和终结点类型
			{
				while(Map[ry][cx]==1)
					++ChessLen;
				End_type=Map[ry][cx];
			}
			if(Map[ry][cx]==0)
			{
				switch (ChessLen)
				{
				case 1:
					Mtype[target]=L1;
					break;
				case 2:
					Mtype[target]=L2;
					break;
				case 3:
					Mtype[target]=L3;
					break;
				case 4:
					Mtype[target]=L4;
					break;
				default:
					Mtype[target]=None;
					break;
				}
			}
			else if(Map[ry][cx]==2)
			{
				
			}
			else
				printf("error in Update ChessNode Type");	
		}
	}
public:
	ChessNode(int newr,int newc)
	{
		memset(flag, 0, sizeof(bool) * 8);
		memset(Mtype, 0, sizeof(enum) * 8);
		memset(Ctype, 0, sizeof(enum) * 8);
		score = PositionMap[newr][newc];
		r = newr;
		c = newc;
		for(int i=0;i<8;i++)
		{
			num[i]=16;
		}
	}
	int GetScore() { return score; }
		//注意，我默认Update传进来的newr和newc和我的当前r,c的某个方向在同一条线上

	//可以返回新增的分
	int Update(int newr, int newc, int player)		//player为1，表示AI，为2表示玩家//更新以后，需要保留之前的这个节点的值，方便退出当前递归的时候的恢复，但是这个事情不应该由本类中的Update函数来做，需要外部调用自己控制
	{
		int Dis;
		if (player == 1)		//八元数组中，落一个子只更新一个方向的	//距离在5以外的点，和我当前点，并没有什么关系
		{
			int target = Target(newr, newc);		//得到更新方向
			Dis=Distance(i,newr,newc);
			if (flag[target] == 0)
			{
				if (Dis==1)		//判断距离，如果是1.更新
				{
					flag[target]=1;
					Update_DirectionType(target,player);
				}
				else		//新下的子仍然不紧邻，更新Num即可
				{	
					if(num[target]>Dis)
						num[target]=Dis;
					return 0;	
				}
			}
		}
	}

};

class StatusBoard		//评分表，对一方而言棋盘上空位点的评分，通过本类做启发式搜索
{
private:
	int Map[15][15];			//防守分和进攻分的总和
	ChessNode* ACMap[15][15];	//进攻分估值，统计自己方棋子的空位分数
	ChessNode* DCMap[15][15];	//防守分估值，统计对方棋子的空位分数
public:
	StatusBoard()		//初始化，估值由是否靠近中心确定
	{
		
	}
	void Update(int r, int c)		//为什么需要输入落下棋子来判定？因为我只需要更新米字上的值，其他值不会变
	{

	}
};

int Max(int i, int j)
{
	if (i > j)
		return i;
	else
		return j;
}


int main()
{
	//初始化位置得分表
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			PositionMap[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
		}
	}
	return 0;
}
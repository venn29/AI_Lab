#include"pch.h"
#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

int llimit, rlimit, ulimit, dlimit;		//四个方向的当前子的最边缘值

int Point[7] = { 1000000, 20000, 2000, 1000,  200,  100,  10 };
//                长连，  活4， 冲4， 活三，  眠3   活2    眠2或者活1

//长连7位，活4 5位
//这是为了区分近层绝杀和真正的胜利，近层绝杀就设为100000，6位
enum DirectionType { DNone,L1, D1, L2, D2, L3, D3, L4, D4 };		//单向类型,
//活1，死1，活2，死2，活3，死3，活4，死4
//数字表示个数，死活表示终结点的类型，活为空，死为对方点
enum MultiType{MNone,Chl,H4,SC4,C4H3,SH3,H3M3,M4,H3,C4,SH2,M3,H2M2,H2,M2,H1,M1};		//包括组合棋型在内的单点棋型
//长连,活4，双冲4，冲4活3，双活3，活3眠3，眠4，活3，冲4，双活2，眠3，活2眠2，活2，眠2，活1，眠1
//单点棋型的定义：例如，单点棋形为活4：表示这个点是活4的一个连5点,单点棋形为双活三，表示在这个点下子以后就可以成为双活三

int PositionMap[15][15];		//每个位置的固定分，

//棋盘string，可以加快判断速度，不用每次判断都要生成字符串(待写)
string Mapr[15];		//横向地图
string Mapc[15];		//纵向地图
string Mapneg[29];		//左上到右下，-45度
string Mappos[29];		//左下到右上，45度

//每一种棋型的模式串数量 
int PatterCount[7] = { 1,1,5,4,10,6,9 };
int Map[15][15];		//棋盘

string AIpatter[7][10] = {
	{"11111"},		//长连
	{"011110"},		//活4
	{"011112","211110","10111","11011","11101"},		//冲4
	{"011100","001110","010110","011010"},		//活3
	{"001112","211100","010112","211010","011012","210110","10011","11001","10101","2011102"},	//眠3	//眠3
	{"001100","000110","011000","001010","010100","010010"},		//活2
	{"000112","211000","001012","210100","010001","100010","2010102","2011002","2001102"}		//眠2
};

string Playerpatter[7][10] = {
	{"22222"},		//长连
	{"022220"},		//活4
	{"022221","122220","20222","22022","22202"},		//冲4
	{"022200","002220","020220","022020"},		//活3
	{"002221","122200","020221","122020","022021","120220","20022","22002","20202","1022201"},	//眠3	//眠3
	{"002200","000220","022000","002020","020200","020020"},		//活2
	{"000221","122000","002021","120200","020002","200020","1020201","1022001","1002201"}		//眠2
};
//模式串，也就是棋型,注意，AI的棋子都是1，玩家是2，同一种棋型，得分互为相反数，但是仍然应该写出

//评估函数好像有点问题///没有问题，但是综合的时候要考虑了，同一类型的不同行之间应该加起来才对
//同一行不叠加
int evaluate(int type, int rank, int player)		//对棋盘上的一行(4种角度)作为string输入，判断得分，类型0123,对应横，纵，pos,neg,  th表示序号,要从string数组的第几个去取,player表示是哪位玩家
{
	string substring;		//子串
	int i;
	bool found = false;
	string Text;
	switch (type)
	{
	case 0:
		Text = Mapr[rank];
		break;
	case 1:
		Text = Mapc[rank];
		break;
	case 2:
		Text = Mappos[rank];
		break;
	case 3:
		Text = Mapneg[rank];
		break;
	default:
		Text = " ";
	}
	if (player == 1)		//AI评分
	{
		for (i = 0; i < 7; i++)
		{
			for (int j = 0; j < PatterCount[i]; j++)
			{
				substring = AIpatter[i][j];
				if (Text.find(substring) != -1)
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
	}
	else				//玩家评分
	{
		for (i = 0; i < 7; i++)
		{
			for (int j = 0; j < PatterCount[i]; j++)
			{
				substring = Playerpatter[i][j];
				if (Text.find(substring) != -1)
				{
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
		if (i == 8)
			return 0;
		else
			return Point[i];

	}
}


//问题，如何处理玩家？新建一个类？还是作为负数存储？

class AI		//AI的当前评分，以及各行情况
{
private:
	int score;		//分数
	int AIrow[15];
	int AIcol[15];
	int AIpos[29];
	int AIneg[29];		//横行，纵列，正45°，负45°的各个行的值,其中，斜线29行中有8行是0没有统计必要的
	int AImaxrow;
	int AImaxcol;
	int AImaxpos;
	int AImaxneg;		//各行的最大值
	int AIcountTh;		//1000以上的行/列/pos/neg的数量
	int AIsum;			//总分，不同行之间应该相加
	int AIscore;	

	int Prow[15];
	int Pcol[15];
	int Ppos[29];
	int Pneg[29];		//横行，纵列，正45°，负45°的各个行的值,其中，斜线29行中有8行是0没有统计必要的
	int Pmaxrow;
	int Pmaxcol;
	int Pmaxpos;
	int Pmaxneg;		//各行的最大值
	int PcountTh;
	int Pscore;
	int Psum;		//总分
public:
	AI()
	{
		AIStatus = new StatusBoard;			//初始化，创建启发值评估棋盘
		memset(AIrow, 0, sizeof(int)*15);
		memset(AIcol, 0, sizeof(int) * 15);
		memset(AIpos, 0, sizeof(int) * 21);
		memset(AIneg, 0, sizeof(int)*21);
		AImaxrow = AImaxcol = AImaxpos = AImaxneg = score = 0;
		memset(Prow, 0, sizeof(int) * 15);
		memset(Pcol, 0, sizeof(int) * 15);
		memset(Ppos, 0, sizeof(int) * 21);
		memset(Pneg, 0, sizeof(int) * 21);
		Pmaxrow = Pmaxcol = Pmaxpos = Pmaxneg = score = 0;
		Pscore = AIscore = 0;
		Psum = AIsum = 0;
	}
	StatusBoard* AIStatus;
	int GetScore() { return score; }
	void Update(int r, int c)//原理同下，我只需要更新米字型上的4个方向,所以需要输入点对		前三个string数组都从0,0开始	正45度，rank=r+c ,负45度从左下角开始，func0表示落子，1表示取走
	{
		//单行信息更新
		int newpos, newneg;
		newpos = r + c;
		newneg = 14 - r + c;
		int AIrold, AIcold, AIpold, AInold, Prold, Pcold, Ppold, Pnold;		//原来的值要保存,为什么？如果是退出递归要删除点，需要判断count是不是需要减小，仅仅这一个功能
		AIrold = AIrow[r];
		AIcold = AIcol[c];
		AIpold = AIpos[newpos];
		AInold = AIneg[newneg];

		Prold = Prow[r];
		Pcold = Pcol[c];
		Ppold = Ppos[newpos];
		Pnold = Pneg[newneg];
		
		AIrow[r]=evaluate(0, r, 1);
		AIcol[c]=evaluate(1, c, 1);
		Prow[r]=evaluate(0, r, 2);
		Pcol[c]= evaluate(1, c, 2);
		if (newpos >= 4 && newpos<=25)
		{
			AIpos[newpos] = evaluate(2, newpos, 1);
			Ppos[newpos]=evaluate(2, newpos, 2);
		}
		if (newneg >= 4 && newneg <= 25 )
		{
			AIpos[newneg] = evaluate(3, newneg, 1);
			Ppos[newneg] = evaluate(3, newneg, 2);
		}

		////////分数信息有问题，没有判断落子类型，有可能落子减分

		/////落子和取子好像没区别
			//AI的分数信息更新
			if (AIrow[r] >= 1000 && AIrold<1000)
				AIcountTh++;
			if (AIcol[c] >= 1000 && AIcold < 1000)
				AIcountTh++;
			if (AIpos[newpos] >= 1000 && AIpold < 1000)
				AIcountTh++;
			if (AIneg[newneg] >= 1000 && AInold < 1000)
				AIcountTh++;
			if (AIrow[r] < 1000 && AIrold >= 1000)
				AIcountTh--;
			if (AIcol[c] < 1000 && AIcold >= 1000)
				AIcountTh--;
			if (AIpos[newpos] < 1000 && AIpold >= 1000)
				AIcountTh--;
			if (AIneg[newneg] < 1000 && AInold >= 1000)
				AIcountTh--;


			AImaxrow = Max(AImaxrow, AIrow[r]);
			AImaxcol = Max(AImaxcol, AIcol[c]);
			AImaxpos = Max(AImaxpos, AIpos[newpos]);
			AImaxneg = Max(AImaxneg, AIneg[newneg]);
			int AImax = 0;
			if (AImaxrow > AImax)
				AImax = AImaxrow;
			if (AImaxcol > AImax)
				AImax = AImaxcol;
			if (AImaxpos > AImax)
				AImax = AImaxpos;
			if (AImaxneg > AImax)
				AImax = AImaxneg;
			if (AImax >= 10000)
				AIscore = AImax;
			else if (AIcountTh >= 2)		//不存在单行活4以上的棋，但是有双活三以上，也就是1000以上的有两个以上，绝杀棋，下了就能赢，和活四同等力度
				AIscore = 10000;
			else							//普通情况，直接各行相加
			{
				AIsum += AIrow[r] - AIrold;
				AIsum += AIcol[c] - AIcold;
				AIsum += AIpos[newpos] - AIpold;
				AIsum += AIneg[newneg] - AInold;
			}

			//玩家的分数信息更新
			if (Prow[r] >= 1000 && Prold < 1000)
				PcountTh++;
			if (Pcol[c] >= 1000 && Pcold < 1000)
				PcountTh++;
			if (Ppos[newpos] >= 1000 && Ppold < 1000)
				PcountTh++;
			if (Pneg[newneg] >= 1000 && Pnold < 1000)
				PcountTh++;
			if (Prow[r] < 1000 && Prold >= 1000)
				PcountTh--;
			if (Pcol[c] < 1000 && Pcold >= 1000)
				PcountTh--;
			if (Ppos[newpos] < 1000 && Ppold >= 1000)
				PcountTh--;
			if (Pneg[newneg] < 1000 && Pnold >= 1000)
				PcountTh--;
			Pmaxrow = Max(Pmaxrow, Prow[r]);
			Pmaxcol = Max(Pmaxcol, Pcol[c]);
			Pmaxpos = Max(Pmaxpos, Ppos[newpos]);
			Pmaxneg = Max(Pmaxneg, Pneg[newneg]);
			int Pmax = 0;
			if (Pmaxrow > Pmax)
				Pmax = Pmaxrow;
			if (Pmaxcol > Pmax)
				Pmax = Pmaxcol;
			if (Pmaxpos > Pmax)
				Pmax = Pmaxpos;
			if (Pmaxneg > Pmax)
				Pmax = Pmaxneg;

			Psum += Prow[r] - Prold;
			Psum += Pcol[c] - Pcold;
			Psum += Ppos[newpos] - Ppold;
			Psum += Pneg[newneg] - Pnold;
			if (Pmax >= 10000)
				Pscore = Pmax;
			else if (PcountTh >= 2)		//不存在单行活4以上的棋，但是有双活三以上，也就是1000以上的有两个以上，绝杀棋，下了就能赢，和活四同等力度(还是要差一点的)
				Pscore = 10000;							
			else		//普通情况，直接各行相加
			{
				Pscore = Psum;
			}
			//总分:	
			if (AIscore == 1000000)
				score = AIscore;
			else if (Pscore == 1000000)
				score = Pscore;
			else
				score = AIscore - Pscore;
	}
};

//////////////////////////////////////
AI* myAI;		//AI是全局并且唯一的，可以方便调用
////////////////////////////////


class ChessNode		//棋盘上的点结构，主要是为了评估空位点的启发函数值，非空位点的分为-1
{
private:
	int NodeType;
	int r, c;
	int score;
	int flag[8];			//0表示旁边是空位，1表示旁边有棋子我方棋子,2表示紧邻敌方棋子
	int Pnum[8];			//相邻棋子数
						//若flag为1，这个数值表示紧邻的地方或者我方评估棋形(只考虑个数是不够的，需要考虑类型，是死的还是活的，很不一样,可以考虑把flag扩大为int，方便表示死活)
						//可以考虑用type表示空位，比如活四点：在这一点下了，就生成一个新活四,比较方便
	//八个方向顺序是：左右上下，左上，右下，左下，右上
	enum  DirectionType Dtype[8];			//八个方向上的单向类型
	enum MultiType LineType[4];			//四条线上的棋型
	int LinePoint[4];
	enum MultiType Ctype;		//本棋子的棋型			
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
		if (flag[target] != 1)
		{
			Dtype[target] = DNone;
			Pnum[target] = 0;
			return;
		}
		int ry=r;
		int cx=c;
		int ChessLen=0;
		int End_type;	
		int deltac, deltar;		//每次的增量
		switch (target)
		{
		case 0:			//往左
			deltac = -1;
			deltar = 0;
			break;
		case 1:		//往右
			deltac = 1;
			deltar = 0;
			break;
		case 2:		//往上
			deltac = 0;
			deltar = -1;
			break;
		case 3:		//往下
			deltac = 0;
			deltar = 1;
			break;
		case 4:		//左上
			deltac = -1;
			deltar = -1;
			break;
		case 5:		//右下
			deltac = 1;
			deltar = 1;
			break;
		case 6:		//左下
			deltac = -1;
			deltar = 1;
			break;
		case 7:		//右上
			deltac = 1;
			deltar = -1;
			break;
		default:
			deltac = 20;
			deltar = 20;
		}
		if(player==1)
		{	
			while(Map[ry][cx]==1)
				++ChessLen;
			End_type=Map[ry][cx];
			if(Map[ry][cx]==0)
			{
				switch (ChessLen)
				{
				case 1:
					Dtype[target]=L1;
					break;
				case 2:
					Dtype[target]=L2;
					break;
				case 3:
					Dtype[target]=L3;
					break;
				case 4:
					Dtype[target]=L4;
					break;
				default:
					Dtype[target]=DNone;
					break;
				}
			}
			else if(Map[ry][cx]==2)
			{
				switch (ChessLen)
				{
				case 1:
					Dtype[target] = D1;
					break;
				case 2:
					Dtype[target] = D2;
					break;
				case 3:
					Dtype[target] = D3;
					break;
				case 4:
					Dtype[target] = D4;
					break;
				default:
					Dtype[target] = DNone;
					break;
				}
			}
			else
				printf("error in Update ChessNode Type");	
		}
		else if (player == 2)
		{
			while (Map[ry][cx] == 2)
				++ChessLen;
			End_type = Map[ry][cx];
			if (Map[ry][cx] == 0)
			{
				switch (ChessLen)
				{
				case 1:
					Dtype[target] = L1;
					break;
				case 2:
					Dtype[target] = L2;
					break;
				case 3:
					Dtype[target] = L3;
					break;
				case 4:
					Dtype[target] = L4;
					break;
				default:
					Dtype[target] = DNone;
					break;
				}
			}
			else if (Map[ry][cx] == 1)
			{
				switch (ChessLen)
				{
				case 1:
					Dtype[target] = D1;
					break;
				case 2:
					Dtype[target] = D2;
					break;
				case 3:
					Dtype[target] = D3;
					break;
				case 4:
					Dtype[target] = D4;
					break;
				default:
					Dtype[target] = DNone;
					break;
				}
			}
			else
				printf("error in Update ChessNode Type");
		}
		Pnum[target] = ChessLen;
	}


	//在以下评估中，涉及到两个空点的都不判了
	void lineEvalue(int i,int j,int k)	//更新一条直线上的棋型，输入数组索引
	{	
		//是活4或者冲4的连五点，下了就能赢
		if (Dtype[i] == L4 || Dtype[j] == L4)//活四
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}
		if (  flag[i]==1&&flag[j]==1&&(Pnum[i]+Pnum[j]==4))		//冲4中间点
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}
		if(Dtype[i] == D4 || Dtype[j] == D4)		//冲4边缘点
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}

		//是活3点，下了可以形成活4
		if (Dtype[i] == L3&&flag[j]!=2 || Dtype[j] == L3 && flag[i] != 2)//活三边缘点
		{
			LineType[k] = H4;
			LinePoint[k] = LinePoint[1];		//活4的分
			return;
		}
		if ((Dtype[i] == L1 && Dtype[j] == L2) || (Dtype[i] == L2 && Dtype[j] == L1))	//活三中间点
		{
			LineType[k] = H4;
			LinePoint[k] = LinePoint[1];		//活4的分
			return;
		}


		//是眠3点，下了可以形成冲4
		if (Dtype[i] == D3 && flag[j] != 2 || Dtype[j] == D3 && flag[i] != 2)//眠三边缘点
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//冲4的分
			return;
		}
		if ((Dtype[i] == L1 && Dtype[j] == D2) || (Dtype[i] == D2 && Dtype[j] == L1))	//眠三中间点
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//冲4的分
			return;
		}
		if ((Dtype[i] == L2 && Dtype[j] == D1) || (Dtype[j] == L2 && Dtype[i] == D1))	//眠三中间点
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//冲4的分
			return;
		}
		if ((Dtype[i] == L3 && flag[j] == 2) || (Dtype[j] == L3 && flag[i] == 2))	//眠三边缘点  2011102
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//冲4的分
			return;
		}
		
		//是活2点，下了可以变活三
		if (Dtype[i] == L2 && flag[j] != 2 || Dtype[j] == L2 && flag[i] != 2)//活2边缘点
		{
			LineType[k] = H3;
			LinePoint[k] = LinePoint[3];		//活3的分
			return;
		}
		if (Dtype[i] ==L1 && Dtype[j] == L1 )//活2中间点
		{
			LineType[k] = H3;
			LinePoint[k] = LinePoint[3];		//活3的分
			return;
		}
		//剩下的就不估计了，对剪枝的意义有限
		LineType[k] = MNone;
		LinePoint[k] = 0;
		return;
	}
	
	void NodeEvalue()			//更新score的值
	{
		int i = 0;
		int k = 0;
		for (; k < 4; k++)
		{
			lineEvalue(i, i + 1,k);
			i = i + 2;
		}
		//判断组合:
		int count = 0;	//	专门计算在1000及其以上的值
		int max = 0;
		for (i = 0; i < 4; i++)
		{
			if (LinePoint[i] > max)
				max = LinePoint[i];
			if (LinePoint[i] >= 1000)
				count++;
		}
		if (count >= 2 && max < 10000)
			max = 10000;
		score = max + PositionMap[r][c];	//加上位置参量
	}
public:
	ChessNode(int newr,int newc,int Type)
	{
		memset(flag, 0, sizeof(bool) * 8);
		memset(Dtype, 0, sizeof(DirectionType) * 8);
		Ctype = MNone;
		score = PositionMap[newr][newc];
		r = newr;
		c = newc;
		for(int i=0;i<8;i++)
		{
			Pnum[i] = 0;
		}
		NodeType = Type;
	}
	int GetScore() { return score; }
		//注意，我默认Update传进来的newr和newc和我的当前r,c的某个方向在同一条线上

	//当新棋子落下时，调用本成员函数
	void Update(int newr, int newc, int player)		//player为1，表示AI，为2表示玩家//更新以后，需要保留之前的这个节点的值，方便退出当前递归的时候的恢复，但是这个事情不应该由本类中的Update函数来做，需要外部调用自己控制
													//	其实也不用保存，直接设置就可以了，不修改当前点的各种值，到时候直接拿掉然后更新
	{
		if (Map[newr][newc] != 0)			//此处有点，不再估值
			return;
		int Dis;
		int target = Target(newr, newc);		//得到更新方向
		Dis = Distance(target, newr, newc);
		if (player == NodeType)			//落子类型和我的类型相同  	//八元数组中，落一个子只更新一个方向的
		{
			if (flag[target] == 0)
			{
				if (Dis==1)		//判断距离，如果是1.更新
				{
					flag[target]=1;
					Update_DirectionType(target,player);
				}
				
			}
			else if(flag[target]==1)		//本来就紧邻我方棋子
			{
				if (Dis <= 5)	//距离在5以外的点，和我当前点，并没有什么关系
					Update_DirectionType(target, player);
			}
			if (flag[target] != 2&&Dis<=5)
				NodeEvalue();
		}
		else		//落子类型和自己相反
		{
			if (flag[target] == 0)
			{
				if (Dis == 1)		//判断距离，如果是1,更新，当前状态是被堵死的状态
				{
					flag[target] = 2;
					Update_DirectionType(target, player);
				}
			}
			else if (flag[target] == 1)		//本来有紧邻我方棋子，新落下的敌方棋子可能改变我当前点的棋型
			{
				if (Dis <= 5)
					Update_DirectionType(target, player);
			}
			//如果该方向本来就有紧邻我方的敌方棋子，那下子并不影响
			if(flag[target]!=2&&Dis<=5)
				NodeEvalue();
		}
		
	}

	//当退出递归，棋子被拿起来的时候，调用本成员函数，同样只更新一个方向
	void PickUp(int newr, int newc, int player)
	{
		int Dis;
		int target = Target(newr, newc);		//得到更新方向
		Dis = Distance(target, newr, newc);
		if (Dis == 1)		//判断距离，如果是1更新FLAG
			flag[target] = 0;
		if (Dis > 5)		//拿掉一个距离在5以外的子，没有关系
			return;
		Update_DirectionType(target, player);
		NodeEvalue();
	}
};

class SimpleChessNode		//总分
{
public:
	SimpleChessNode(int newr,int newc,int  newscore)
	{
		r = newr;
		c = newc;
		score = newscore;
	}
	void UpdateScore(int newscore)
	{
		score = newscore;
	}
	int r;
	int c;
	int score;
};

class StatusBoard		//评分表，对一方而言棋盘上空位点的评分，通过本类做启发式搜索
{

public:
	StatusBoard()		//初始化，估值由是否靠近中心确定
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				ACMap[i][j] = new ChessNode(i, j, 1);
				DCMap[i][j] = new ChessNode(i, j, 2);
				ScoreMap[i][j] = new SimpleChessNode(i,j,PositionMap[i][j]);
			}
		}
	}
	void Update(int r, int c,int player)		//为什么需要输入落下棋子来判定？因为我只需要更新米字上的值，其他值不会变
	{
		int i, j;
		i = r;
		int nAs;
		int nDs;
		for ( j = 0; j < 15; j++)		//横行更新
		{
			if (j == c)
				continue;
			ACMap[i][j]->Update(i, j, player);
			DCMap[i][j]->Update(i, j, player);
			nAs= ACMap[i][j]->GetScore();
			nDs=DCMap[i][j]->GetScore();
			ScoreMap[i][j]->UpdateScore(nAs + nDs);
		}
		j = c;
		for (i = 0; i < 15; i++)		//纵列更新
		{
			if (i == r)
				continue;
			ACMap[i][j]->Update(i, j, player);
			DCMap[i][j]->Update(i, j, player);
			nAs = ACMap[i][j]->GetScore();
			nDs = DCMap[i][j]->GetScore();
			ScoreMap[i][j]->UpdateScore(nAs + nDs);
		}
		//正45度更新
		if (r+c>14)		//下半区
		{
			int sumrc = r + c;
			i = 14;
			j = sumrc - i;
			for (; j <= 14;)
			{
				ACMap[i][j]->Update(i, j, player);
				DCMap[i][j]->Update(i, j, player);
				nAs = ACMap[i][j]->GetScore();
				nDs = DCMap[i][j]->GetScore();
				ScoreMap[i][j]->UpdateScore(nAs + nDs);
				i--;
				j++;
			}
		}
		else		//上半区
		{
			int sumrc = r + c;
			j = 0;
			i = sumrc - i;
			for (; i >= 0;)
			{
				ACMap[i][j]->Update(i, j, player);
				DCMap[i][j]->Update(i, j, player);
				nAs = ACMap[i][j]->GetScore();
				nDs = DCMap[i][j]->GetScore();
				ScoreMap[i][j]->UpdateScore(nAs + nDs);
				i--;
				j++;
			}
		}
		//负45度更新
		if (r > c)		//下半区
		{
			j = 0;
			i = r - c;
			for (; i <= 14; )
			{
				ACMap[i][j]->Update(i, j, player);
				DCMap[i][j]->Update(i, j, player);
				nAs = ACMap[i][j]->GetScore();
				nDs = DCMap[i][j]->GetScore();
				ScoreMap[i][j]->UpdateScore(nAs + nDs);
				i++;
				j++;
			}
		}
		else			//上半区
		{
			i = 0;
			j = c - r;
			for (; j <= 14;)
			{
				ACMap[i][j]->Update(i, j, player);
				DCMap[i][j]->Update(i, j, player);
				nAs = ACMap[i][j]->GetScore();
				nDs = DCMap[i][j]->GetScore();
				ScoreMap[i][j]->UpdateScore(nAs + nDs);
				i++;
				j++;
			}
		}
	}
	ChessNode* ACMap[15][15];	//进攻分估值，统计自己方棋子的空位分数
	ChessNode* DCMap[15][15];	//防守分估值，统计对方棋子的空位分数
	SimpleChessNode* ScoreMap[15][15];			//进攻分和防守分的和，对双方来说，这个总和都是一样的，你的进攻分就是我的防守分
	
};

int Max(int i, int j)
{
	if (i > j)
		return i;
	else
		return j;
}

void LimitUpdate(int r, int c)
{
	if (llimit == -1)
	{
		llimit = rlimit = c;
		ulimit = dlimit = r;
	}
	if (c < llimit)
		llimit = c;
	if (c > rlimit)
		rlimit = r;
	if (r < ulimit)
		ulimit = r;
	if (r > dlimit)
		dlimit = r;

}

void PrintBoard()		//打印棋盘
{
	printf(" ");
	for (int i = 0; i < 14; i++)
		cout << i;
	for (int i = 0; i < 14; i++)
	{
		printf("%c", 'A' + i);
		for (int j = 0; j < 14; j++)
		{
			if (Map[i][j] == 0)
				printf("0");
			else if (Map[i][j] == 1)		//AI的棋子
				cout << "+";
			else
				cout << "*";
		}
	}
}

bool compare(SimpleChessNode* a, SimpleChessNode *b)
{
	return a->score > b->score;
}

int BetaTree(int maxlength, int depth, int MaxBefore)
{
	int NodeCount;		//我这次需要估计的棋子的数量
	SimpleChessNode* Chess[225] = { NULL };	//最多225个指针，也不多		//简化的指针，只有分数和坐标值
	int LBoard, RBoard, UBoard, DBoard;
	if (llimit == 0)
		LBoard = 0;
	else
		LBoard = llimit - 1;
	if (rlimit == 0)
		RBoard = 0;
	else
		RBoard = rlimit - 1;
	if (ulimit == 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit == 0)
		DBoard = 0;
	else
		DBoard = dlimit - 1;
	NodeCount = (RBoard - LBoard)*(DBoard - UBoard);
	int i, j, k;
	i = UBoard;
	k = 0;
	for (; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			Chess[k] = myAI->AIStatus->ScoreMap[i][j];
			k++;
			NodeCount++;
		}
	}
	sort(Chess, Chess + NodeCount, compare);		//对待搜索的点，排序
	int newr, newc;
	int MinScore = 999999;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	int tempscore;			//每一个循环的子的分数
	for (int i = 0; i < NodeCount; i++)
	{
		//下子
		newr = Chess[i]->r;
		newc = Chess[i]->c;
		Map[newr][newc] = 1;
		myAI->Update(newr, newc);		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		tempscore = BetaTree(maxlength, depth, MinScore);
		if (tempscore < MinScore)
		{
			Pickr = newr;
			Pickc = newc;
			MinScore = tempscore;
		}
		//取回棋子
		Map[newr][newc] = 0;
		myAI->Update(newr, newc);		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		if (MinScore < MaxBefore)		//剪枝
			break;
	}
	return MinScore;
}

int AlphaTree(int maxlength,int depth,int MinBefore)		//最大点，有深度限制，当前深度		
{
	if (depth > maxlength)
		return  myAI->GetScore();
	int NodeCount;		//我这次需要估计的棋子的数量
	SimpleChessNode* Chess[225] = { NULL };	//最多225个指针，也不多		//简化的指针，只有分数和坐标值
	int LBoard, RBoard, UBoard, DBoard;
	if (llimit == 0)
		LBoard = 0;
	else
		LBoard = llimit - 1;
	if (rlimit == 0)
		RBoard = 0;
	else
		RBoard = rlimit - 1;
	if (ulimit == 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit == 0)
		DBoard = 0;
	else
		DBoard = dlimit - 1;
	NodeCount = (RBoard - LBoard)*(DBoard - UBoard);
	int i, j, k;
	i = UBoard;
	k = 0;
	for (; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			Chess[k] = myAI->AIStatus->ScoreMap[i][j];
			k++;
			NodeCount++;
		}
	}
	sort(Chess, Chess + NodeCount, compare);		//对待搜索的点，排序
	int newr, newc;
	int MaxScore = -999999;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	int tempscore;			//每一个循环的子的分数
	for (int i = 0; i < NodeCount; i++)
	{
		//下子
		newr = Chess[i]->r;
		newc = Chess[i]->c;
		Map[newr][newc] = 1;
		myAI->Update(newr, newc);		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		tempscore = BetaTree(maxlength, depth, MaxScore);
		if (tempscore > MaxScore)
		{
			Pickr = newr;
			Pickc = newc;
			MaxScore = tempscore;
		}
		//取回棋子
		Map[newr][newc] = 0;
		myAI->Update(newr, newc);		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		if (MaxScore > MinBefore)
			break;
	}
	if (depth == 0)		//下子
	{
		Map[newr][newc] = 1;
		myAI->Update(newr, newc);		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		myAI->GetScore();			//返回真实的评估值
	}
	return MaxScore;
}

int main()
{
	//初始化外围:
	llimit = -1;
	rlimit = -1;
	ulimit = -1;
	dlimit = -1;
	//初始化位置得分表
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			PositionMap[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
		}
	}
	int func;
	printf("请输入谁先行:\n1、您\n2、AI\n");
	scanf("%d", &func);
	if (func == 2)
	{
		Map[7][7] = 1;
		myAI->Update(7, 7);
		myAI->AIStatus->Update(7, 7, 1);
		LimitUpdate(7, 7);
				//直接下到中央
	}
	int win = 0;		//0为还没有出结果，2为玩家胜利，1为AI胜利
	char row;
	int col;
	int score;
	while (1)
	{
		PrintBoard();
		printf("\n请输入您要下的位置:字母 数字\n");
		scanf("%c %d", &row, col);
		//下子+更新状态+判断胜利条件
		Map[row][col] = 2;
		myAI->Update(row, col);
		myAI->AIStatus->Update(row, col, 2);
		score = myAI->GetScore();
		if (score == -1000000)
		{
			win = 2;
			break;
		}
		//搜索落子位置，因为返回两个坐标比较麻烦，所以直接在0层Max树下子
		score=AlphaTree(1, 0, -999999);
		//更新判断胜利条件
		if (score == 1000000)
		{
			win = 1;
			break;
		}
		
	}
	if (win == 0)
		printf("\n平局\n");
	else if (win == 1)
		printf("\n您获胜了\n");
	else
		printf("\nAI胜利\n");
	return 0;
}
#include"pch.h"
#include<iostream>
#include<cmath>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
using namespace std;
//limit只扩张，不减小
int llimit, rlimit, ulimit, dlimit;		//四个方向的当前子的最边缘值
/*
int Point[7] = { 1200000, 20000, 2000, 1000,  100,  100,  10 };
//                长连，  活4， 冲4， 活三，  眠3   活2    眠2或者活1

//长连7位，活4 5位
//这是为了区分近层绝杀和真正的胜利，近层绝杀就设为100000，6位
enum DirectionType { DNone, L1, D1, L2, D2, L3, D3, L4, D4 };		//单向类型,
//活1，死1，活2，死2，活3，死3，活4，死4
//数字表示个数，死活表示终结点的类型，活为空，死为对方点
enum MultiType { MNone, Chl, H4, SC4, C4H3, SH3, H3M3, M4, H3, C4, SH2, M3, H2M2, H2, M2, H1, M1 };		//包括组合棋型在内的单点棋型
//长连,活4，双冲4，冲4活3，双活3，活3眠3，眠4，活3，冲4，双活2，眠3，活2眠2，活2，眠2，活1，眠1
//单点棋型的定义：例如，单点棋形为活4：表示这个点是活4的一个连5点,单点棋形为双活三，表示在这个点下子以后就可以成为双活三

int PositionMap[15][15];		//每个位置的固定分，

//棋盘string，可以加快判断速度，不用每次判断都要生成字符串，正45度斜线从0，0开始，每一行从左边开始，每一个点的坐标为:[r+c][c],		负45度斜线[c-r+14][c]，从左下角开始，每一行从左边开始


//每一种棋型的模式串数量 
int PatterCount[7] = { 1,1,5,4,10,6,9 };


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
*/


//评估函数好像有点问题///没有问题，但是综合的时候要考虑了，同一类型的不同行之间应该加起来才对
//同一行不叠加



//问题，如何处理玩家？新建一个类？还是作为负数存储？
/*
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
			return abs(newr - r);
		case 2:
		case 3:
			return abs(newc - c);
		default:
			return abs(newr - r);		//斜线情况，
		}
	}

	void Update_DirectionType(int target, int player)	//更新单向棋形			//为了保证每一个空位点可以使用旁边空位点的棋形结果，主程序中需要从落子点向四周发散开来
	{
		if (flag[target] != 1)
		{
			Dtype[target] = DNone;
			Pnum[target] = 0;
			return;
		}
		int ry = r;
		int cx = c;
		int ChessLen = 0;
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
		if (player == 1)
		{
			while (Map[ry][cx] == 1)
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
			else if (Map[ry][cx] == 2)
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
	void lineEvalue(int i, int j, int k)	//更新一条直线上的棋型，输入数组索引
	{
		//是活4或者冲4的连五点，下了就能赢
		if (Dtype[i] == L4 || Dtype[j] == L4)//活四
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}
		if (flag[i] == 1 && flag[j] == 1 && (Pnum[i] + Pnum[j] == 4))		//冲4中间点
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}
		if (Dtype[i] == D4 || Dtype[j] == D4)		//冲4边缘点
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//最大值
			return;
		}

		//是活3点，下了可以形成活4
		if (Dtype[i] == L3 && flag[j] != 2 || Dtype[j] == L3 && flag[i] != 2)//活三边缘点
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
		if (Dtype[i] == L1 && Dtype[j] == L1)//活2中间点
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
			lineEvalue(i, i + 1, k);
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
	ChessNode(int newr, int newc, int Type)
	{
		memset(flag, 0, sizeof(bool) * 8);
		memset(Dtype, 0, sizeof(DirectionType) * 8);
		Ctype = MNone;
		score = PositionMap[newr][newc];
		r = newr;
		c = newc;
		for (int i = 0; i < 8; i++)
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
				if (Dis == 1)		//判断距离，如果是1.更新
				{
					flag[target] = 1;
					Update_DirectionType(target, player);
				}

			}
			else if (flag[target] == 1)		//本来就紧邻我方棋子
			{
				if (Dis <= 5)	//距离在5以外的点，和我当前点，并没有什么关系
					Update_DirectionType(target, player);
			}
			if (flag[target] != 2 && Dis <= 5)
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
			if (flag[target] != 2 && Dis <= 5)
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
	SimpleChessNode(int newr, int newc, int  newscore)
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
				ScoreMap[i][j] = new SimpleChessNode(i, j, PositionMap[i][j]);
			}
		}
	}
	void Update(int r, int c, int player)		//为什么需要输入落下棋子来判定？因为我只需要更新米字上的值，其他值不会变
	{
		int i, j;
		i = r;
		int nAs;
		int nDs;
		for (j = 0; j < 15; j++)		//横行更新
		{
			if (j == c)
				continue;
			ACMap[i][j]->Update(r, c, player);
			DCMap[i][j]->Update(r, c, player);
			nAs = ACMap[i][j]->GetScore();
			nDs = DCMap[i][j]->GetScore();
			ScoreMap[i][j]->UpdateScore(nAs + nDs);
		}
		j = c;
		for (i = 0; i < 15; i++)		//纵列更新
		{
			if (i == r)
				continue;
			ACMap[i][j]->Update(r, c, player);
			DCMap[i][j]->Update(r, c, player);
			nAs = ACMap[i][j]->GetScore();
			nDs = DCMap[i][j]->GetScore();
			ScoreMap[i][j]->UpdateScore(nAs + nDs);
		}
		//正45度更新
		if (r + c > 14)		//下半区
		{
			i = 14-r+c;
			j = 0;
			for (; j <= r+c-14;)
			{
				ACMap[i][j]->Update(r, c, player);
				DCMap[i][j]->Update(r, c, player);
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

};*/
/*
class AI		//AI的当前评分，以及各行情况
{
private:
	int score;		//分数
	
	int AIsum;		//总分
	int AIscore;

	
	int Psum;
	int Pscore;
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
			if (i == 7)
				return 0;
			else
				return Point[i];

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
			if (i == 7)
				return 0;
			else
				return Point[i];

		}
	}
public:
	AI()
	{
		AIStatus = new StatusBoard;			//初始化，创建启发值评估棋盘
		AIsum = AIscore = 0;
		Psum = Pscore = 0;
		score = 0;
	}
	StatusBoard* AIStatus;
	int GetScore() { return score; }
	void Update()//每一次重新匹配吧
	{
		int AIcount;	//1000以上的不同行/列/pos/neg的数量
		int onescore;
		AIcount = 0;
		AIsum = 0;
		for (int i = 0; i < 15; i++)
		{
			onescore = evaluate(0, i, 1);
			if (onescore >= 1000)
				AIcount++;
			AIsum += onescore;
		}
		for (int i = 0; i < 15; i++)
		{
			onescore = evaluate(1, i, 1);
			if (onescore >= 1000)
				AIcount++;
			AIsum += onescore;
		}
		for (int i = 4; i < 26; i++)
		{
			onescore = evaluate(2, i, 1);
			if (onescore >= 1000)
				AIcount++;
			AIsum += onescore;
		}
		for (int i = 4; i < 26; i++)
		{
			onescore = evaluate(3, i, 1);
			if (onescore >= 1000)
				AIcount++;
			AIsum += onescore;
		}
		Psum = 0;
		int Pcount = 0;
		for (int i = 0; i < 15; i++)
		{
			onescore = evaluate(0, i, 2);
			if (onescore >= 1000)
				Pcount++;
			Psum += onescore;
		}
		for (int i = 0; i < 15; i++)
		{
			onescore = evaluate(1, i, 2);
			if (onescore >= 1000)
				Pcount++;
			Psum += onescore;
		}
		for (int i = 4; i < 26; i++)
		{
			onescore = evaluate(2, i, 2);
			if (onescore >= 1000)
				Pcount++;
			Psum += onescore;
		}
		for (int i = 4; i < 26; i++)
		{
			onescore = evaluate(3, i, 2);
			if (onescore >= 1000)
				Pcount++;
			Psum += onescore;
		}
			//AI的分数信息更新
	
		if (AIcount >= 2&&AIsum<20000)		//不存在单行活4以上的棋，但是有双活三以上，也就是1000以上的有两个以上，绝杀棋，下了就能赢，和活四同等力度
			AIscore = 20000;
		else							//普通情况，直接各行相加
			AIscore = AIsum;
		if (Pcount >= 2 && Psum < 20000)		//不存在单行活4以上的棋，但是有双活三以上，也就是1000以上的有两个以上，绝杀棋，下了就能赢，和活四同等力度
			Pscore = 20000;
		else							//普通情况，直接各行相加
			Pscore = Psum;
		score = AIscore - Pscore;
		//玩家的分数信息更新
		
	}
};

//////////////////////////////////////
AI* myAI;		//AI是全局并且唯一的，可以方便调用
////////////////////////////////


*/
/*
char* Mapr[15];		//横向地图
char* Mapc[15];		//纵向地图
char* Mapneg[29];		//左上到右下，-45度
char* Mappos[29];		//左下到右上，45度


void UpdateMapstring(int r, int c, int player)		//更新4个用于判断的MapString数组的元素
{
	Mapr[r][c] = player + '0';
	Mapc[c][r] = player + '0';
	if (r + c <= 14)
		Mappos[r + c][c] = player + '0';
	else
		Mappos[r + c][14 - r] = player + '0';
	if (r >= c)
		Mapneg[c - r + 14][c] = player + '0';
	else
		Mapneg[c - r + 14][r] = player + '0';
}
*/

int Map[15][15];		//棋盘
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
		rlimit = c;
	if (r < ulimit)
		ulimit = r;
	if (r > dlimit)
		dlimit = r;

}

void LimitRollBack(int l, int r, int u, int d)
{
	llimit = l;
	rlimit = r;
	ulimit = u;
	dlimit = d;
}

void PrintBoard(int *a)		//打印棋盘
{
	system("cls");
	printf("  ");
	for (int i = 0; i < 15; i++)
		printf("%1d ", i);
	printf("\n");
	for (int i = 0; i < 15; i++)
	{
		printf("%c ", 'A' + i);
		for (int j = 0; j < 15; j++)
		{
			if (j < 9)
			{
				if (Map[i][j] == 0)
					printf("- ");
				else if (Map[i][j] == 1)		//AI的棋子
					cout << "+ ";
				else
					cout << "* ";

			}
			else
			{
				if (Map[i][j] == 0)
					printf("-  ");
				else if (Map[i][j] == 1)		//AI的棋子
					cout << "+  ";
				else
					cout << "*  ";
			}
		}
		cout << endl;
	}
	cout << "AI下在了: ";
	printf("%c\t", 'A' + a[0]);
	printf("%d", a[1]);
}
/*
bool compare(SimpleChessNode* a, SimpleChessNode *b)
{
	return a->score > b->score;
}*/
/*

int BetaTree(int maxlength, int depth, int MaxBefore)
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
	if (rlimit == 14)
		RBoard = 14;
	else
		RBoard = rlimit + 1;
	if (ulimit == 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit == 14)
		DBoard = 14;
	else
		DBoard = dlimit + 1;
	NodeCount = 0;
	int i, j;
	i = UBoard;
	for (; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			if (Map[i][j] != 0)
				continue;
			Chess[NodeCount] = myAI->AIStatus->ScoreMap[i][j];
			NodeCount++;
		}
	}
	sort(Chess, Chess + NodeCount, compare);		//对待搜索的点，排序
	int newr, newc;
	int MinScore = 999999;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	int tempscore;			//每一个循环的子的分数
	int lold, rold, uold, dold;
	lold= llimit;
	rold = rlimit;
	uold = ulimit;
	dold = dlimit;
	for (int i = 0; i < NodeCount; i++)
	{
		//下子
		newr = Chess[i]->r;
		newc = Chess[i]->c;
		Map[newr][newc] = 2;
		UpdateMapstring(newr, newc, 2);
		LimitUpdate(newr, newc);
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		tempscore = AlphaTree(maxlength, depth+1, MinScore);
		if (tempscore < MinScore)
		{
			Pickr = newr;
			Pickc = newc;
			MinScore = tempscore;
		}
		//取回棋子
		Map[newr][newc] = 0;
		UpdateMapstring(newr, newc, 0);
		myAI->AIStatus->Update(newr, newc, 2);		//更新启发函数的值
		LimitRollBack(lold, rold, uold, dold);
		if (MinScore < MaxBefore)		//剪枝
			break;
	}
	return MinScore;
}

int AlphaTree(int maxlength, int depth, int MinBefore)		//最大点，有深度限制，当前深度		
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
	if (rlimit == 14)
		RBoard = 14;
	else
		RBoard = rlimit + 1;
	if (ulimit == 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit == 14)
		DBoard = 14;
	else
		DBoard = dlimit + 1;
	NodeCount = 0;
	int i, j, k;
	i = UBoard;
	for (; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			if (Map[i][j] != 0)
				continue;
			Chess[NodeCount] = myAI->AIStatus->ScoreMap[i][j];
			NodeCount++;
		}
	}
	//sort(Chess, Chess + NodeCount, compare);		//对待搜索的点，排序
	int newr, newc;
	int MaxScore = -999999;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	int tempscore;			//每一个循环的子的分数
	int lold, rold, uold, dold;
	lold = llimit;
	rold = rlimit;
	uold = ulimit;
	dold = dlimit;
	for (int i = 0; i < NodeCount; i++)
	{
		//下子
		newr = Chess[i]->r;
		newc = Chess[i]->c;
		Map[newr][newc] = 1;
		UpdateMapstring(newr, newc, 1);
		LimitUpdate(newr, newc);
		myAI->Update();		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		tempscore = BetaTree(maxlength, depth+1, MaxScore);
		if (tempscore > MaxScore)
		{
			Pickr = newr;
			Pickc = newc;
			MaxScore = tempscore;
		}
		//取回棋子
		Map[newr][newc] = 0;
		UpdateMapstring(newr, newc, 0);
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		LimitRollBack(lold, rold, uold, dold);
		if (MaxScore > MinBefore)
			break;
	}
	if (depth == 0)		//下子
	{
		Map[Pickr][Pickc] = 1;
		UpdateMapstring(Pickr, Pickc, 1);
		myAI->Update();		//更新分数
		myAI->AIStatus->Update(newr, newc, 1);		//更新启发函数的值
		LimitUpdate(newr, newc);
		return myAI->GetScore();			//返回真实的评估值
		
	}
	return MaxScore;
}

*/

class StepOutPut
{
private:
	bool changeline;
public:
	StepOutPut()
	{
		changeline = false;
	}
	void Output(int r, int c)
	{
		ofstream fout("output.txt", ios::app);
		if (changeline)
			fout << "[" << r << "," << c << "]\n";
		else
			fout << "[" << r << "," << c << "]\t";
		fout.close();
		changeline = !changeline;
	}
};

StepOutPut* Myoutput;
int EachScore(bool h, int *a, int line, bool conti, int pos,int player)//标记头尾有没有被堵上 
{
	if (h)		//被堵上
	{
		if (!conti)				//5步以内被堵住，没有可能性了
			return 0;
		switch (line)		//5步以内没有被堵住
		{
		case 1:
			return 1;
		case 2:					//只有中间最多空一个的情况是眠2，隔得太远则无意义
		{
			if (a[1] == player || a[2] == player)
				return 5;
			else
				return 1;
		}
		case 3:
		{
			if (a[4] == player)			//如果3个中有一个在最后一格，那么无论如何都是眠2而不是眠3，下面的活三同理
				return 5;
			else
				return 200;
		}
		case 4:							//无论如何都是冲4
			return 2000;
		}
	}
	else							//左边没有堵住
	{
		if (!conti)			//五步以内被堵住了
		{
			switch (line)
			{
			case 1:
				return 1;
			case 2:
			{
				switch (pos)
				{
				case 2:
					return 5;	//眠2
				case 3:
					if (a[2] == player)
						return 5;		//眠2
					else
						return 100;		//活2
				case 4:
					if (a[3] == player)
						return 5;
					else
						return 100;
				default:
					return 0;
				}
			}
			case 3:
				switch (pos)
				{
				case 3:
					return 200;		//眠3
				case 4:
					if (a[3] == player)
						return 200;
					else
						return 1000;
				default:
					return 0;
				}
			case 4:
				return 2000;		//连续冲4
			default:
				return 0;
			}
		}
		switch (line)		//范围内没有被堵住
		{
		case 1:
			return 10;
		case 2:
			if (a[1] == player || a[2] == player)		//活2
				return 100;
			else							//距离太远，忽略
				return 5;
		case 3:
			if (a[4] == player)				//眠3
				return 200;
			else						//活3
				return 1000;
		case 4:
			if (a[4] == player)
				return 2000;
			else
				return 100000;
		}
	}
}



int Evaluate()
{
	int As=0;		//AI的分
	int Ps=0;			//玩家的分
	int a[5];
	bool h = false;  //头有没有被堵上，中间有没有对方的点
	int linecount;	//连续子数量
	bool conti;		//没有被敌方打断
	bool Awin=false;
	bool Pwin = false;
	int AHcount = 0;
	int PHcount = 0;		//统计活3以上的数量
	int pos;		//5元组中对面的棋子的位置(如果有的话)
	int temp;
	for (int i = 0; i < 15; i++)		
	{
		for (int j = 0; j < 15; j++)
		{	
			if (Map[i][j] == 0)		//遇到棋子才开始判断
				continue;
			
			else 	if(Map[i][j]==1)		//AI棋子的评分
			{
			//横向
			if (j + 4 < 15)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || Map[i][j - 1] != 0)		//左边被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i][j + k];
					if (a[k] == 1 && conti)
						linecount++;
					if (a[k] == 2)
					{
						conti = false;
						pos = k;
					}

				}
				if (linecount == 5)
				{
					Awin = true;
					break;
				}
				temp= EachScore(h, a, linecount, conti, pos, 1);
				if (temp >= 1000)
					AHcount ++ ;
				As += temp;
			}
			//纵向
			if (i + 4 < 15)
			{
				pos = 0;
				conti = true;
				linecount = 0;
				if (i - 1 < 0 || Map[i - 1][j] != 0)		//上边被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i + k][j];
					if (a[k] == 1 && conti)
						linecount++;
					if (a[k] == 2)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Awin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 1);
				if (temp >= 1000)
					AHcount++;
				As += temp;
			}
			//正45度
			if (i - 4 >= 0 && j + 4 < 15)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || i + 1 < 0 || Map[i + 1][j - 1] != 0)		//左下被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i - k][j + k];
					if (a[k] == 1 && conti)
						linecount++;
					if (a[k] == 2)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Awin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 1);
				if (temp >= 1000)
					AHcount++;
				As += temp;
			}
			//负45度
			if (i + 4 < 15 && j + 4 < 15)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || i - 1 < 0 || Map[i - 1][j - 1] != 0)		//左上被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i + k][j + k];
					if (a[k] == 1 && conti)
						linecount++;
					if (a[k] == 2)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Awin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 1);
				if (temp >= 1000)
					AHcount++;
				As += temp;
			}
			}
			else 				//(Map[i][j] == 2)				//玩家棋子的评分
			{
			//横向
			if (j + 4 <= 14)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || Map[i][j - 1] != 0)		//左边被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i][j + k];
					if (a[k] == 2 && conti)
						linecount++;
					if (a[k] == 1)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Pwin = true;
					break;
				}
				temp= EachScore(h, a, linecount, conti, pos, 2);
				if (temp >= 1000)
					PHcount++;
				Ps += temp;
			}
			//纵向
			if (i + 4 <= 14)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (i - 1 < 0 || Map[i - 1][j] != 0)		//上边被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i + k][j];
					if (a[k] == 2 && conti)
						linecount++;
					if (a[k] == 1)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Pwin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 2);
				if (temp >= 1000)
					PHcount++;
				Ps += temp;
			}
			//正45度
			if (i - 4 >= 0 && j + 4 <= 14)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || i + 1 < 0 || Map[i + 1][j - 1] != 0)		//左下被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i - k][j + k];
					if (a[k] == 2 && conti)
						linecount++;
					if (a[k] == 1)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Pwin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 2);
				if (temp >= 1000)
					PHcount++;
				Ps += temp;
			}
			//负45度
			if (i + 4 < 15 && j + 4 < 15)
			{
				h = false;
				pos = 0;
				conti = true;
				linecount = 0;
				if (j - 1 < 0 || i - 1 < 0 || Map[i - 1][j - 1] != 0)		//左上被堵上了
					h = true;
				for (int k = 0; k < 5; k++)
				{
					a[k] = Map[i + k][j + k];
					if (a[k] == 2 && conti)
						linecount++;
					if (a[k] == 1)
					{
						conti = false;
						pos = k;
					}
				}
				if (linecount == 5)
				{
					Pwin = true;
					break;
				}
				temp = EachScore(h, a, linecount, conti, pos, 2);
				if (temp >= 1000)
					PHcount++;
				Ps += temp;
			}
			}
		}
		if (Pwin)
		{
			return -1000000;		//6个0
		}
		if (Awin)
		{
			return 1000000;		//6个0
		}
	}
	if (AHcount >= 2 && As < 10000)		//有两个活3及其以上的点
		As += 10000;
	if (PHcount >= 2 && Ps < 10000)		//有两个活3及其以上的点
		Ps += 10000;
	return As - Ps;
}

int AlphaTree(int maxlength, int depth, int MinBefore,int *a);
int BetaTree(int maxlength, int depth, int MaxBefore)		//最大点，有深度限制，当前深度		
{
	if (depth >= maxlength)
	{
		return  Evaluate();
	}
	int LBoard, RBoard, UBoard, DBoard;
	if (llimit <= 0)
		LBoard = 0;
	else
		LBoard = llimit - 1;
	if (rlimit >= 14)
		RBoard = 14;
	else
		RBoard = rlimit + 1;
	if (ulimit <= 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit >= 14)
		DBoard = 14;
	else
		DBoard = dlimit + 1;
	int i, j, k;
	i = UBoard;
	int tempscore;			//每一个循环的子的分数
	int MinScore = 9999999;
	int lold, rold, uold, dold;
	lold = llimit;
	rold = rlimit;
	uold = ulimit;
	dold = dlimit;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	bool cut = false;
	for (i=UBoard; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			if (Map[i][j] != 0)
				continue;

			Map[i][j] = 2;
			if (Evaluate() == -1000000)
			{
				Pickr = i;
				Pickc = j;
				cut =true;
				Map[i][j] = 0;
				MinScore = -1000000;
				break;
			}
			LimitUpdate(i, j);
			tempscore = AlphaTree(maxlength, depth+1, MinScore,NULL);
			if (tempscore < MinScore)
			{
				MinScore = tempscore;
				Pickr = i;
				Pickc = j;
			}
			Map[i][j] = 0;
			LimitRollBack(lold, rold, uold, dold);
			if (MinScore < MaxBefore)
			{
				cut = true;
				break;
			}
		}
		if (cut)
			break;
	}

	return MinScore*(1-0.05*depth);
}

int AlphaTree(int maxlength, int depth, int MinBefore,int *a)		//最大点，有深度限制，当前深度		
{
	if (depth >= maxlength)
	{
		return  Evaluate();
	}
	int LBoard, RBoard, UBoard, DBoard;
	if (llimit <= 0)
		LBoard = 0;
	else
		LBoard = llimit - 1;
	if (rlimit >= 14)
		RBoard = 14;
	else
		RBoard = rlimit + 1;
	if (ulimit <= 0)
		UBoard = 0;
	else
		UBoard = ulimit - 1;
	if (dlimit >= 14)
		DBoard = 14;
	else
		DBoard = dlimit + 1;
	int i, j, k;
	i = UBoard;
	int tempscore;			//每一个循环的子的分数
	int MaxScore = -9999999;
	int lold, rold, uold, dold;
	lold = llimit;
	rold = rlimit;
	uold = ulimit;
	dold = dlimit;
	int Pickr, Pickc;		//在0层起作用，决定确实下哪一个子
	bool cut = false;
	if (depth == 0)
	{
		for (i=UBoard; i <= DBoard; i++)
		{
			for (j = LBoard; j <= RBoard; j++)
			{
				if (Map[i][j] != 0)
					continue;
				Map[i][j] = 1;
				if (Evaluate() == 1000000)
				{
					a[0] = i;
					a[1] = j;
					return 1000000;
				}
				Map[i][j] = 0;
				
			}
			
		}
	}
	
	for (i=UBoard; i <= DBoard; i++)
	{
		for (j = LBoard; j <= RBoard; j++)
		{
			if (Map[i][j] != 0)
				continue;
			Map[i][j] = 1;
			if (Evaluate() == 1000000)
			{
				Pickr = i;
				Pickc = j;
				cut = true;
				Map[i][j] = 0;
				MaxScore = 1000000;
				break;
			}
			LimitUpdate(i, j);

			tempscore = BetaTree(maxlength, depth , MaxScore);
			if (tempscore > MaxScore)
			{
				MaxScore = tempscore;
				Pickr = i;
				Pickc = j;
			}
			Map[i][j] = 0;
			LimitRollBack(lold, rold, uold, dold);
			if (MaxScore > MinBefore)
			{
				cut = true;
				break;
			}
		}
		if (cut)
			break;
	}

	if (depth == 0)		//下子
	{
		Map[Pickr][Pickc] = 1;
		LimitUpdate(Pickr,Pickc);
		a[0] = Pickr;
		a[1] = Pickc;
		return Evaluate();			//返回真实的评估值
	}
	return MaxScore*(1-depth*0.05);
}



int main()
{
	//初始化外围:
	llimit = -1;
	rlimit = -1;
	ulimit = -1;
	dlimit = -1;
	//初始化位置得分表以及Map
	/*
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			PositionMap[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
			Map[i][j] = 0;
		}
	}*/
	//初始化用于匹配的string数组
	/*
	for (int i = 0; i < 29; i++)
	{
		if (i < 15)
		{
			Mapr[i] = new char[15];
			Mapc[i] = new char[15];
			Mapr[i] = "000000000000000";
			Mapc[i] = "000000000000000";
			for (int j = 0; j <= i; j++)
			{
				Mappos[i].push_back('0');
				Mapneg[i].push_back('0');
			}
		}
		else			//下标在15以后，
		{	
			for (int j = 0; j < 29 - i; j++)
			{
				Mappos[i].push_back('0');
				Mapneg[i].push_back('0');
			}
		}
	}
	*/
	int a[2] = { 0 };
	int func;
	printf("请输入谁先行:\n1、您\n2、AI\n");
	cin >> func;
	ofstream fout("output.txt");
	if (func == 1)
		fout << "ME" << "\t" << "AI" << endl;
	else
		fout << "AI" << "\t" << "ME" << endl;
	fout.close();
	Myoutput = new StepOutPut;
	if (func == 2)
	{
		Map[7][7] = 1;
		LimitUpdate(7,7);
		a[0] = 7;
		a[1] = 7;
		//直接下到中央
		Myoutput->Output(7, 7);
	}
	
	int win = 0;		//0为还没有出结果，2为玩家胜利，1为AI胜利
	char Crow;
	int row;
	int col;
	int score;
	PrintBoard(a);
	while (1)
	{
		printf("\n请输入您要下的位置:字母(大写) 数字\n");
		cin >> Crow;
		cin >> col;
		row = Crow - 'A';
		if (row < 0 || row >= 15)
		{
			continue;
		}
		if (col < 0 || col >= 15)
		{
			continue;
		}
		if (Map[row][col] != 0)
		{
			continue;
		}
		//下子+更新状态+判断胜利条件
		Map[row][col] = 2;
		Myoutput->Output(row, col);
		LimitUpdate(row, col);
		//myAI->AIStatus->Update(row, col, 2);
		score = Evaluate();
		if (score == -1000000)
		{
			win = 2;
			break;
		}
		PrintBoard(a);
		//搜索落子位置，因为返回两个坐标比较麻烦，所以直接在0层Max树下子
		score = AlphaTree(2, 0, 999999,a);
		//更新判断胜利条件
		score = Evaluate();
		Myoutput->Output(a[0], a[1]);
		if (score == 1000000)
		{
			win = 1;
			break;
		}

		PrintBoard(a);
	}
	PrintBoard(a);
	ofstream fend("output.txt", ios::app);
	if (win == 2)
	{
		printf("您获胜了\n");
		fend << endl << "您获胜了" << endl;
		fend.close();
	}
	else
	{
		printf("AI胜利\n");
		fend << endl << "AI胜利" << endl;
		fend.close();
	}
	return 0;
}
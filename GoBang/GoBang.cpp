#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

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
	StatusBoard* AIStatus;
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
	int GetScore() { return score; }
	int Update(int r, int c)//原理同下，我只需要更新米字型上的4个方向,所以需要输入点对
	{

	}
};

class StatusBoard		//评分表，对一方而言棋盘上空位点的评分，通过本类做启发式搜索
{
private:
	int Map[15][15];
public:
	StatusBoard()		//初始化，估值由是否靠近中心确定
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Map[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
			}
		}
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
	return 0;
}
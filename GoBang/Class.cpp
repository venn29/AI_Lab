int Max(int i, int j)
{
	if (i > j)
		return i;
	else
		return j;
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

class AI		//AI的当前评分，以及各行情况
{
private:
	int score;		//分数
public:
	int GetScore() { return score; }

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
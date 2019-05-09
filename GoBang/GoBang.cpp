#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

enum ChessNodeType { L1, C1, L2, C2, L3, C3, L4, C4 };		//��������
//��1����1����2����2����3����3����4����4
enum MultiType{};		//��������������ڵĵ�������

int PositionMap[15][15];		//ÿ��λ�õĹ̶��֣�

int Point[10]=
{

};
//ÿ�����͵÷֣��±���Patterһһ��Ӧ
string Patter[10]=
{

};
//ģʽ����Ҳ��������,ע�⣬AI�����Ӷ���1�������2��ͬһ�����ͣ��÷ֻ�Ϊ�෴����������ȻӦ��д��


int evaluate(string s)		//�������ϵ�һ��(4�ֽǶ�)��Ϊstring���룬�жϵ÷�
{

}

class Map		//����
{
public:
	Map()
	{

	}
	void Pick()
	{

	}
};

//���⣬��δ�����ң��½�һ���ࣿ������Ϊ�����洢��

class AI		//AI�ĵ�ǰ���֣��Լ��������
{
private:
	int score;		//����
	int row[15];
	int col[15];
	int pos[21];
	int neg[21];		//���У����У���45�㣬��45��ĸ����е�ֵ,���У�б��29������8����0û��ͳ�Ʊ�Ҫ��
	int maxrow;
	int maxcol;
	int maxpos;
	int maxneg;		//���е����ֵ
	StatusBoard* AIStatus;
public:
	AI()
	{
		AIStatus = new StatusBoard;			//��ʼ����������������
		memset(row, 0, sizeof(int)*15);
		memset(col, 0, sizeof(int) * 15);
		memset(pos, 0, sizeof(int) * 21);
		memset(neg, 0, sizeof(int)*21);
		maxrow = maxcol = maxpos = maxneg = score = 0;
	}
	int GetScore() { return score; }
	int Update(int r, int c)//ԭ��ͬ�£���ֻ��Ҫ�����������ϵ�4������,������Ҫ������
	{

	}
};

class ChessNode		//�����ϵĵ�ṹ����Ҫ��Ϊ��������λ�����������ֵ���ǿ�λ��ķ�Ϊ-1
{
private:
	int r, c;
	int score;
	int flag[8];			//0��ʾ�Ա��ǿ�λ��1��ʾ�Ա��������ҷ������ӣ�2��ʾ���ҷ������Ӹ�����3��ʾ���ڵз�����
	int num[8];			//��flagΪ0�������ֵ��ʾ������ҷ���������������Ͼ����Ҷ�Զ����Ϊ-n����ʾ����ĵз���������������Ͼ����Ҷ�Զ	
						//��flagΪ1�������ֵ��ʾ���ڵĵط������ҷ���������(ֻ���Ǹ����ǲ����ģ���Ҫ�������ͣ������Ļ��ǻ�ģ��ܲ�һ��,���Կ��ǰ�flag����Ϊint�������ʾ����)
						//���Կ�����type��ʾ��λ��������ĵ㣺����һ�����ˣ�������һ���»���,�ȽϷ���
	//�˸�����˳���ǣ��������ϣ����ϣ����£����£�����
	MultiType Mtype[8];			//�˸������ϵĵ�������
	ChessNodeType Ctype[8];			
	int Target(int newr, int newc)		//�����ж�
	{
		if (newr == r)		//ͬһ��
		{
			if (newc < r)		//�µ�����
				return 0;
			else
				return 1;
		}
		else if (newc == c)		//ͬһ�С�
		{
			if (newr < r)
				return 2;		//��
			else
				return 3;		//�� 
		}
		else if (newr - r == newc - c)		//��45����
		{
			if (newr < r)		//����
				return 4;
			else
				return 5;
		}
		else		//��45����
		{
			if (newr < r)
				return 6;
			else
				return 7;
		}
	}

	int Distance(int i, int newr, int newc)		//�ж�ͬ�����ϵľ���
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
			return abs(newr-r);		//б�������
		}
	}
public:
	ChessNode(int newr,int newc)
	{
		memset(flag, 0, sizeof(bool) * 8);
		memset(num, 0, sizeof(int) * 8);
		score = PositionMap[newr][newc];
		r = newr;
		c = newc;
	}
	int GetScore() { return score; }
		//ע�⣬��Ĭ��Update��������newr��newc���ҵĵ�ǰr,c��ĳ��������ͬһ������

	//���Է��������ķ�
	int Update(int newr, int newc, int player)		//playerΪ1����ʾAI��Ϊ2��ʾ���//�����Ժ���Ҫ����֮ǰ������ڵ��ֵ�������˳���ǰ�ݹ��ʱ��Ļָ�������������鲻Ӧ���ɱ����е�Update������������Ҫ�ⲿ�����Լ�����
	{
		if (player == 1)		//��Ԫ�����У���һ����ֻ����һ�������
		{
			int target = Target(newr, newc);		//�õ����·���
			if (flag[target] == 0)
			{
				if (Distance(i,newr,newc)==1)		//�жϾ��룬�����1.����
				{
					
				}
				else		//���µ�����Ȼ�����ڣ�����Num����
				{	
					
				}
			}
		}
	}

};

class StatusBoard		//���ֱ���һ�����������Ͽ�λ������֣�ͨ������������ʽ����
{
private:
	int Map[15][15];			//���طֺͽ����ֵ��ܺ�
	ChessNode ACMap[15][15];	//�����ֹ�ֵ��ͳ���Լ������ӵĿ�λ����
	ChessNode DCMap[15][15];	//���طֹ�ֵ��ͳ�ƶԷ����ӵĿ�λ����
public:
	StatusBoard()		//��ʼ������ֵ���Ƿ񿿽�����ȷ��
	{
		
	}
	void Update(int r, int c)		//Ϊʲô��Ҫ���������������ж�����Ϊ��ֻ��Ҫ���������ϵ�ֵ������ֵ�����
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
	//��ʼ��λ�õ÷ֱ�
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			PositionMap[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
		}
	}
	return 0;
}
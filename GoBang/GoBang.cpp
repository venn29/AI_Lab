#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

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

class StatusBoard		//���ֱ���һ�����������Ͽ�λ������֣�ͨ������������ʽ����
{
private:
	int Map[15][15];
public:
	StatusBoard()		//��ʼ������ֵ���Ƿ񿿽�����ȷ��
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				Map[i][j] = 7 - Max(abs(7 - i), abs(7 - j));
			}
		}
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
	return 0;
}
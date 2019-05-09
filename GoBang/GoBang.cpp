#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

enum ChessNodeType { None,L1, D1, L2, D2, L3, D3, L4, D4 };		//��������,
//��1����1����2����2����3����3����4����4
//���ֱ�ʾ�����������ʾ�ս������ͣ���Ϊ�գ���Ϊ�Է���
enum MultiType{None,Chl,H4,SC4,C4H3,SH3,H3M3,M4,H3,SH2,M3,H2M2,H2,M2};		//��������������ڵĵ�������
//����,��4��˫��4����4��3��˫��3����3��3����4����3��˫��2����3����2��2����2����2����
//�������͵Ķ��壺���磬��������Ϊ��4����ʾ������ǻ�4��һ����5��,��������Ϊ˫��������ʾ������������Ժ�Ϳ��Գ�Ϊ˫����

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
	StatusBoard* AIStatus;
	int GetScore() { return score; }
	int Update(int r, int c)//ԭ��ͬ�£���ֻ��Ҫ�����������ϵ�4������,������Ҫ������
	{

	}
};

//////////////////////////////////////
AI* myAI;		//AI��ȫ�ֲ���Ψһ�ģ����Է������
////////////////////////////////


class ChessNode		//�����ϵĵ�ṹ����Ҫ��Ϊ��������λ�����������ֵ���ǿ�λ��ķ�Ϊ-1
{
private:
	int r, c;
	int score;
	int flag[8];			//0��ʾ�Ա��ǿ�λ��1��ʾ�Ա��������ҷ������ӣ�2��ʾ���ҷ������Ӹ�����3��ʾ���ڵз�����
	int num[8];			//��flagΪ0�������ֵ��ʾ������ҷ���������������Ͼ����Ҷ�Զ����Ϊ-n����ʾ����ĵз���������������Ͼ����Ҷ�Զ	
						//��flagΪ1�������ֵ��ʾ���ڵĵط������ҷ���������(ֻ���Ǹ����ǲ����ģ���Ҫ�������ͣ������Ļ��ǻ�ģ��ܲ�һ��,���Կ��ǰ�flag����Ϊint�������ʾ����)
						//���Կ�����type��ʾ��λ��������ĵ㣺����һ�����ˣ�������һ���»���,�ȽϷ���
	//�˸�����˳���ǣ��������£����ϣ����£����£�����
	MultiType Mtype[8];			//�˸������ϵĵ�������
	ChessNodeType Ctype;		//�����ӵ�����			
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

	void Update_DirectionType(int target,int player)	//���µ�������			//Ϊ�˱�֤ÿһ����λ�����ʹ���Ա߿�λ������ν��������������Ҫ�����ӵ������ܷ�ɢ����
	{
		int ry=r;
		int cx=c;
		int ChessLen=0;
		int End_type;		
		if(player==1)
		{	

			int deltac,deltar;		//ÿ�ε�����
			switch(target)
			{
				case 0:			//����
					deltac=-1;
					deltar=0;
					break;
				case 1:		//����
					deltac =1;
					deltar=0;
					break;
				case 2:		//����
					deltac = 0;
					deltar=-1;
					break;
				case 3:		//����
					deltac=0;
					deltar=1;
					break;
				case 4:		//����
					deltac=-1;
					deltar=-1;
					break;
				case 5:		//����
					deltac=1;
					deltar=1;
					break;
				case 6:		//����
					deltac=-1;
					deltar=1;
					break;
				case 7:		//����
					deltac=1;
					deltar=-1;
					break;
				default:
					deltac=20;
					deltar=20;
			}
			if(flag[target]==0)
				Mtype[target]= myAI->AIStatus->ACMap[r+deltar][c+deltac];
			else			//ͳ���ڽ������������ս������
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
		//ע�⣬��Ĭ��Update��������newr��newc���ҵĵ�ǰr,c��ĳ��������ͬһ������

	//���Է��������ķ�
	int Update(int newr, int newc, int player)		//playerΪ1����ʾAI��Ϊ2��ʾ���//�����Ժ���Ҫ����֮ǰ������ڵ��ֵ�������˳���ǰ�ݹ��ʱ��Ļָ�������������鲻Ӧ���ɱ����е�Update������������Ҫ�ⲿ�����Լ�����
	{
		int Dis;
		if (player == 1)		//��Ԫ�����У���һ����ֻ����һ�������	//������5����ĵ㣬���ҵ�ǰ�㣬��û��ʲô��ϵ
		{
			int target = Target(newr, newc);		//�õ����·���
			Dis=Distance(i,newr,newc);
			if (flag[target] == 0)
			{
				if (Dis==1)		//�жϾ��룬�����1.����
				{
					flag[target]=1;
					Update_DirectionType(target,player);
				}
				else		//���µ�����Ȼ�����ڣ�����Num����
				{	
					if(num[target]>Dis)
						num[target]=Dis;
					return 0;	
				}
			}
		}
	}

};

class StatusBoard		//���ֱ���һ�����������Ͽ�λ������֣�ͨ������������ʽ����
{
private:
	int Map[15][15];			//���طֺͽ����ֵ��ܺ�
	ChessNode* ACMap[15][15];	//�����ֹ�ֵ��ͳ���Լ������ӵĿ�λ����
	ChessNode* DCMap[15][15];	//���طֹ�ֵ��ͳ�ƶԷ����ӵĿ�λ����
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
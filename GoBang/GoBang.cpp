#include<iostream>
#include<math.h>
#include<vector>
#include<string>
using namespace std;

int Point[7] = { 1000000, 10000, 5000, 1000,  200,  100,  10 };
//                ������  ��4�� ��4�� ������  ��3   ��2    ��2���߻�1

//����7λ����4 5λ
//����Ϊ�����ֽ����ɱ��������ʤ���������ɱ����Ϊ100000��6λ
enum DirectionType { DNone,L1, D1, L2, D2, L3, D3, L4, D4 };		//��������,
//��1����1����2����2����3����3����4����4
//���ֱ�ʾ�����������ʾ�ս������ͣ���Ϊ�գ���Ϊ�Է���
enum MultiType{MNone,Chl,H4,SC4,C4H3,SH3,H3M3,M4,H3,C4,SH2,M3,H2M2,H2,M2,H1,M1};		//��������������ڵĵ�������
//����,��4��˫��4����4��3��˫��3����3��3����4����3����4��˫��2����3����2��2����2����2����1����1
//�������͵Ķ��壺���磬��������Ϊ��4����ʾ������ǻ�4��һ����5��,��������Ϊ˫��������ʾ������������Ժ�Ϳ��Գ�Ϊ˫����

int PositionMap[15][15];		//ÿ��λ�õĹ̶��֣�

//����string�����Լӿ��ж��ٶȣ�����ÿ���ж϶�Ҫ�����ַ���(��д)
string Mapr[15];		//�����ͼ
string Mapc[15];		//�����ͼ
string Mapneg[29];		//���ϵ����£�-45��
string Mappos[29];		//���µ����ϣ�45��

//ÿһ�����͵�ģʽ������ 
int PatterCount[7] = { 1,1,5,4,10,6,9 };
int Map[15][15];		//����

string AIpatter[7][10] = {
	{"11111"},		//����
	{"011110"},		//��4
	{"011112","211110","10111","11011","11101"},		//��4
	{"011100","001110","010110","011010"},		//��3
	{"001112","211100","010112","211010","011012","210110","10011","11001","10101","2011102"},	//��3	//��3
	{"001100","000110","011000","001010","010100","010010"},		//��2
	{"000112","211000","001012","210100","010001","100010","2010102","2011002","2001102"}		//��2
};

string Playerpatter[7][10] = {
	{"22222"},		//����
	{"022220"},		//��4
	{"022221","122220","20222","22022","22202"},		//��4
	{"022200","002220","020220","022020"},		//��3
	{"002221","122200","020221","122020","022021","120220","20022","22002","20202","1022201"},	//��3	//��3
	{"002200","000220","022000","002020","020200","020020"},		//��2
	{"000221","122000","002021","120200","020002","200020","1020201","1022001","1002201"}		//��2
};
//ģʽ����Ҳ��������,ע�⣬AI�����Ӷ���1�������2��ͬһ�����ͣ��÷ֻ�Ϊ�෴����������ȻӦ��д��


int evaluate(int type, int rank, int player)		//�������ϵ�һ��(4�ֽǶ�)��Ϊstring���룬�жϵ÷֣�����0123,��Ӧ�ᣬ�ݣ�pos,neg,  th��ʾ���,Ҫ��string����ĵڼ���ȥȡ,player��ʾ����λ���
{
	string substring;		//�Ӵ�
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
	if (player == 1)		//AI����
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
	else				//�������
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


//���⣬��δ�����ң��½�һ���ࣿ������Ϊ�����洢��

class AI		//AI�ĵ�ǰ���֣��Լ��������
{
private:
	int score;		//����
	int AIrow[15];
	int AIcol[15];
	int AIpos[29];
	int AIneg[29];		//���У����У���45�㣬��45��ĸ����е�ֵ,���У�б��29������8����0û��ͳ�Ʊ�Ҫ��
	int AImaxrow;
	int AImaxcol;
	int AImaxpos;
	int AImaxneg;		//���е����ֵ
	int AIcountTh;		//1000���ϵ���/��/pos/neg������

	int Prow[15];
	int Pcol[15];
	int Ppos[29];
	int Pneg[29];		//���У����У���45�㣬��45��ĸ����е�ֵ,���У�б��29������8����0û��ͳ�Ʊ�Ҫ��
	int Pmaxrow;
	int Pmaxcol;
	int Pmaxpos;
	int Pmaxneg;		//���е����ֵ
	int PcountTh;
public:
	AI()
	{
		AIStatus = new StatusBoard;			//��ʼ������������ֵ��������
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
	}
	StatusBoard* AIStatus;
	StatusBoard* PlStatus;
	int GetScore() { return score; }
	int Update(int r, int c,int func)//ԭ��ͬ�£���ֻ��Ҫ�����������ϵ�4������,������Ҫ������		ǰ����string���鶼��0,0��ʼ	��45�ȣ�rank=r+c ,��45�ȴ����½ǿ�ʼ��func0��ʾ���ӣ�1��ʾȡ��
	{
		int newpos, newneg;
		newpos = r + c;
		newneg = 14 - r + c;
		int AIrold, AIcold, AIpold, AInold, Prold, Pcold, Ppold, Pnold;		//ԭ����ֵҪ����,Ϊʲô��������˳��ݹ�Ҫɾ���㣬��Ҫ�ж�count�ǲ�����Ҫ��С��������һ������
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
		if (func == 0)		//����
		{
			if (AIrow[r] >= 1000 && AIrold<1000)
				AIcountTh++;
			if (AIcol[c] >= 1000 && AIcold < 1000)
				AIcountTh++;
			if (AIpos[newpos] >= 1000 && AIpold < 1000)
				AIcountTh++;
			if (AIneg[newneg] >= 1000 && AInold < 1000)
				AIcountTh++;
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
		}

	}
};

//////////////////////////////////////
AI* myAI;		//AI��ȫ�ֲ���Ψһ�ģ����Է������
////////////////////////////////


class ChessNode		//�����ϵĵ�ṹ����Ҫ��Ϊ��������λ�����������ֵ���ǿ�λ��ķ�Ϊ-1
{
private:
	int NodeType;
	int r, c;
	int score;
	int flag[8];			//0��ʾ�Ա��ǿ�λ��1��ʾ�Ա��������ҷ�����,2��ʾ���ڵз�����
	int Pnum[8];			//����������
						//��flagΪ1�������ֵ��ʾ���ڵĵط������ҷ���������(ֻ���Ǹ����ǲ����ģ���Ҫ�������ͣ������Ļ��ǻ�ģ��ܲ�һ��,���Կ��ǰ�flag����Ϊint�������ʾ����)
						//���Կ�����type��ʾ��λ��������ĵ㣺����һ�����ˣ�������һ���»���,�ȽϷ���
	//�˸�����˳���ǣ��������£����ϣ����£����£�����
	enum  DirectionType Dtype[8];			//�˸������ϵĵ�������
	enum MultiType LineType[4];			//�������ϵ�����
	int LinePoint[4];
	enum MultiType Ctype;		//�����ӵ�����			
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
		int deltac, deltar;		//ÿ�ε�����
		switch (target)
		{
		case 0:			//����
			deltac = -1;
			deltar = 0;
			break;
		case 1:		//����
			deltac = 1;
			deltar = 0;
			break;
		case 2:		//����
			deltac = 0;
			deltar = -1;
			break;
		case 3:		//����
			deltac = 0;
			deltar = 1;
			break;
		case 4:		//����
			deltac = -1;
			deltar = -1;
			break;
		case 5:		//����
			deltac = 1;
			deltar = 1;
			break;
		case 6:		//����
			deltac = -1;
			deltar = 1;
			break;
		case 7:		//����
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


	//�����������У��漰��������Ķ�������
	void lineEvalue(int i,int j,int k)	//����һ��ֱ���ϵ����ͣ�������������
	{	
		//�ǻ�4���߳�4������㣬���˾���Ӯ
		if (Dtype[i] == L4 || Dtype[j] == L4)//����
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//���ֵ
			return;
		}
		if (  flag[i]==1&&flag[j]==1&&(Pnum[i]+Pnum[j]==4))		//��4�м��
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//���ֵ
			return;
		}
		if(Dtype[i] == D4 || Dtype[j] == D4)		//��4��Ե��
		{
			LineType[k] = Chl;
			LinePoint[k] = LinePoint[0];		//���ֵ
			return;
		}

		//�ǻ�3�㣬���˿����γɻ�4
		if (Dtype[i] == L3&&flag[j]!=2 || Dtype[j] == L3 && flag[i] != 2)//������Ե��
		{
			LineType[k] = H4;
			LinePoint[k] = LinePoint[1];		//��4�ķ�
			return;
		}
		if ((Dtype[i] == L1 && Dtype[j] == L2) || (Dtype[i] == L2 && Dtype[j] == L1))	//�����м��
		{
			LineType[k] = H4;
			LinePoint[k] = LinePoint[1];		//��4�ķ�
			return;
		}


		//����3�㣬���˿����γɳ�4
		if (Dtype[i] == D3 && flag[j] != 2 || Dtype[j] == D3 && flag[i] != 2)//������Ե��
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//��4�ķ�
			return;
		}
		if ((Dtype[i] == L1 && Dtype[j] == D2) || (Dtype[i] == D2 && Dtype[j] == L1))	//�����м��
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//��4�ķ�
			return;
		}
		if ((Dtype[i] == L2 && Dtype[j] == D1) || (Dtype[j] == L2 && Dtype[i] == D1))	//�����м��
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//��4�ķ�
			return;
		}
		if ((Dtype[i] == L3 && flag[j] == 2) || (Dtype[j] == L3 && flag[i] == 2))	//������Ե��  2011102
		{
			LineType[k] = C4;
			LinePoint[k] = LinePoint[2];		//��4�ķ�
			return;
		}
		
		//�ǻ�2�㣬���˿��Ա����
		if (Dtype[i] == L2 && flag[j] != 2 || Dtype[j] == L2 && flag[i] != 2)//��2��Ե��
		{
			LineType[k] = H3;
			LinePoint[k] = LinePoint[3];		//��3�ķ�
			return;
		}
		if (Dtype[i] ==L1 && Dtype[j] == L1 )//��2�м��
		{
			LineType[k] = H3;
			LinePoint[k] = LinePoint[3];		//��3�ķ�
			return;
		}
		//ʣ�µľͲ������ˣ��Լ�֦����������
		LineType[k] = MNone;
		LinePoint[k] = 0;
		return;
	}
	
	void NodeEvalue()			//����score��ֵ
	{
		int i = 0;
		int k = 0;
		for (; k < 4; k++)
		{
			lineEvalue(i, i + 1,k);
			i = i + 2;
		}
		//�ж����:
		int count = 0;	//	ר�ż�����1000�������ϵ�ֵ
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
		score = max + PositionMap[r][c];	//����λ�ò���
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
		//ע�⣬��Ĭ��Update��������newr��newc���ҵĵ�ǰr,c��ĳ��������ͬһ������

	//������������ʱ�����ñ���Ա����
	void Update(int newr, int newc, int player)		//playerΪ1����ʾAI��Ϊ2��ʾ���//�����Ժ���Ҫ����֮ǰ������ڵ��ֵ�������˳���ǰ�ݹ��ʱ��Ļָ�������������鲻Ӧ���ɱ����е�Update������������Ҫ�ⲿ�����Լ�����
	{
		int Dis;
		int target = Target(newr, newc);		//�õ����·���
		Dis = Distance(target, newr, newc);
		if (player == NodeType)			//�������ͺ��ҵ�������ͬ  	//��Ԫ�����У���һ����ֻ����һ�������
		{
			if (flag[target] == 0)
			{
				if (Dis==1)		//�жϾ��룬�����1.����
				{
					flag[target]=1;
					Update_DirectionType(target,player);
				}
				
			}
			else if(flag[target]==1)		//�����ͽ����ҷ�����
			{
				if (Dis <= 5)	//������5����ĵ㣬���ҵ�ǰ�㣬��û��ʲô��ϵ
					Update_DirectionType(target, player);
			}
			if (flag[target] != 2&&Dis<=5)
				NodeEvalue();
		}
		else		//�������ͺ��Լ��෴
		{
			if (flag[target] == 0)
			{
				if (Dis == 1)		//�жϾ��룬�����1,���£���ǰ״̬�Ǳ�������״̬
				{
					flag[target] = 2;
					Update_DirectionType(target, player);
				}
			}
			else if (flag[target] == 1)		//�����н����ҷ����ӣ������µĵз����ӿ��ܸı��ҵ�ǰ�������
			{
				if (Dis <= 5)
					Update_DirectionType(target, player);
			}
			//����÷��������н����ҷ��ĵз����ӣ������Ӳ���Ӱ��
			if(flag[target]!=2&&Dis<=5)
				NodeEvalue();
		}
		
	}

	//���˳��ݹ飬���ӱ���������ʱ�򣬵��ñ���Ա������ͬ��ֻ����һ������
	void PickUp(int newr, int newc, int player)
	{
		int Dis;
		int target = Target(newr, newc);		//�õ����·���
		Dis = Distance(target, newr, newc);
		if (Dis == 1)		//�жϾ��룬�����1����FLAG
			flag[target] = 0;
		if (Dis > 5)		//�õ�һ��������5������ӣ�û�й�ϵ
			return;
		Update_DirectionType(target, player);
		NodeEvalue();
	}
};

class StatusBoard		//���ֱ���һ�����������Ͽ�λ������֣�ͨ������������ʽ����
{
private:
	int Map[15][15];			//���طֺͽ����ֵ��ܺ�
	
public:
	StatusBoard()		//��ʼ������ֵ���Ƿ񿿽�����ȷ��
	{
		
	}
	void Update(int r, int c)		//Ϊʲô��Ҫ���������������ж�����Ϊ��ֻ��Ҫ���������ϵ�ֵ������ֵ�����
	{

	}
	ChessNode* ACMap[15][15];	//�����ֹ�ֵ��ͳ���Լ������ӵĿ�λ����
	ChessNode* DCMap[15][15];	//���طֹ�ֵ��ͳ�ƶԷ����ӵĿ�λ����
};

int Max(int i, int j)
{
	if (i > j)
		return i;
	else
		return j;
}

void PrintBoard()		//��ӡ����
{

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
	int func;
	printf("������˭����:\n1����\n2��AI\n");
	scanf("%d", &func);
	if (func == 2)
	{
				//ֱ���µ�����
	}
	int win = 0;		//0Ϊ��û�г������1Ϊ���ʤ����2ΪAIʤ��
	char row;
	int col;
	while (!win)
	{
		PrintBoard();
		printf("\n��������Ҫ�µ�λ��:��ĸ ����\n");
		scanf("%c %d", &row, col);
		//����+����״̬+�ж�ʤ������

		//��������λ��

		//�����ж�ʤ������

		
	}
	return 0;
}
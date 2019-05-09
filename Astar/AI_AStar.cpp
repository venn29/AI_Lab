// AI_AStar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "pch.h" 
#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include<stdio.h>
#include<time.h>
#include<math.h>
const int SumNode = 450;			//18*25
#define MAXF 65535;

using namespace std;

int endr, endc;
int beginr, beginc;
int rmax, cmax;
int limitF;			//迭代深度的最大上限，如果超过了，就说明找不到了

int newf;

int **Map;
int **Status;
int **H;

enum Flag
{
	Untouched,		//从来没有遇到过的节点
	OPEN,			//Open表中
	Closed			//已经close掉的
};


struct Node			//A*中的节点定义
{
	int row, col;
	int G;
	int H;
	int F = MAXF;
	Node* Parent = NULL;	//Parent用于A*
};

struct INode	//IDA*中节点的定义
{
	int row, col;
	int G;
	int H;
	int F = MAXF;
	INode* Child;
};

class MinHeap
{
private:
	Node* vec[SumNode];
	int count;						//当前堆中的总的元素数量
public:
	MinHeap()
	{
		memset(vec, 0, sizeof(Node*)*SumNode);
		count = 0;
	}
	~MinHeap()
	{
		for (int i = 0; i < count; i++)
			delete vec[i];
	}
	bool Empty()
	{
		if (count == 0)
			return true;
		else
			return false;
	}

	Node* GetNode(int i)
	{
		return vec[i];
	}
	void Min_HeapFY(int i)			//可以考虑后续改成循环，因为如果堆节点特别多的话，递归占用的栈空间太大
	{
		int	l = 2 * i + 1;		//左子节点
		int	r = 2 * i + 2;		//右子节点
		int Largest;
		if (l <= count - 1 && vec[l]->F < vec[i]->F)
			Largest = l;
		else
			Largest = i;
		if (r <= count - 1 && vec[r]->F < vec[i]->F)
			Largest = r;
		if (Largest != i)
		{
			Node* temp;
			temp = vec[i];
			vec[i] = vec[Largest];
			vec[Largest] = temp;
			Min_HeapFY(Largest);
		}
	}

	Node* ExtractMin()
	{
		if (count == 0)
		{
			printf("error\n");
			exit(0);
		}
		Node* min = vec[0];
		vec[0] = vec[count - 1];
		vec[count - 1] = NULL;
		--count;
		Min_HeapFY(0);

		int minr, minc;
		minr = min->row;
		minc = min->col;
		Status[minr][minc] = Closed;
		return min;
	}

	int Serch(int r, int c)			//返回vec中的编号
	{
		int target;
		for (int i = 0; i < count; i++)
		{
			if (vec[i]->row == r && vec[i]->col == c)
			{
				target = i;
				break;
			}
		}
		return target;
	}

	void Decrease(int i, int NewG)		//因为堆的结构和x,y无关，i是数组下标
	{
		vec[i]->G = NewG;
		vec[i]->F = NewG + vec[i]->H;
		int a = i;
		int p = (a - 1) / 2;
		while (a > 0 && vec[p]->F > vec[a]->F)
		{
			Node* temp;
			temp = vec[a];
			vec[a] = vec[p];
			vec[p] = temp;
			a = p;
			p = (a - 1) / 2;
		}
	}
	void Insert(Node* n)
	{
		vec[count] = n;
		count++;
		int i = count - 1;
		int p = (i - 1) / 2;
		while (i > 0 && vec[p]->F > vec[i]->F)
		{
			Node* temp;
			temp = vec[i];
			vec[i] = vec[p];
			vec[p] = temp;
			i = p;
			p = (i - 1) / 2;
		}

		int inx, iny;
		inx = n->row;
		iny = n->col;
		Status[inx][iny] = OPEN;
	}
};

bool Pass(int r, int c)			//判断当前点是否是一个可通过的节点
{
	if (r < 0 || r>rmax - 1)
		return false;
	if (c < 0 || c>cmax - 1)
		return false;
	if (Map[r][c] == 1)
		return false;
	else
		return true;
}

void Handle(MinHeap* heap, Node* P, int r, int c)		//根据父节点，处理周围的节点
{
	if (Pass(r, c))		//可通行
	{
		if (Status[r][c] == Untouched)		//新节点
		{
			Node* NewNode = new Node;
			NewNode->row = r;
			NewNode->col = c;
			NewNode->G = P->G + 1;
			NewNode->H = abs(endr - r) + abs(endc - c);
			NewNode->F = NewNode->G + NewNode->H;
			NewNode->Parent = P;
			heap->Insert(NewNode);
		}
		else if (Status[r][c] == OPEN)				//已经在Open表中的节点,看是否需要更新
		{
			int i;
			i = heap->Serch(r, c);
			Node* C = heap->GetNode(i);
			if (C->G > P->G + 1)
			{
				heap->Decrease(i, P->G + 1);
				C->Parent = P;
			}
		}
		return;
	}
	else
		return;
}

void A_Star()
{
	clock_t begintime, endtime;
	double sumtime;
	//
	int GC;
	int HC;
	int FC;
	begintime = clock();
	Node* EndNode = new Node;
	EndNode->row = endr;
	EndNode->col = endc;
	EndNode->Parent = NULL;
	GC = 0;
	HC = abs(endr - beginr) + abs(endc - beginc);
	FC = GC + HC;

	Node* StartNode = new Node;
	StartNode->row = beginr;
	StartNode->col = beginc;
	StartNode->G = GC;
	StartNode->H = HC;
	StartNode->F = FC;
	StartNode->Parent = NULL;

	MinHeap* ASheap = new MinHeap;
	ASheap->Insert(StartNode);
	Node* Current;
	bool find = false;
	while (!ASheap->Empty())		//寻路的栈不为空
	{
		Current = ASheap->ExtractMin();
		if ((Current->row == EndNode->row) && (Current->col == EndNode->col))
		{
			EndNode = Current;
			find = true;
			break;
		}
		Handle(ASheap, Current, Current->row - 1, Current->col);
		Handle(ASheap, Current, Current->row + 1, Current->col);
		Handle(ASheap, Current, Current->row, Current->col - 1);
		Handle(ASheap, Current, Current->row, Current->col + 1);
	}
	endtime = clock();
	sumtime = (double)(endtime - begintime) / (double)CLOCKS_PER_SEC;
	string path;
	int step = 0;
	if (!find)
		printf("can not find a path\n");
	else			//打印路径
	{
		ofstream fout("output_A.txt");
		fout << "时间：" << sumtime <<"s"<< endl;
		fout << "操作序列:" << endl;
		Node* p = EndNode;
		Node* pre = p->Parent;
		while (p->Parent != NULL)
		{
			if (p->row == pre->row)		//行相等，处理列不同的情况,L或者R
			{
				if (p->col < pre->col)		//向左走
					path.push_back('L');
				else
					path.push_back('R');
			}
			else
			{
				if (p->row > pre->row)		//向下走
					path.push_back('D');
				else
					path.push_back('U');
			}
			++step;
			p = pre;
			pre = pre->Parent;
		}
		for (int i = step - 1; i >= 0; i--)
		{
			fout << path[i] << " ";
		}
		fout << endl << "总步数：" << step << endl;
	}
}
/*
INode* NewNode(int r, int c, int G)	//为深度优先搜索创建一个新的节点		//想得到真实的消耗值，只需要在得到路径以后跑到底就可以了，反正要打印的
{
	int H = abs(endr - r) + abs(endc - c);
	INode* NewNode = new INode;
	NewNode->row = r;
	NewNode->col = c;
	NewNode->G = G;
	NewNode->H = H;
	NewNode->F = G + H;
	NewNode->Child = NULL;
	return NewNode;
}
*/
/*
bool DFS(int maxf, INode* p)		//max最大深度，depth当前深度,p父节点
{
	if (p->F > maxf)
		return false;
	if (p->row == endr && p->col == endc)
		return true;
	INode* Next;
	bool found = false;
	for (int i = 0; i < 4; i++)
	{
		int r = p->row + plusr[i];
		int c = p->col + plusc[i];
		if (Pass(r, c))
			Next = NewNode(r, c, p->G + 1);
		else
			continue;
		found = DFS(maxf, Next);
		if (found)
		{
			p->Child = Next;
			break;
		}
		else
		{
			delete Next;
		}
	}
	return found;
}*/

int plusr[4] = { 0,1,0,-1 };
int plusc[4] = { 1,0,-1,0 };
bool DFS(int maxf, int r, int c,int g)
{
	if (r == endr && c == endc)
	{
		Map[r][c] = 2;
		return true;
	}
	int f = H[r][c] + g;
	if (f > maxf)
	{
		if (f < newf)
			newf = f;
		return false;
	}
	Map[r][c] = 2;
	Status[r][c] = f;
	bool found = false;
	int newr;
	int newc;
	for (int i = 0; i < 4; i++)
	{
		newr = r + plusr[i];
		newc = c + plusc[i];
		if (  (g + 1 +H[r][c] ) > Status[newr][newc]||Map[newr][newc] != 0 || c < 0||c>59)
			continue;
		found = DFS(maxf, newr, newc, g + 1);
		if (found)
			break;
	}
	if (!found)
		Map[r][c] = 0;
	return found;
}

int ToFindPath(int r, int c)
{
	int newr, newc;
	int i;
	for ( i = 0; i < 4; i++)
	{
		newr = r + plusr[i];
		newc = c + plusc[i];
		if (Map[newr][newc] == 2)
			break;
	}
	return i;
}

void IDA_Star()
{
	
	clock_t begintime, endtime;
	double sumtime;
	bool found = false;
	begintime = clock();
	int maxf = H[beginr][beginc];
	newf = 65535;
	while (!found && maxf < limitF)		//
	{
		for (int i = 0; i < rmax; i++)
		{
			for (int j = 0; j < cmax; j++)
				Status[i][j] = 65535;
		}
		found = DFS(maxf,beginr,beginc,0);
		if (found)
			break;
		maxf = newf;
		newf = 65535;

	}
	endtime = clock();
	sumtime = (double)(endtime - begintime) / (double)CLOCKS_PER_SEC;
	if (!found)
	{
		printf("can not find a path\n");
		return;
	}
	int fr, fc;
	fr = beginr;
	fc = beginc;
	string path;
	int next;
	for (int i = 0; i < maxf; i++)
	{
		next = ToFindPath(fr, fc);
		switch (next)
		{
		case 0: {path.push_back('R'); break; }
		case 1: {path.push_back('D'); break; }
		case 2:{path.push_back('L'); break;}
		case 3: {path.push_back('U'); break; }
		default: 
		{printf("error");
			break;
		}
		}
		Map[fr][fc] = 0;
		fr = fr + plusr[next];
		fc = fc + plusc[next];
	}
	ofstream fout("output_IDA.txt");
	fout << "时间" << sumtime <<"s"<< endl;
	fout << "总步数" << path.size() << endl << "动作序列";
	for (auto it : path)
		fout << it << " ";
	fout << endl;

}

int main()
{
	int func;
	string fname;
	printf("请输入您要选择的图：\n1、18*25\n2、30*60\n");
	//读文件：
	scanf("%d", &func);
	if (func == 1)
	{
		fname = "input.txt";
		rmax = 18;
		cmax = 25;
	}
	else
	{
		fname = "input2.txt";
		rmax = 30;
		cmax = 60;
	}
	string Buffer;
	ifstream fin(fname);
	if (!fin)
	{
		printf("error:无法打开文件\n");
		exit(0);
	}
	//初始化map和status：
	Map = new int*[rmax];
	Status = new int*[rmax];
	H = new int*[rmax];
	for (int i = 0; i < rmax; i++)
	{

		Map[i] = new int[cmax];
		Status[i] = new int[cmax];
		H[i] = new int[cmax];
		memset(Map[i], 0, cmax * sizeof(int));
		memset(Status[i], 0, cmax * sizeof(int));
	}
	//
	for (int i = 0; i < rmax; i++)
	{
		for (int j = 0; j < cmax; j++)
		{
			fin >> Buffer;
			Map[i][j] = atof(Buffer.c_str());
		}
	}
	if (func == 1)
	{
		beginr = 1; beginc = 0;
		endr = 16; endc = 24;
		limitF = 400;
	}
	else
	{
		limitF = 900;
		Buffer = "";
		char c;
		bool pointbegin = false;//表示开始读取坐标了,以(为标志
		while (fin >> c)
		{
			if (pointbegin)
				Buffer.push_back(c);
			else
			{
				if (c == '(')
					pointbegin = true;
			}
		}

		string NumBuffer;
		bool b = false;
		int i = 0;
		int temp[4];
		for (auto iter : Buffer)
		{
			if ('0' <= iter && iter <= '9')
			{
				if (b == false)
				{
					NumBuffer = "";			//开始读取数字
					b = true;
				}
				NumBuffer.push_back(iter);		//一个数字的后续
			}
			else
			{
				if (b == true)
				{
					temp[i] = atoi(NumBuffer.c_str());
					b = false;
					i++;
					if (i == 4)
						break;
				}
			}
		}
		beginr = temp[0];
		beginc = temp[1];
		endr = temp[2];
		endc = temp[3];

	}
	for (int i = 0; i < rmax; i++)
	{
		for (int j = 0; j < cmax; j++)
		{
			H[i][j] = abs(endr - i) + abs(endc - j);
		}
	}
	//判断起点和终点是否可达
	if (Map[endr][endc] == 1 || beginr < 0 || beginr>rmax - 1 || beginc < 0 || beginc>cmax - 1)
	{
		printf("起点不合法");
		return 0;
	}
	if (Map[endr][endc] == 1 || endr < 0 || endr>rmax - 1 || endc < 0 || endc>cmax - 1)
	{
		printf("终点不合法");
		return 0;
	}
	
	A_Star();
	IDA_Star();
	return 0;
}

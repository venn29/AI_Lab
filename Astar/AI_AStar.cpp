﻿// AI_AStar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "pch.h" 
#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include<stdio.h>
const int SumNode = 450;			//为堆空间分配的初始容量，可以有助于提升vector的效率，暂定为200
#define MAXF 65535;
using namespace std;

const int endr = 16;
const int endc = 24;


int Map[18][25];
int Status[18][25] = { 0 };

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
	bool found=false;		//标记这条路径有没有找到后续节点
	int cost=MAXF;		//真实的消耗
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
			if ( vec[i]->row ==r&&vec[i]->col==c)
			{
				target = i;
				break;
			}
		}
		return target;
	}

	void Decrease(int i,int NewG)		//因为堆的结构和x,y无关，i是数组下标
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
			a=p;
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

bool Pass(int r,int c)			//判断当前点是否是一个可通过的节点
{
	if (Map[r][c] == 1)
		return false;
	if (r < 0 || r>16)
		return false;
	if (c < 0 || c>24)
		return false;
	else
		return true;
}

void Handle(MinHeap* heap ,Node* P,int r,int c)		//根据父节点，处理周围的节点
{
	if (Pass(r, c))		//可通行
	{
		if (Status[r][c] == Untouched)		//新节点
		{
			Node* NewNode = new Node;
			NewNode->row = r;
			NewNode->col = c;
			NewNode->G = P->G + 1;
			NewNode->H = (16 - r) + (24 - c);
			NewNode->F = NewNode->G + NewNode->H;
			NewNode->Parent = P;
			heap->Insert(NewNode);
		}
		else if(Status[r][c] == OPEN)				//已经在Open表中的节点,看是否需要更新
		{
			int i;
			i=heap->Serch(r, c);
			Node* C = heap->GetNode(i);
			if (C->G > P->G + 1)
				heap->Decrease(i, P->G + 1);
		}
		return;
	}
	else
		return;
}

void A_Star()
{
	//
	int GC;
	int HC;
	int FC;
	Node* EndNode = new Node;
	EndNode->row = 16;
	EndNode->col = 24;
	EndNode->Parent = NULL;
	GC = 0;
	HC = (16 - 1) + (24 - 0);
	FC = GC + HC;

	Node* StartNode = new Node;
	StartNode->row = 1;
	StartNode->col = 0;
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
	string path;
	int step = 0;
	if (!find)
		printf("can not find a path\n");
	else			//打印路径
	{
		ofstream fout("output_A.txt");
		fout << "时间：" << endl;
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

Node* NewNode(int r, int c,int G)	//为深度优先搜索创建一个新的节点
{
	int H = (endr - r) + (endc - c);
	INode* NewNode = new INode;
	NewNode->row = r;
	NewNode->col = c;
	NewNode->G = G;
	NewNode->H = H;
	NewNode->F = G + H;
	if (H == 0)
		NewNode->found = true;
	NewNode->cost=G;
}

Node* DFS(int max, int depth,INode* p)		//max最大深度，depth当前深度,p父节点
{
	if (depth > max)
		return NULL;
	depth++;
	Node* Current[4] = { NULL };
	Node* Temp;
	int flag[4] = { 65535 };	//决定要选择哪一条路径先走
	//R
	int r = p->row;
	int c = p->col+1; 
	if (Pass(r, c))
		Current[0] = NewNode(r, c, p->G + 1);
	//D
	int r = p->row+1;
	int c = p->col;
	if (Pass(r, c))
	{
		Temp = NewNode(r, c, p->G + 1);
		if (Temp->F < Current[0]->F || Current[0] == NULL)
		{
			Current[1] = Current[0];
			Current[0] = Temp;
		}
	}
	//L
	int r = p->row;
	int c = p->col - 1;
	if (Pass(r, c))
	{
		Temp = NewNode(r, c, p->G + 1);
		int i;
		for ( i = 0; Current[i] != NULL &&Current[i]->F <= Temp->F ; i++);
		if(i!=2)
		{
			for (int j = 2; j > i; j--)
				Current[j] = Current[j - 1];
			Current[i] = Temp;
		}
	}
	//U
	int r = p->row-1;
	int c = p->col;
	if (Pass(r, c))
	{
		Temp = NewNode(r, c, p->G + 1);
		int i;
		for (i = 0; Current[i] != NULL && Current[i]->F <= Temp->F; i++);
		if (i != 3)
		{
			for (int j = 3; j > i; j--)
				Current[j] = Current[j - 1];
			Current[i] = Temp;
		}
	}
}

void IDA_Star()
{
	int GC;
	int HC;
	int FC;
	INode* EndNode = new INode;
	EndNode->row = 16;
	EndNode->col = 24;
	GC = 0;
	HC = (16 - 1) + (24 - 0);
	FC = GC + HC;

	INode* StartNode = new INode;
	StartNode->row = 1;
	StartNode->col = 0;
	StartNode->G = GC;
	StartNode->H = HC;
	StartNode->F = FC;
	int maxf = FC;
	INode* Current = StartNode;
	while (Current != EndNode)		//
	{

	}
}

int main()
{
	//读文件：
	string Buffer;
	string fname = "map2.txt";
	ifstream fin(fname);

	int i = 0;
	int j = 0;
	if (!fin)
	{
		printf("error:无法打开文件\n");
		exit(0);
	}
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			fin >> Buffer;
			Map[i][j] = atof(Buffer.c_str()); 
		}
	}
	A_Star();
	IDA_Star();
}

#include"pch.h"
#include <iostream>
#include<vector>
#include<fstream>
#include<string>
#include<stdio.h>
#include<time.h>
#include<math.h>
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


int plusr[4] = { 0,1,0,-1 };
int plusc[4] = { 1,0,-1,0 };
bool DFS(int maxf, int r, int c, int g)
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
		if ((g + 1 + H[r][c]) > Status[newr][newc] || Map[newr][newc] != 0 || c < 0 || c>59)
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
	for (i = 0; i < 4; i++)
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
		found = DFS(maxf, beginr, beginc, 0);
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
		case 2: {path.push_back('L'); break; }
		case 3: {path.push_back('U'); break; }
		default:
			{
				printf("error");
				break;
			}
		}
		Map[fr][fc] = 0;
		fr = fr + plusr[next];
		fc = fc + plusc[next];
	}
	ofstream fout("output_IDA.txt");
	fout << "时间" << sumtime << "s" << endl;
	fout << "动作序列";
	for (auto it : path)
		fout << it;
	fout << endl;
	fout << "总步数" << path.size() << endl;
	fout.close();

}

int main()
{
	int func;
	string fname;
	printf("请输入您要选择的图：\n1、18*25\n2、30*60\n");
	//读文件：
	cin >> func;
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

	IDA_Star();
	return 0;
}

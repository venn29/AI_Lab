// AI_AStar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h" 
#include <iostream>
#include<vector>
using namespace std;

int* Dis;		//距离表

	//堆的数据结构应该是 元素ID(i*m+j)，根据元素ID到距离表中查询即可
struct Node			//堆中的节点定义
{
	int x,y;
	int G;
	int H;
	int F;
}

class MinHeap
{
private:
	vector <int> vec;				//使用i*m+j表示迷宫中的元素（i为当前行数，j为当前列数，m为列总数）
	int count;						//当前堆中的总的元素数量
public:
	MinHeap()
	{
		vec.clear();
		count = 0;
	}
	void search(int a)
	{	
		//按层遍历
		for (int i = 0; i < count; i++)
		{

		}
	}

	void Min_HeapFY(int i)
	{

	}
	void Insert(int i)
	{
		vec.push_back(i);
		count++;
		Min_HeapFY(i);
	}
};

int main()
{
    std::cout << "Hello World!\n"; 
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

int Max(int i, int j)
{
	if (i > j)
		return i;
	else
		return j;
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

class AI		//AI�ĵ�ǰ���֣��Լ��������
{
private:
	int score;		//����
public:
	int GetScore() { return score; }

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
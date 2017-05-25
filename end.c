#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>





void print_path();




void path_maker2();
void LRUD(char a, int dx, int dy);

int table[10][10] = { 0, };


int main(void)
{

	srand((unsigned)time(NULL));

	path_maker2();
	print_path();

	return 0;

}





void print_path()  // ���� 0���� 1���� �Ǵ��ؼ� ���� ������
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (table[i][j] == 1)
				printf("\033[1m |##|\033[0m");                  //�۾��� ���� �ؼ� �ߺ��̰� ��
			else
				printf(" |%d%d|", i, j);

		}
		printf("\n\n");
	}                //1�ʰ��� ������
}

void path_maker2()
{
	char rule[4] = { 'L','R','U','D' };
	for (int i = 0; i < 10; i++)                                    //�迭 0���� ��� �ʱ�ȭ(�� ����)
	{
		for (int j = 0; j < 10; j++)
		{
			table[i][j] = 0;
		}
	}
	int direction = 0;
	int va;
	int x = 0;
	int y = 0;
	table[0][0] = 1;
	int lava[12][12] = { 0, };


	int Le, Ri, Up, Do = 1;

	for (int i = 0; i < 12; i++)
	{
		lava[0][i] = 1;
		lava[11][i] = 1;
	}
	for (int i = 0; i < 12; i++)
	{
		lava[i][0] = 1;
		lava[i][11] = 1;
	}

	while (x != 10 && y != 10)
	{
		if (lava[y+1][x] == 1)			//����
		{
			rule[0] = 'N';
			Le = 0;
		}
		if (lava[y+1][x+2] == 1)		//������
		{
			rule[1] = 'N';
			Ri = 0;
		}
		if (lava[y][x+1] == 1)			//����
		{
			rule[2] = 'N';
			Up = 0;
		}

		if (lava[y+2][x+1] == 1)			//�Ʒ���
		{
			rule[3] = 'N';
			Do = 0;
		}
		va = Le + Ri + Up + Do;

		for (int i = 0; i < va; i++)
		{
			int j = i;
			while (rule[j] == 'N' && j < 4)
				j++;
			rule[i] = rule[j];
			rule[j] = 'N';
		}


		lava[x + 1][y] = 1;
		lava[x + 1][y + 2] = 1;
		lava[x][y + 1] = 1;
		lava[x + 2][y] = 1;								//���� ���� ���� �����¿쿡�� ���� ���� �� ����.

		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 12; j++)
			{
				printf("%d ",lava[i][j]);
			}
			printf("\n");
		}
		
		if (va == 0)
		{
			printf("stuck\n");
		}
		else
			direction = rand() % va;

		switch (direction)
		{
		case 0:
			LRUD(rule[0], x, y);
			break;
		case 1:
			LRUD(rule[1], x, y);
			break;
		case 2:
			LRUD(rule[2], x, y);
			break;
		case 3:
			LRUD(rule[3], x, y);
			break;
		default:
			break;
		}
		table[x][y] = 1;
		printf("%d\n", table[x][y]);
		printf("%d, %d\n", x, y);
		Le = 1;
		Ri = 1;
		Up = 1;
		Do = 1;

		Sleep(300);

	}
}

void LRUD(char a, int dx, int dy)
{
	if (a == 'L')
		dx = dx - 1;
	if (a == 'R')
		dx = dx + 1;
	if (a == 'U')
		dy = dy - 1;
	if (a == 'D')
		dy = dy+ 1;
}

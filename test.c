#include <stdio.h>
#include <stdlib.h> // 난수 생성
#include <time.h> // 시간
#include <windows.h> // 딜레이 생성





int table[10][10] = {0, };






void path_maker(int table[][10])
{

	int x = 0;
	int y = 0;
	table[x][y] = 1;

	while (table[9][9] != 1 && x != 9 && y != 9)
	{
		if (rand() % 2 == 0)
		{
			x++;
			table[x][y] = 1;
		}
		else
		{
			y++;
			table[x][y] = 1;
		}

	}
	if (x == 9)
	{
		for (; y < 10; y++)
			table[x][y] = 1;
	}
	if (y == 9)
	{
		for (; x< 10; x++)
			table[x][y] = 1;
	}
}

void print_path(int table[][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (table[i][j] == 1)
				printf(" |##|");
			else
				printf(" |%d%d|", i, j);

		}
		printf("\n\n");
	}
}

void print_menu()
{
	int menu = 0;
	while (menu != 2)
	{
		printf("==================================\n");
		printf("1. Game Start\n");
		printf("2. Exit\n");
		printf("==================================\n\n\n");

		printf("select menu : ");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1:
			path_maker(table);
			print_path(table);
			break;

		case 2:
			break;

		default:
			break;
		}

	}

}





int main(void)
{
	srand(time(NULL));
	printf("pathfinder_201310395_박선혁\n");
	print_menu();
	return 0;
}


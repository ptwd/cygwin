
#include <stdio.h>
#include <stdlib.h> // 난수 생성
#include <time.h> // 시간
#include <windows.h> // 딜레이 생성



void path_maker(int table[][10]);
void print_menu();
void print_path(int table[][10]);
void gotoxy(int x, int y);
void print_game();


int table[10][10] = { 0, };



int main(void)
{

	srand(time(NULL));                                      //srand 여러번 선언하면 계속 초기화되서 중복가능성이 있음
	printf("pathfinder_201310395_박선혁\n");
	print_menu();
	return 0;
}


void path_maker(int table[][10])
{

	for (int i = 0; i < 10; i++)                                    //배열 0으로 모두 초기화(길 없앰)
	{
		for (int j = 0; j < 10; j++)
		{
			table[i][j] = 0;
		}
	}

	int x = 0;
	int y = 0;
	table[x][y] = 1;

	while (table[9][9] != 1 && x != 9 && y != 9) //만약 9,9에 도달하거나, 가장 오른쪽, 가장 아래에 위치했을때는 그 상황에 맞게
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

void print_path(int table[][10])  // 길이 0인지 1인지 판단해서 길을 보여줌
{
	system("clear");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (table[i][j] == 1)
				printf("\033[1m |##|\033[0m");                  //글씨를 굵게 해서 잘보이게 함
			else
				printf(" |%d%d|", i, j);

		}
		printf("\n\n");
	}
	Sleep(1000);                    //1초간만 보여줌
	system("clear");
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
			print_game();
			break;

		case 2:
			break;

		default:
			break;
		}

	}

}

void gotoxy(int x, int y)
{
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}

void print_game()
{
	system("clear");
	printf("\n\n\t\t");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}
		printf("\t\t\t\t\t");

		for (int j = 0; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}
		printf("\n\n\t\t");
	}

}



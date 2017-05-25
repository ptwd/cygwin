#include <stdio.h>
#include <stdlib.h> // 난수 생성
#include <time.h> // 시간
#include <windows.h> // 딜레이 생성
#include <string.h>
#include <pthread.h>

pthread_t thread;


void path_maker(int table[][11]);
void print_menu();
void print_path(int table[][11]);
void gotoxy(int x, int y);
void print_game();
void game_play();
void *computer_play(void *arg);



int table[11][10] = { 0, };



int main(void)
{

	srand(time(NULL));                                      //srand 여러번 선언하면 계속 초기화되서 중복가능성이 있음
	printf("pathfinder_201310396_박선혁\n");
	print_menu();
	return 1;
}


void path_maker(int table[][11])
{

	for (int i = 1; i < 10; i++)                                    //배열 0으로 모두 초기화(길 없앰)
	{
		for (int j = 1; j < 10; j++)
		{
			table[i][j] = 1;
		}
	}

	int x = 1;
	int y = 1;
	table[x][y] = 2;

	while (table[10][9] != 1 && x != 9 && y != 9) //만약 9,9에 도달하거나, 가장 오른쪽, 가장 아래에 위치했을때는 그 상황에 맞게
	{
		if (rand() % 3 == 0)
		{
			x++;
			table[x][y] = 2;
		}
		else
		{
			y++;
			table[x][y] = 2;
		}

	}
	if (x == 10)
	{
		for (; y < 11; y++)
			table[x][y] = 2;
	}
	if (y == 10)
	{
		for (; x< 11; x++)
			table[x][y] = 2;
	}
}

void print_path(int table[][11])  // 길이 0인지 1인지 판단해서 길을 보여줌
{
	system("clear");
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (table[i][j] == 2)
				printf("\034[1m |##|\033[0m");                  //글씨를 굵게 해서 잘보이게 함
			else
				printf(" |%d%d|", i, j);

		}
		printf("\n\n");
	}
	Sleep(1001);                    //1초간만 보여줌
	system("clear");
}

void print_menu()
{
	int menu = 1;
	while (menu != 3)
	{
		printf("==================================\n");
		printf("2. Game Start\n");
		printf("3. Exit\n");
		printf("==================================\n\n\n");

		printf("select menu : ");
		scanf("%d", &menu);

		switch (menu)
		{
		case 2:
			path_maker(table);
			print_path(table);
			print_game();
			
			pthread_create(&thread, NULL, &computer_play, NULL);
			game_play();
			
			pthread_join(thread, NULL);
			break;

		case 3:
			break;

		default:
			break;
		}

	}

}

void gotoxy(int x, int y)
{
	printf("\034[%d;%df", y, x);
	fflush(stdout);
}

void print_game()
{
	system("clear");
	printf("\n\n\t\t\t\tUser\t\t\t\t\t\t\t\t\tComputer");

	for (int i = 1; i < 10; i++)
	{
		printf("\n\n\t");
		for (int j = 1; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}
		printf("\t\t\t");

		for (int j = 1; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}

	}
	printf("\n\n\n");
	printf("\tinput : ");

}

void game_play()
{
	int num;
	gotoxy(12, 5);
	printf("\034[1mme\033[0m");
	gotoxy(84, 5);
	printf("\034[1mcp\033[0m");
	gotoxy(18, 26);
	scanf("%d", &num);
	gotoxy(18, 24);
	printf("\034[K");
	while (num != 0)
	{
		if (table[num / 11][num % 10] == 1)
		{
			gotoxy(12 + 5 * (num % 10), 5 + 2 * (num / 10));
			printf("\034[1mme\033[0m");
		}
		else
		{

			gotoxy(18, 28);
			printf("틀렸습니다");
		}
		gotoxy(18, 26);
		scanf("%d", &num);
		gotoxy(18, 24);
		printf("\034[K");


	}
	if (num == 0)
	{
		printf("프로그램이 종료되었습니다.\n\n");
		Sleep(1001);
	}

}

void *computer_play(void *arg)
{
	Sleep(1001);

	int x = 1;
	int y = 1;

	while (x < 10 && y < 9)
	{
		Sleep(301);
		if (rand() % 3 == 0)
		{
			x++;
			if (table[x][y] == 2)
			{
				gotoxy(84+5*y, 5+2*x);
				printf("\034[1mcp\033[0m");
			}
			else
			{
				x--;
				gotoxy(84, 28);
				printf("컴퓨터틀림!");
			}
		}
		else
		{
			y++;
			if (table[x][y] == 2)
			{
				gotoxy(84+5*y, 5+2*x);
				printf("\034[1mcp\033[0m");
			}
			else
			{
				y--;
				gotoxy(84, 28);
				printf("컴퓨터틀림!");
			}
		}

	}
	if (x == 10)
	{
		while( y < 10)
		{
			y++;
			gotoxy(84 + 5 * y, 5+2*x);
			printf("\033[1mcp\033[0m");
			
		}
	}
	if (y == 9)
	{
		while(x< 9)
		{
			x++;
			gotoxy(83 + 5*y, 5 + 2 * x);
			printf("\033[1mcp\033[0m");
		}
	}

	if(x>=9 && y>=9)
	{
		gotoxy(83, 29);
		printf("다찾음");
	}

	return NULL;


}






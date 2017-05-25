#include <stdio.h>
#include <stdlib.h> // 난수 생성
#include <time.h> // 시간
#include <windows.h> // 딜레이 생성
#include <string.h>
#include <pthread.h>

pthread_t thread1;
pthread_t thread2;


void path_maker();
void path_maker2();
void print_menu();
void print_path();
void gotoxy(int x, int y);
void print_game();
void computer_positive(int x, int y);
void computer_negative(int x, int y);
void *game_play(void *arg);
void *computer_play(void *arg);
void *myreplay(void *arg);
void *comreplay(void *arg);
void *hard_me(void *arg);
void print_me();
void LRUD(char a, int* dx, int* dy);



int table[10][10] = { 0, };
int flag1 = 0;
int flag2 = 0;
int me_win = 0;
int com_win = 0;
int me_num[500] = { 0, };
int com_num[500] = { 0, };
long me_replay[5000] = { 0, };
int com_replay[5000] = { 0, };
int a = 0;
int b = 0;




int main(void)
{
	srand(time(NULL));                                      //srand 여러번 선언하면 계속 초기화되서 중복가능성이 있음
	printf("pathfinder_201310395_박선혁\n");
	print_menu();
	return 0;
}


void path_maker()
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

void path_maker2()
{
	char rule[4] = { 'L','R','U','D' };
	for (int i = 0; i < 10; i++)                                    //배열 0으로 모두 초기화(길 없앰)
	{
		for (int j = 0; j < 10; j++)
		{
			table[i][j] = 0;
		}
	}
	int direction = 0;
	int va = 0;
	int x = 0;
	int y = 0;
	int test = 0;
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

	lava[1][1] = 1;
	while ((x != 9 || y != 9) &&test !=100)
	{

		Le = 1;																		//모두 초기상태로
		Ri = 1;
		Up = 1;
		Do = 1;
		rule[0] = 'L';
		rule[1] = 'R';
		rule[2] = 'U';
		rule[3] = 'D';

		if (y >= 8)
		{
			rule[0] = 'N';													//밑에서두번째칸까지는 무조건 오른쪽으로
			Le = 0;
		}
		if (x >= 8)
		{
			rule[2] = 'N';
			Up = 0;
		}
		if (x <= 1)
		{
			rule[2] = 'N';
			Up = 0;
		}
		if (y <= 1)
		{
			rule[0] = 'N';
			Le = 0;;

		}

		if (lava[y + 1][x] == 1)			//왼쪽
		{
			rule[0] = 'N';
			Le = 0;
		}
		if (lava[y + 1][x + 2] == 1)		//오른쪽
		{
			rule[1] = 'N';
			Ri = 0;
		}
		if (lava[y][x + 1] == 1)			//위쪽
		{
			rule[2] = 'N';
			Up = 0;
		}

		if (lava[y + 2][x + 1] == 1)			//아래쪽
		{
			rule[3] = 'N';
			Do = 0;
		}

		for (int i = 0; i < va; i++)
		{
			int j = i;
			while (rule[j] == 'N' && j < 4)
				j++;
			rule[i] = rule[j];
			rule[j] = 'N';
		}

		lava[y+1][x] = 1;
		lava[y+1][x+2] = 1;
		lava[y][x + 1] = 1;
		lava[y + 2][x] = 1;								//전에 만든 길의 상하좌우에는 길을 만들 수 없다.
		lava[y + 1][x + 1] = 1;

		va = Le + Ri + Up + Do;

		if (va == 0)
		{
			printf("stuck\n");							//길만들기 실패, 알고리즘을 추가해서 va==0인상황이 없어야 할듯
		}
		else
		{
			direction = rand() % va;
		}

		switch (direction)
		{
		case 0:
			LRUD(rule[0], &x, &y);
			break;
		case 1:
			LRUD(rule[1], &x, &y);
			break;
		case 2:
			LRUD(rule[2], &x, &y);
			break;
		case 3:
			LRUD(rule[3], &x, &y);
			break;
		default:
			break;
		}

		table[x][y] = 1;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (table[i][j] == 1)
					printf("\033[1m |##|\033[0m");                  //테스트용
				else
					printf(" |%d%d|", i, j);

			}
			printf("\n\n");
		}

		Sleep(30);
		test++;

		/*
		추가구현사항
		길이 모서리와 2이하로 떨어져있을 경우에는 항상 바깥쪽으로 길이 나야한다
		길과 길사이의 거리가 5 이상이어야 안쪽을 파고 나올 수 있다.


		*/

	}
}

void LRUD(char a, int* dx, int* dy)
{
	if (a == 'L')
		*dx = *dx - 1;
	if (a == 'R')
		*dx = *dx + 1;
	if (a == 'U')
		*dy = *dy - 1;
	if (a == 'D')
		*dy = *dy + 1;
}

void print_path()  // 길이 0인지 1인지 판단해서 길을 보여줌
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
	while (menu != 6)
	{
		printf("==================================\n");
		printf("1. Game Start\n");
		printf("2. Game Start - Helper\n");
		printf("3. Replay\n");
		printf("4. HardCore\n");
		printf("5. advanced path maker\n");
		printf("6. Exit\n");
		printf("7. advanced game start(오류가능성 다분)\n");
		printf("==================================\n\n\n");

		printf("select menu : ");
		fflush(stdin);
		scanf("%d", &menu);
		fflush(stdin);

		switch (menu)
		{
		case 1:

			path_maker();
			print_game();
			pthread_create(&thread1, NULL, &game_play, NULL);
			pthread_create(&thread2, NULL, &computer_play, NULL);
			Sleep(100);
			while (!flag1 && !flag2)
			{
			}
			Sleep(1000);
			flag1 = 0;
			flag2 = 0;
			gotoxy(0, 33);
			printf("me vs com : %d vs %d\n\n", me_win, com_win);
			fflush(stdout);

			
			break;

		case 2:

			path_maker();
			print_path();
			print_game();
			pthread_create(&thread1, NULL, &game_play, NULL);
			pthread_create(&thread2, NULL, &computer_play, NULL);
			Sleep(100);
			while (!flag1 && !flag2)
			{
			}
			Sleep(1000);
			flag1 = 0;
			flag2 = 0;
			gotoxy(0, 33);
			printf("me vs com : %d vs %d\n\n", me_win, com_win);
			fflush(stdout);

			break;

		case 3:
			print_game();
			pthread_create(&thread1, NULL, &myreplay, NULL);
			pthread_create(&thread2, NULL, &comreplay, NULL);
			Sleep(100);
			while (!flag1 && !flag2)
			{
			}
			Sleep(1000);
			flag1 = 0;
			flag2 = 0;
			gotoxy(0, 33);
			printf("me vs com : %d vs %d\n\n", me_win, com_win);
			fflush(stdout);
			break;

		case 4:
			path_maker();
			print_game();
			pthread_create(&thread1, NULL, &hard_me, NULL);
			pthread_create(&thread2, NULL, &computer_play, NULL);
			Sleep(100);
			while (!flag1 && !flag2)
			{
			}
			Sleep(1000);
			flag1 = 0;
			flag2 = 0;
			gotoxy(0, 33);
			printf("me vs com : %d vs %d\n\n", me_win, com_win);
			fflush(stdout);
			break;

		case 5:
			path_maker2();
			print_path();
			break;


		case 6:
			printf("\n\n프로그램이 종료되었습니다.\n");
			fflush(stdout);

			break;

		case 7:
			path_maker2();
			print_game();
			pthread_create(&thread1, NULL, &game_play, NULL);
			pthread_create(&thread2, NULL, &computer_play, NULL);
			Sleep(100);
			while (!flag1 && !flag2)
			{
			}
			Sleep(1000);
			flag1 = 0;
			flag2 = 0;
			gotoxy(0, 33);
			printf("me vs com : %d vs %d\n\n", me_win, com_win);
			fflush(stdout);
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
	printf("\n\n\t\t\t\tUser\t\t\t\t\t\t\t\t\tComputer");

	for (int i = 0; i < 10; i++)
	{
		printf("\n\n\t");
		for (int j = 0; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}
		printf("\t\t\t");

		for (int j = 0; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}

	}
	printf("\n\n\n");
	printf("\tinput : ");

	gotoxy(11, 5);
	printf("\033[1mme\033[0m");													//00에 me, cp 출력
	gotoxy(83, 5);
	printf("\033[1mcp\033[0m");
	gotoxy(17, 26);

}

void computer_positive(int x, int y)
{
	printf("\033[s");
	gotoxy(83 + 5 * y, 5 + 2 * x);
	printf("\033[1mcp\033[0m");
	printf("\033[u");
	fflush(stdout);
}

void computer_negative(int x, int y)
{
	printf("\033[s");
	gotoxy(83, 28);
	printf("컴퓨터틀림!");
	printf("\033[u");
	fflush(stdout);
}

void *game_play(void *arg)
{
	pthread_detach(thread1);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	for (int i = 0; i < a; i++)
	{
		me_replay[i] = 0;

	}


	int i = 0;
	for (int j = 0; j < 500; j++)
	{
		me_num[j] = 0;

	}
	char line[100];



	while (me_num[i] != -1 && flag2 == 0)
	{

		clock_t start, end;
		i++;
		gotoxy(17, 26);
		start = clock();
		fgets(line, sizeof(line), stdin);
		sscanf(line, "%d", &me_num[i]);
		end = clock();
		me_replay[a] = me_num[i];
		a = a + 1;
		me_replay[a] = end - start;
		a = a + 1;

		
		//index 1부터 값을 받음. 다음 if문에서 비교할 인덱스가 필요하기때문


		if (me_num[i] > 0 && me_num[i] < 100)
		{
			if (abs(me_num[i] / 10 - me_num[i - 1] / 10) == 1 || abs(me_num[i] % 10 - me_num[i - 1] % 10) == 1)
			{
				if (table[me_num[i] / 10][me_num[i] % 10] == 1)
				{
					gotoxy(11 + 5 * (me_num[i] % 10), 5 + 2 * (me_num[i] / 10));
					printf("\033[1mme\033[0m");
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					if (me_num[i] == 99)
					{
						flag1 = 1;
						pthread_cancel(thread2);
						me_win++;
						break;
					}
				}
				else
				{
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					printf("길이 아닙니다.(종료 -1)");
				}

			}
			else
			{
				gotoxy(17, 28);
				printf("\033[K");
				fflush(stdout);
				printf("길을 만들 수 없습니다.(종료 -1)");
			}

		}
		else if (me_num[i] == -1)
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("프로그램이 종료되었습니다.\n\n");
			flag1 = 1;
			pthread_cancel(thread2);
		}
		else
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("1과 99사이의 숫자를 입력해 주세요.(종료 -1)");

		}
	}
}

void *computer_play(void *arg)
{
	pthread_detach(thread2);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int i = 0;
	int direction;

	int x = 0;
	int y = 0;
	int k = 0;
	int l = 0;
	int exist = 0;
	for (int j = 0; j < 500; j++)
	{
		com_num[j] = 0;
	}

	i++;
	while (!(x == 9 && y == 9) && flag1 == 0)
	{

		direction = rand() % 4;
		com_replay[b] = direction;
		b++;

		k = x + ((-2) * (direction / 2) + 1)*(direction % 2);
		l = y + ((-2) * (direction / 2) + 1)*((direction + 1) % 2);

		if (k >= 0 && k < 10 && l >= 0 && l < 10)
		{
			for (int z = 0; z < i; z++)
				if (com_num[z] == 10 * k + l)
					exist = 1;

			if (exist == 0)
			{
				Sleep(300);
				if (table[k][l] == 1)
				{
					x = k;
					y = l;
					i++;
					com_num[i] = 10 * y + x;

					computer_positive(x, y);

				}

				else
				{
					computer_negative(x, y);
				}

			}
			else
				exist = 0;
		}
		else
			continue;
	}
	if (x >= 9 && y >= 9)
	{
		printf("\033[s");
		gotoxy(83, 29);
		printf("다찾음");
		printf("\033[u");
		fflush(stdout);
		gotoxy(0, 31);

		com_win++;
		flag2 = 1;
		pthread_cancel(thread1);
	}
}

void *myreplay(void *arg)
{
	pthread_detach(thread1);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	int re = 0;


	int i = 0;
	for (int j = 0; j < 500; j++)
	{
		me_num[j] = 0;

	}
	char line[100];



	while (me_num[i] != -1 && flag2 == 0)
	{
		i++;
		gotoxy(17, 26);
		re++;
		Sleep(me_replay[re]);

		re--;
		me_num[i] = me_replay[re];
		printf("%d", me_replay[re]);
		re++;
		re++;



		//index 1부터 값을 받음. 다음 if문에서 비교할 인덱스가 필요하기때문


		if (me_num[i] > 0 && me_num[i] < 100)
		{
			if (abs(me_num[i] / 10 - me_num[i - 1] / 10) == 1 || abs(me_num[i] % 10 - me_num[i - 1] % 10) == 1)
			{
				if (table[me_num[i] / 10][me_num[i] % 10] == 1)
				{
					gotoxy(11 + 5 * (me_num[i] % 10), 5 + 2 * (me_num[i] / 10));
					printf("\033[1mme\033[0m");
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					if (me_num[i] == 99)
					{
						flag1 = 1;
						pthread_cancel(thread2);
						break;
					}
				}
				else
				{
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					printf("길이 아닙니다.(종료 -1)");
				}

			}
			else
			{
				gotoxy(17, 28);
				printf("\033[K");
				fflush(stdout);
				printf("길을 만들 수 없습니다.(종료 -1)");
			}

		}
		else if (me_num[i] == -1)
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("프로그램이 종료되었습니다.\n\n");
			flag1 = 1;
			pthread_cancel(thread2);
		}
		else
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("1과 99사이의 숫자를 입력해 주세요.(종료 -1)");

		}
	}
}

void *comreplay(void *arg)
{
	pthread_detach(thread2);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int i = 0;
	int re = 0;
	int direction;

	int x = 0;
	int y = 0;
	int k = 0;
	int l = 0;
	int exist = 0;
	for (int j = 0; j < 500; j++)
	{
		com_num[j] = 0;
	}

	i++;
	while (!(x == 9 && y == 9) && flag1 == 0)
	{
		direction = com_replay[re];
		re++;

		k = x + ((-2) * (direction / 2) + 1)*(direction % 2);
		l = y + ((-2) * (direction / 2) + 1)*((direction + 1) % 2);

		if (k >= 0 && k < 10 && l >= 0 && l < 10)
		{
			for (int z = 0; z < i; z++)
				if (com_num[z] == 10 * k + l)
					exist = 1;

			if (exist == 0)
			{
				Sleep(300);
				if (table[k][l] == 1)
				{
					x = k;
					y = l;
					i++;
					com_num[i] = 10 * y + x;

					computer_positive(x, y);

				}

				else
				{
					computer_negative(x, y);
				}

			}
			else
				exist = 0;
		}
		else
			continue;
	}
	if (x >= 9 && y >= 9)
	{
		printf("\033[s");
		gotoxy(83, 29);
		printf("다찾음");
		printf("\033[u");
		fflush(stdout);
		gotoxy(0, 31);

		com_win++;
		flag2 = 1;
		pthread_cancel(thread1);
	}

}

void *hard_me(void *arg)
{
	pthread_detach(thread1);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	for (int i = 0; i < a; i++)
	{
		me_replay[i] = 0;

	}


	int i = 0;
	for (int j = 0; j < 500; j++)
	{
		me_num[j] = 0;

	}
	char line[100];



	while (me_num[i] != -1 && flag2 == 0)
	{

		clock_t start, end;
		i++;
		gotoxy(17, 26);
		start = clock();
		fgets(line, sizeof(line), stdin);
		sscanf(line, "%d", &me_num[i]);
		end = clock();
		me_replay[a] = me_num[i];
		a = a + 1;
		me_replay[a] = end - start;
		a = a + 1;


		//index 1부터 값을 받음. 다음 if문에서 비교할 인덱스가 필요하기때문


		if (me_num[i] > 0 && me_num[i] < 100)
		{
			if (abs(me_num[i] / 10 - me_num[i - 1] / 10) == 1 || abs(me_num[i] % 10 - me_num[i - 1] % 10) == 1)
			{
				if (table[me_num[i] / 10][me_num[i] % 10] == 1)
				{
					gotoxy(11 + 5 * (me_num[i] % 10), 5 + 2 * (me_num[i] / 10));
					printf("\033[1mme\033[0m");
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					if (me_num[i] == 99)
					{
						flag1 = 1;
						pthread_cancel(thread2);
						me_win++;
						break;
					}
				}
				else
				{
					gotoxy(17, 28);
					printf("\033[K");
					fflush(stdout);
					printf("길이 아닙니다.(종료 -1)");
					for (int j = 0; j < a; j++)			//a는 scanf된 수들의 두배이므로 a로 더 넓게 초기화가능
						me_num[j] = 0;
					i = 0;
					print_me();
				}

			}
			else
			{
				gotoxy(17, 28);
				printf("\033[K");
				fflush(stdout);
				printf("길을 만들 수 없습니다.(종료 -1)");
				for (int j = 0; j < a; j++)
					me_num[j] = 0;
				i = 0;
				print_me();
			}

		}
		else if (me_num[i] == -1)
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("프로그램이 종료되었습니다.\n\n");
			flag1 = 1;
			pthread_cancel(thread2);
		}
		else
		{
			gotoxy(17, 28);
			printf("\033[K");
			fflush(stdout);
			printf("1과 99사이의 숫자를 입력해 주세요.(종료 -1)");
			for (int j = 0; j < a; j++)
				me_num[j] = 0;
			i = 0;
			print_me();

		}
	}
}

void print_me()
{
	gotoxy(0, 0);
	printf("\n\n\t\t\t\tUser");

	for (int i = 0; i < 10; i++)
	{
		printf("\n\n\t");
		for (int j = 0; j < 10; j++)
		{
			printf(" |%d%d|", i, j);

		}
	}
	printf("\n\n\n");
	printf("\tinput : ");

	gotoxy(11, 5);
	printf("\033[1mme\033[0m");													//00에 me, cp 출력
	gotoxy(83, 5);
	printf("\033[1mcp\033[0m");
	gotoxy(17, 26);
}

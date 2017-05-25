#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void gotoxy(int x, int y);

char flag;

int main(void)
{
	int i = 10;
	system("clear");
	while (1)
	{
		gotoxy(0, 0);
		printf("Input : ");
		scanf("%s", &flag);
		gotoxy(9, 2);
		printf("      ");
		switch (flag) {
		case '1':
			gotoxy(0, 5 * 1);
			printf("1번 입력");
			break;
		case '2':
			gotoxy(0, 5 * 2);
			printf("2번 입력");
			break;
		default:
			system("clear");
		}
	}

	return 0;
}

void gotoxy(int x, int y)
{
	printf("\033[%d;%df", y, x);
	fflush(stdout);
}


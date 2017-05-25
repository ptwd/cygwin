#include <stdio.h>
#include <stdlib.h>
#include <time.h>







void path_maker(int table[][10])
{
	int x = 0;
	int y = 0;
	table[x][y] = 1;
	srand(time(NULL));
	
	while(table[9][9] != 1 && x!=9 && y!=9)
	{
		if(rand()%2 == 0)
		{
			printf("rand가 0\n");
			x++;
			table[x][y] = 1;
		}
		else
		{
			printf("rand가 1\n");
			y++;
			table[x][y] = 1;
		}

	}
	if( x== 9)
	{
		for( ; y < 10; y++)
			table[x][y] = 1;
	}
	if( y == 9)
	{
		for( ; x< 10; x++)
			table[x][y] = 1;
	}
}

void print_path(int table[][10])
{
	for(int i = 0; i < 10; i ++)
	{
		for(int j = 0; j < 10; j++)
			printf("%3d", table[i][j]);
		printf("\n");
	}
}




int main(void)
{
	int table[10][10] = {0};
	print_path(table);
	path_maker(table);
	print_path(table);
	return 0;
}


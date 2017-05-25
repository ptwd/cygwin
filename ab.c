#include <stdio.h>
#include <pthread.h>

pthread_t thread;

void *thread_function(void *);

int main(void)
{
	int i = 10;
	int *pi = &i;
	pthread_create(&thread, NULL, &thread_function, (void *)pi);
	for( int j = 0; j < i; j++)
	{
		printf("main, j = %d \n", j);
	}
	pthread_join(thread, NULL);

	return 0;
}

void *thread_function(void *arg)
{
	int *pi = (int *)arg;
	int i = *pi;
	printf("i = %d\n", i);
	for(int j = 0; j < i; j++)
	{
		printf("thread, j = %d \n", j);
	}

	return NULL;
}



#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
int a=1;
pthread_mutex_t sync_mutex;
void *threadRoutine(void *argumentPointer)
{
	pthread_t id = pthread_self();
	printf("thread ID (TID) :: %lu\n", id);
	//Critical Section
	pthread_mutex_lock(&sync_mutex);
	for(int i=0;i<1000000;i++)a = a + 1;
	pthread_mutex_unlock(&sync_mutex);
	return NULL;
}
int main()
{
	pthread_t threadID[4];
	pthread_mutex_init(&sync_mutex, NULL);
	printf("Create Thread!\n");
	for(int i=0;i<4;i++)pthread_create(&threadID[i], NULL,
			threadRoutine, NULL);
	printf("Main Thread is now waiting for Child Thread!\n");
	for(int i=0;i<4;i++)pthread_join(threadID[i], NULL);
	pthread_mutex_destroy(&sync_mutex);
	printf("Main Thread finish waitng Child Thread!\na=%d\n",a);
	return 0;
}

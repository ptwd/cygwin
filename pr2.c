#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void *threadRoutine(void *argumentPointer)
{
	pthread_t id = pthread_self();
	if(*((int*)argumentPointer) == 1) sleep(1);
	else if(*((int*)argumentPointer) == 2) sleep(2);
	else sleep(3);
	printf("thread ID (TID) : %lu, argumentPointer : %d\n",
			id,*((int*)argumentPointer));
	return NULL;
}
int main()
{
	pthread_t threadID[3];
	int i,a[3];
	for(i=0;i<3;i++)a[i]=i+1;
	printf("Create Thread!\n");
	pthread_create(&threadID[0], NULL, threadRoutine, (void*)a);
	pthread_create(&threadID[1], NULL, threadRoutine,
			(void*)&a[1]);
	pthread_create(&threadID[2], NULL, threadRoutine,
			(void*)(a+2));
	// threadID를 가진 thread가 실행되는 동안 기다린다.
	printf("Main Thread is now waiting for Child Thread!\n");
	pthread_join(threadID[0], NULL);
	pthread_join(threadID[1], NULL);
	pthread_join(threadID[2], NULL);
	printf("Main Thread finish waitng Child Thread!\n");
	return 0;
}

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
void *threadRoutine(void *argumentPointer)
{
	pthread_t id = pthread_self();
	// TID를 반환하고 출력
	printf("thread ID (TID) :: %lu\n", id);
	// 부모 스레드 부분에서 리턴값을 받기때문에 항상 리턴을 해준다.
	return NULL;
}
int main()
{
	pthread_t threadID[3];
	// threadID로 TID를 받아오고, threadRoutine라는 함수 포인터로
	
		printf("Create Thread!\n");
	pthread_create(&threadID[0], NULL, threadRoutine, NULL);
	pthread_create(&threadID[1], NULL, threadRoutine, NULL);
	pthread_create(&threadID[2], NULL, threadRoutine, NULL);
	// threadID를 가진 thread가 실행되는 동안 기다린다.
	printf("Main Thread is now waiting for Child Thread!\n");
	pthread_join(threadID[0], NULL);
	pthread_join(threadID[1], NULL);
	pthread_join(threadID[2], NULL);
	printf("Main Thread finish waitng Child Thread!\n");
	return 9;
}

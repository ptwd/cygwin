#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

pthread_t making;
pthread_t printing;




//시뮬레이션으로 생성될 노드
typedef struct node {
	int start;
	int end;
	struct node* next; // 아직 typedef되지 않았으므로 NODE*라고 쓰지 못함
}NODE;

//20층으로 각각 구별해서 노드를 저장
NODE head[20];

void addFirst(NODE *target, int st, int e) // target층에 저장
{
	NODE *newNode = malloc(sizeof(NODE));
	newNode->next = target->next;
	newNode->start = st;
	newNode->end = e;
	target->next = newNode;
}

void print_list(NODE target)
{
	NODE *newNode;
	newNode = target.next;
	printf("%d층 (", target.start);

	while (newNode)
	{
		printf("%d ", newNode->end);
		newNode = newNode->next;
	}
	printf(")\n");

}



void* makingneeds(void *sth)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while(1)
	{
		int a = rand() % 20 + 1;
		int b = rand() % 19 + 1;
		if (a <= b)
		{
			b = b + 1;
		}
		addFirst(&head[a - 1], a, b);
		Sleep(100);
	}
}
void* print(void *sth)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		for (int i = 0; i < 20; i++)
		{
			print_list(head[i]);
		}
		Sleep(100);
	}
}

int main(void)
{

	srand((unsigned)time(NULL));

	//각 원소마다 기준이될 층을 입력
	for (int i = 0; i < 20; i++)
	{
		head[i].start = i + 1;
		head[i].next = NULL;
	}

	pthread_create(&making, NULL, &makingneeds, NULL);
	pthread_create(&printing, NULL, &print, NULL);

	Sleep(5000);

	pthread_cancel(making);
	pthread_cancel(printing);
	pthread_join(making, NULL);
	pthread_join(printing, NULL);

	return 0;
}

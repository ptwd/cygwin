#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

pthread_t making;
pthread_t printing;

//시뮬레이션으로 생성될 노드
typedef struct node
{
	int start;
	int end;
	struct node *next; // 아직 typedef되지 않았으므로 NODE*라고 쓰지 못함
} NODE;

//20층으로 각각 구별해서 노드를 저장
NODE nhead[20];





void addFirst(NODE *target, int st, int e) // target층에 저장
{
	NODE *newNode = malloc(sizeof(NODE));
	newNode->next = target->next;
	newNode->start = st;
	newNode->end = e;
}

void removeFirst(NODE *target) // 기준 노드의 다음 노드를 삭제하는 함수
{
	NODE *removeNode = target->next; // 기준 노드의 다음 노드 주소를 저장
	target->next = removeNode->next; // 기준 노드의 다음 노드에 삭제할 노드의 다음 노드를 지정

	free(removeNode); // 노드 메모리 해제
}



void *make(void *sth)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		int a = rand() % 20 + 1;
		int b = rand() % 19 + 1; //목적층은 출발층과 달라야함
		if (a <= b)
		{
			b = b + 1;
		}
		addFirst(&nhead[a - 1], a, b);
		Sleep(100);
	}
}
void *print(void *sth)
{
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		for (int i = 0; i < 20; i++)
		{
			print_list(nhead[i]);
		}
		Sleep(100);
	}
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

int main(void)
{

	srand((unsigned)time(NULL));

	//각 원소마다 기준이될 층을 입력
	for (int i = 0; i < 20; i++)
	{
		nhead[i].start = i + 1;
		nhead[i].next = NULL;
	}

	pthread_create(&making, NULL, &make, NULL);
	pthread_create(&printing, NULL, &print, NULL);

	Sleep(5000);

	pthread_cancel(making);
	pthread_cancel(printing);
	pthread_join(making, NULL);
	pthread_join(printing, NULL);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

pthread_t making;
pthread_t printing;
pthread_t el;
pthread_t fo;
pthread_t printel;

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;

typedef struct elevator
{
	int nfloor; //현재 층
	int ofloor; //목표 층
	int state;  //현재 상태 1 ,0,-1
} ELE;

ELE D1, D2, U1, U2, A1, A2; // 6개 선언 down, up, all 2개씩

typedef struct node //입력으로 생성될 노드
{
	int start;
	int end;
	struct node *next; // 아직 typedef되지 않았으므로 NODE*라고 쓰지 못함
} NODE;

NODE mystackD1[11];
NODE mystackD2[11];
NODE mystackU1[11];
NODE mystackU2[11];
NODE mystackA1[21];
NODE mystackA2[21];

//20층으로 각각 구별해서 노드를 저장 P는 위로가는(plus) M는 아래로가는(minus)
NODE nheadW[21];  // 전층용을 타야하는 입력을 저장
NODE nheadSP[21]; // 전층용을 사용하지 않아도 되는 입력을 저장
NODE nheadSM[21];
// 직관성을 높이기 위해 21개를 선언 (1층에 해당하는 것들을 [0]에 저장하지 않고 [1]에 저장하려고)

int align_obj(NODE *mystack) //엘리베이터의 목표 할당 함수
{
	int i, count, tempt = 1; //i와 갯수새는 count, 리턴할 temp
	int check[21] = {0, 0};  //어디에 있는지 확인하는 함수.
	for (i = 1; i < 21; i++)
	{
		NODE *temp = mystack[i].next; //temp임시로 넣음
		count += get_len(&mystack[i]);
		if (get_len(&mystack[i]) > 0)
			check[i]++;
		if (A1.state == 1)
		{
			if (tempt < temp->end)
				tempt = temp->end;
		}
		else if (A1.state == -1)
		{
			if (tempt > temp->end)
				tempt = temp->end;
		}
	}
	if (count == 1) //만약 스택에 저장된 게 1개라면
	{
		for (i = 0; i < 21; i++)
		{
			NODE *temp = mystack[i].next;
			if (check[i] == 1)	 //그 저장된 위치의 목표가
				tempt = temp->end; //엘리베이터의 목표층
		}
	}
	return tempt; // 목표값 리턴
}

void *elevatorA1(void *sth)
{
	int i;
	A1.nfloor = 1; //초기 현재층 1층
	A1.ofloor = 0; //목표층 X
	A1.state = 0;  //정지상태

	while (1)
	{ //고장!! 전역변수 설정하여 그 수가 X면 고장남.
		/*if()
		{
				
		}*/

		//random함수 이용하여 X 분의 1확률로 고장나도록 설정
		// 전역변수 X로 만듦.
		if (A1.state == 1) // state상태별로 +1층, -1층, 정지 (정지시에는 스택비교 X)
			A1.nfloor++;
		else if (A1.state == -1)
			A1.nfloor--;

		else if (A1.state == 0)
			continue;

		for (i = 1; i < 21; i++) //mystack검사해서 현재층과 같은 목표층 가진 것들 검사
		{
			NODE *temp = mystackA1[i].next;
			if (temp->end == A1.nfloor)
			{
				//print 로그 출력gotoxy() printf("A1 %d층에 도착",A1.nfloor);
				removeFirst(&mystackA1[i]); //자기스택에서 목표층에 도착한 것 제거
			}
		}

		if (A1.state == 1) //양의 방향으로 움직일 시
		{
			for (int i = 0; i < get_len(&nheadW[A1.nfloor]); i++) //
																  //현재 층에 몇개의 입력 저장되어있는지?
			{
				NODE *temp = mystackA1[i].next;
				if (nheadW[i].start < temp->end)
				{
					NODE *temp = popfromin(&nheadW[A1.nfloor]); //nheadW --> mystack에 넣음
					addNode(&mystackD1[temp->end], temp);
					//로그출력 gotoxy()
					//printf(" X->X A1에 할당");
				}
			}
		}
		else if (A1.state == -1) //음의 방향으로 움직일 시
		{
			for (int i = 0; i < get_len(&nheadW[A1.nfloor]); i++) //음의방향저장 nheadSM의
																  //현재 층에 몇개의 입력 저장되어있는지?
			{
				NODE *temp = mystackA1[i].next;
				if (temp->start > temp->end) //시작층 > 목표층 (-방향) 일시 저장
				{
					NODE *temp = popfromin(&nheadW[A1.nfloor]); //nheadW--> mystack
					addNode(&mystackD1[temp->end], temp);
					//로그출력 gotoxy()
					//printf(" X->X A1에 할당");
				}
			}
		}

		if (A1.nfloor == A1.ofloor) //목표층 도착시 그 위 혹은 아래까지스택 검사
		{							//검사하여 받아온 스택 Mystack에 저장함.

			if (A1.state == 1)
			{
				for (int i = A1.nfloor; i < 21; i++) //양의상태면 20까지
				{
					int tempt = A1.nfloor; // 최대의 현재층 임시입력
					if (!isEmpty(nheadW[i]))
					{
						NODE *temp = mystackA1[i].next;
						if (temp->start > tempt)
							tempt = nheadW[i].start; //temp에 시작점 넣음
					}
					A1.ofloor = tempt; //목표층 temp로 변경함.
									   //가면서 자연스럽게 스택에 있는 것 받아옴.
				}
			}
			else if (A1.state == -1)
			{
				for (int i = A1.nfloor; i > 1; i--)
				{
					int tempt = A1.nfloor;
					if (!isEmpty(nheadW[i]))
					{
						NODE *temp = mystackA1[i].next;
						if (temp->start < tempt)
							tempt = nheadW[i].start; //temp에 시작점 넣음
					}
					A1.ofloor = tempt; //
				}
			}
			if (A1.nfloor == A1.ofloor) //위에서 목표층 변경 안 되었을 시
			{
				for (int i = 0; i < get_len(&nheadW[A1.nfloor]); i++) //
																	  // 도착 층 검사해서 역방향
				{													  // 으로 가는 것 있다면 스택 넣음.
					NODE *temp = popfromin(&nheadW[A1.nfloor]);		  //nheadW--> mystack
					addNode(&mystackD1[temp->end], temp);
					//로그출력 gotoxy()
					//printf(" X->X A1에 할당");
				}
				if (get_len(&nheadW[A1.nfloor]) == 0) //역방향으로 가는 것도 없다면
					A1.state = 0;					  //state 0 상태로 만듦
			}
			A1.ofloor = align_obj(&mystackA1); // align_obj함수 이용하여 목표층 설정
		}
		if (A1.nfloor < A1.ofloor)
			A1.state = 1;
		else if (A1.nfloor > A1.ofloor)
			A1.state = -1;

		sleep(1); //1초 마다 움직이므로 모든 동장 1초 반복
	}			  //while문끝
}
void scanner()
{
	while (1)
	{
		int st, e
					printf("입 력 : ");
		scanf("%d %d", &st, &e);

		addFIrst(NODE *
	}
}
int main()
{
	pthread_create(&el, NULL, &elevatorA1, NULL); //엘레베이터 쓰레드 실행 X6
	pthread_create(&making, NULL, &scanner, NULL);
	printf("AAAAA\n");
	return 0;
}

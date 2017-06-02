#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>
#include <math.h>

pthread_t making;
pthread_t printing;

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

//20층으로 각각 구별해서 노드를 저장 P는 위로가는(plus) M는 아래로가는(minus)
NODE nheadW[21];  // 전층용을 타야하는 입력을 저장
NODE nheadSP[21]; // 전층용을 사용하지 않아도 되는 입력을 저장
NODE nheadSM[21];
// 직관성을 높이기 위해 21개를 선언 (1층에 해당하는 것들을 [0]에 저장하지 않고 [1]에 저장하려고)

int stop = 0;

void addFirst(NODE *target, int st, int e) // target층에 저장    출발층, 도착층을 받고 노드를 만들어서 스택에 저장
{
    NODE *newNode = malloc(sizeof(NODE));
    newNode->next = target->next;
    target->next = newNode;
    newNode->start = st;
    newNode->end = e;
    //메모리 할당 오류 체크 할까 말까?
}

void removeFirst(NODE *target) // target층의 첫번째 노드를 삭제함
{
    NODE *removeNode = target->next; // 첫번째 노드 기억
    target->next = removeNode->next; //target층이 두번째 노드를 가리키게 함

    free(removeNode); // 노드 메모리 해제
}

void addNode(NODE *target, NODE *ins) // 만들어져 있는 노드를 스택에 저장
{
    ins->next = target->next;
    target->next = ins;
}

int isEmpty(NODE *target) // 임시로 만듬
{
    return (target->next == NULL);
}

int get_len(NODE *target) // target층에 해당하는 값들의 갯수를 구함
{
    NODE *temp = target->next;
    int i = 0;
    while (temp != NULL)
    {
        i++;
        temp = temp->next;
    }
    return i;
}

NODE *popfromin(NODE *target) //특정층에서 노드(입력)를 하나 빼옴
{
    NODE *readNode = target->next;
    target->next = readNode->next;
    readNode->next = NULL;
    return readNode;
}

/*NODE elevator6[21]; // 엘레베이터 6의 목적층을 기준으로한 헤드노드들을 20개만듬(전층용)
get_len(nheadW[1]) => 1층이 출발인 스택을 검사 (전층용)
for(int i = 0; i < get_len(nheadW[1]), i++ )
{
NODE *hi = popfromin(nheadW[1]); // 하나빼옴
addNode( elevator6[hi.end], hi);
}
이런식으로 하면 엘레베이터6(전층용)이 현재층이 1층일 때 1층, 전층용에 해당하는 스택의 값들을 가져오고, 그것들을 다시 엘레베이터 스택에다가 목적지 층을 기준으로 저장
*/

//addNode( elevator6[nheadw], popfromin(nheadW(1)))

void *make(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int a, b;
    while (!stop)
    {
        int am, bm = 1;
        a = rand() % 20 + 1;
        b = rand() % 19 + 1; //시작층과 도착층이 같게하지 않기위함
        if (a <= b)          //P의 조건
        {
            b = b + 1; // 같지않게
        }
        if (a <= 10)
            am = -1;
        if (b <= 10)
            bm = -1;

        if (am * bm == 1)
        {
            if (a < b)
                addFirst(&nheadSP[a], a, b);
            else
                addFirst(&nheadSM[a], a, b);
        }
        else
            addFirst(&nheadW[a], a, b);

        Sleep(100);
    }
}

void print_list(NODE target)
{
    NODE *newNode;
    newNode = target.next;
    printf("%d층 ( ", target.start);

    while (newNode)
    {
        printf("%d ", newNode->end);
        newNode = newNode->next;
    }
    printf(")\n");
}

void *print(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (!stop)
    {
        for (int i = 1; i < 21; i++)
        {
            print_list(nheadSP[i]);
            print_list(nheadSM[i]);
            print_list(nheadWP[i]);
            print_list(nheadWM[i]);
        }
        Sleep(100);
    }
}

void *elevatormove(void *sth) // 일단 전층용 생각
{
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    A1.nfloor = 1;
    A1.ofloor = 0;
    A1.state = 0;
}

int comparisonforDS(int a)
{
    if (D1.state * (a - D1.nfloor) > 0) //ex +방향이라면 출발층이 현재층보다 높아야 태울 수 있다. -도 마찬가지
        return 1;
    if (D2.state * (a - D2.nfloor) > 0)
        return 1;
    if (A1.state * (a - A1.nfloor) > 0)
        return 1;
    if (A2.state * (a - A2.nfloor) > 0)
        return 1;
    return 0;
}
int comparisonforUS(int a)
{
    if (U1.state * (a - U1.nfloor) > 0)
        return 1;
    if (U2.state * (a - U2.nfloor) > 0)
        return 1;
    if (A1.state * (a - A1.nfloor) > 0)
        return 1;
    if (A2.state * (a - A2.nfloor) > 0)
        return 1;
    return 0;
}
int comparisonforA(int a)
{
    if (A1.state * (a - A1.nfloor) > 0)
        return 1;
    if (A2.state * (a - A2.nfloor) > 0)
        return 1;
    return 0;
}

int miniDS(a)
{
    int who = 0;
    int temp = 100; // 100이상 차이날 리는 없으니까
    if (D1.state == 0)
    {
        temp = abs(a - D1.nfloor); //절댓값
        who = 1;
    }
    if (D2.state == 0)
    {
        if (temp > abs(a - D2.nfloor))
        {
            temp = abs(a - D2.nfloor);
            who = 2;
        }
    }
    if (A1.state == 0)
    {
        if (temp > abs(a - A1.nfloor))
        {
            temp = abs(a - A1.nfloor);
            who = 3;
        }
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - A2.nfloor))
        {
            who = 4;
        }
    }
    return who;
}

int miniUS(a)
{
    int who = 0;
    int temp = 100; // 100이상 차이날 리는 없으니까
    if (U1.state == 0)
    {
        temp = abs(a - D1.nfloor); //절댓값
        who = 1;
    }
    if (U2.state == 0)
    {
        if (temp > abs(a - D2.nfloor))
        {
            temp = abs(a - D2.nfloor);
            who = 2;
        }
    }
    if (A1.state == 0)
    {
        if (temp > abs(a - A1.nfloor))
        {
            temp = abs(a - A1.nfloor);
            who = 3;
        }
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - A2.nfloor))
        {
            who = 4;
        }
    }
    return who;
}
int miniA(a)
{
    int who = 0;
    int temp = 100; // 100이상 차이날 리는 없으니까
    if (A1.state == 0)
    {
        temp = abs(a - D1.nfloor); //절댓값
        who = 1;
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - D2.nfloor))
        {
            who = 2;
        }
    }
    return who;
}


void *real3(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int a, b;
    while (!stop)
    {
        a = rand() % 20 + 1;
        b = rand() % 19 + 1; //시작층과 도착층이 같게하지 않기위함
        if (a <= b)          //P의 조건
        {
            b = b + 1; // 같지않게
        }

        if (a <= 10 && b <= 10)
        {
            if (comparsionforDS(a) != 0)
            {
                if (a < b)
                    addFirst(&nheadSP[a], a, b);
                else
                    addFirst(&nheadSM[a], a, b);
            }
            else
            {
                switch (miniDS(a))
                {
                case 0: // 대기인 상태가 없다
                    if (a < b)
                        addFirst(&nheadSP[a], a, b);
                    else
                        addFirst(&nheadSM[a], a, b);
                    break;
                case 1:
                    //D1
                    break;
                case 2:
                    //D2
                    break;
                case 3:
                    //A1
                    break;
                case 4:
                    //A2
                    break;
                }
            }
        }
        else if (a >= 10 && b >= 10)
        {
           if (comparsionforUS(a) != 0)
            {
                if (a < b)
                    addFirst(&nheadSP[a], a, b);
                else
                    addFirst(&nheadSM[a], a, b);
            }
            else
            {
                switch (miniUS(a))
                {
                case 0: // 대기인 상태가 없다
                    if (a < b)
                        addFirst(&nheadSP[a], a, b);
                    else
                        addFirst(&nheadSM[a], a, b);
                    break;
                case 1:
                    //U1
                    break;
                case 2:
                    //U2
                    break;
                case 3:
                    //A1
                    break;
                case 4:
                    //A2
                    break;
                }
            }
        }
        else
        {
            if (comparsionforA(a) != 0)
                addFirst(&nheadW[a], a, b);
            else
            {
                int ans = miniA(a);
                if (ans == 0)
                    addFirst(&nheadW[a], a, b);
                else if (ans == 1)
                    //A1
                else
                    //A2
            }
        }
        Sleep(100);
    }
}
int main(void)
{

    srand((unsigned)time(NULL));

    //각 원소마다 기준이될 층을 입력
    for (int i = 1; i <= 20; i++)
    {
        nheadSP[i].start = i;
        nheadSM[i].start = i;
        nheadWP[i].start = i;
        nheadWM[i].start = i;
    }

    pthread_create(&making, NULL, &make, NULL);
    pthread_create(&printing, NULL, &print, NULL);

    Sleep(3000); //일단 3초로 해놓음

    stop = 1; // stop변수는 쓰레드 종료하려고 넣었음... 쓰레드에있는 while문을 while(1)로 돌리면 종료가 안되서.. 그냥 전역변수로 while문이 끝나게했음.. cancel로 종료가 안되요 ㅠㅠ
    pthread_join(making, NULL);
    pthread_join(printing, NULL);
    //stop있어서 join은 필요없긴할듯? 검사해보면 조인함수 에러남

    return 0;
}
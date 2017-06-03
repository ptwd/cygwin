#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

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

NODE *popfromin(NODE *target) //특정층에서 노드(입력)를 하나 빼옴
{
    NODE *readNode = target->next;
    target->next = readNode->next;
    readNode->next = NULL;
    return readNode;
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

void *elevatorD1(void *sth) // 일단 전층용 생각
{
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    D1.nfloor = 1;
    D1.ofloor = 0;
    D1.state = 0;

    NODE mystackD1[11];

    /*for (int i = 1; i <= 20; i++)
    {
        mystack[i].end = i;
    }*/

    while (1)
    {

        if (D1.state != 0)
        {
            while (D1.nfloor == D1.ofloor)
            {
                for (int i = 0; i < get_len(&nheadSP[D1.nfloor]); i++)
                {
                    NODE *temp = popfromin(&nheadSP[D1.nfloor]);
                    addNode(&mystack[temp->end], temp);

                    if ((temp->end - D1.ofloor) * D1.state > 0)
                    {
                        D1.ofloor == temp->end;
                    }
                }
                for (int i = 0; i < get_len(mystack[D1.nfloor]); i++)
                {
                    NODE *temp = mystack[D1.nfloor]->next;
                    printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                    removeFirst(&mystack[D1.nfloor]);
                }
                D1.nfloor += D1.state;
                Sleep(1000);
            }
            D1.state == 0;
        }
    }
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

void *figureout(void *sth)
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
            if (a < b)
            {
                if (comparsionforDS(a) != 0)
                    addFirst(&nheadSP[a], a, b);
                else
                {
                    int key = miniDS(a);
                    if (key == 0)
                    {
                        addFirst(&nheadSP[a], a, b);
                    }
                    else
                    {
                        qwerqwer(key).state = 1;
                        qwerqwer(key).ofloor = b;
                        addFirst(asdfasdf(key,b), a, b);
                    }
                    switch (miniDS(a))
                    {
                    case 0: // 대기인 상태가 없다

                        addFirst(&nheadSP[a], a, b);
                        break;
                    case 1:
                        D1.state = 1;
                        D1.ofloor = b;
                        addFirst(&mystackD1[b], a, b);
                        break;
                    case 2:
                        D2.state = 1;
                        D2.ofloor = b;
                        addFirst(&mystackD2[b], a, b);
                        break;
                    case 3:
                        U1.state = 1;
                        U1.ofloor = b;
                        addFirst(&mystackU1[b], a, b);
                        break;
                    case 4:
                        U2.state = 1;
                        U2.ofloor = b;
                        addFirst(&mystackU2[b], a, b);
                        break;
                    }
                }
            }
            else
            {
                if (comparsionforDS(a) != 0)
                    addFirst(&nheadSM[a], a, b);
                else
                {
                    switch (miniDS(a))
                    {
                    case 0: // 대기인 상태가 없다

                        addFirst(&nheadSM[a], a, b);
                        break;
                    case 1:
                        D1.state break;
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

ELE qwerqwer(int i)
{
    swtich(i)
    {
    case 1:
        return D1;
    case 2:
        return D2;
    case 3:
        return U1;
    case 4:
        return U2;
    case 5:
        return A1;
    case 6:
        return A2;
    default:
        return NULL;
    }
}

NODE *asdfasdf(int i, int b)
{
    swtich(i)
    {
    case 1:
        return &mystackD1[b];
    case 2:
        return &mystackD2[b];
    case 3:
        return &mystackU1[b];
    case 4:
        return &mystackU2[b];
    case 5:
        return &mystackA1[b];
    case 6:
        return &mystackA2[b];
    default:
        return NULL;
    }
}

qwerqwer(miniDS(a)).state = 1;
qwerqwer(miniDS(a)).ofloor = b;
addFirst(&mystackD1[b], a, b);
int main(void)
{

    srand((unsigned)time(NULL));

    //각 원소마다 기준이될 층을 입력
    for (int i = 1; i <= 20; i++)
    {
        nheadSP[i].start = i;
        nheadSM[i].start = i;
        nheadW[i].start = i;
    }
    //굳이?

    pthread_create(&making, NULL, &make, NULL);
    pthread_create(&printing, NULL, &print, NULL);

    Sleep(3000); //일단 3초로 해놓음

    stop = 1; // stop변수는 쓰레드 종료하려고 넣었음... 쓰레드에있는 while문을 while(1)로 돌리면 종료가 안되서.. 그냥 전역변수로 while문이 끝나게했음.. cancel로 종료가 안되요 ㅠㅠ
    pthread_join(making, NULL);
    pthread_join(printing, NULL);
    //stop있어서 join은 필요없긴할듯? 검사해보면 조인함수 에러남

    return 0;
}

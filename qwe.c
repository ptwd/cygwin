#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

pthread_t el;
pthread_t fo;
pthread_t printel;

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t figure_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t figure_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t ele_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ele_mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ele_mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond3 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ele_mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond4 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ele_mutex5 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond5 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ele_mutex6 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ele_cond6 = PTHREAD_COND_INITIALIZER;

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

int stop = 0; //쓰레드 강제종료를 위한...ㅋㄷㅋㄷ
int a, b = 0; //입력을 저장할 곳

void addFirst(NODE *target, int st, int e);
void removeFirst(NODE *target);
NODE *popfromin(NODE *target);
void addNode(NODE *target, NODE *ins);
int isEmpty(NODE *target);
int get_len(NODE *target);
int comparisonforDS(int a);
int comparisonforUS(int a);
int comparisonforA(int a);
int miniDS(int a);
int miniUS(int a);
int miniA(int a);
ELE *qwerqwer(int i);
NODE *asdfasdf(int i, int b);
NODE *simple(int i, int a);
void *figureout(void *sth);
void *elevatorD1(void *sth);
void *elevatorD2(void *sth);
void *elevatorU1(void *sth);
void *elevatorU2(void *sth);
void *elevatorA1(void *sth);
void *elevatorA2(void *sth);
void removeAllD1();
void printbuilding();
void printmenu();
void gotoxy(int x, int y);
void *printelevator(void *sth);
void *print(void *sth);
void print_list(NODE target);
void marking(ELE *hi);

int main(void)
{
    int sc = 0;

    while (sc != -1)
    {
        system("clear");
        printbuilding();
        printmenu();
        scanf("%d", &sc);

        if (sc == 1) // 시뮬레이션 실행
        {
            pthread_create(&el, NULL, &elevatorD1, NULL); //엘레베이터함수 실행
            pthread_create(&fo, NULL, &figureout, NULL);  //입력처리함수(figure)실행
            pthread_create(&printel, NULL, &printelevator, NULL);
            printf("출발층과 도착층을 입력하세요 (1~20)\n");
            while (1)
            {
                scanf("%d %d", &a, &b);
                fflush(stdin);
                gotoxy(0, 52);
                printf("\033[J"); //입력한 숫자를 지움

                if (a >= 1 && b >= 1 && a <= 20 && b <= 20 && a != b)
                {
                    pthread_cond_signal(&figure_cond);
                }
                else if (a == -1)
                {
                    break; // 나가기
                }
                else
                {
                    printf("다시 입력하세요");
                }
            }
            pthread_cancel(printel);
            pthread_cancel(fo);
            pthread_cancel(el);
        }
        if (sc == 2)
        {
            //쓰레드들 모두 종료 (clean up handler 만들어야함)
        }
    }
    return 1;
}
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

int miniDS(int a)
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
            who = 5;
        }
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - A2.nfloor))
        {
            who = 6;
        }
    }
    return who;
}

int miniUS(int a)
{
    int who = 0;
    int temp = 100;
    if (U1.state == 0)
    {
        temp = abs(a - D1.nfloor);
        who = 3;
    }
    if (U2.state == 0)
    {
        if (temp > abs(a - D2.nfloor))
        {
            temp = abs(a - D2.nfloor);
            who = 4;
        }
    }
    if (A1.state == 0)
    {
        if (temp > abs(a - A1.nfloor))
        {
            temp = abs(a - A1.nfloor);
            who = 5;
        }
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - A2.nfloor))
        {
            who = 6;
        }
    }
    return who;
}
int miniA(int a)
{
    int who = 0;
    int temp = 100;
    if (A1.state == 0)
    {
        temp = abs(a - D1.nfloor);
        who = 5;
    }
    if (A2.state == 0)
    {
        if (temp > abs(a - D2.nfloor))
        {
            who = 6;
        }
    }
    return who;
}

ELE *qwerqwer(int i)
{
    switch (i)
    {
    case 1:
        return &D1;
    case 2:
        return &D2;
    case 3:
        return &U1;
    case 4:
        return &U2;
    case 5:
        return &A1;
    case 6:
        return &A2;
    }
}

NODE *asdfasdf(int i, int b)
{
    switch (i)
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
    }
}

NODE *simple(int i, int a)
{
    if (i == 1)
        return &nheadSP[a];
    else if (i == -1)
        return &nheadSM[a];
    else
        return &nheadW[a];
}

void *figureout(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (1)
    {
        pthread_mutex_lock(&figure_mutex);
        pthread_cond_wait(&figure_cond, &figure_mutex);
        int compa, key, UnD, WC;
        if (a <= 10 && b <= 10)
        {
            compa = comparisonforDS(a);
            key = miniDS(a);
        }
        else if (a >= 10 && b >= 10)
        {
            compa = comparisonforUS(a);
            key = miniUS(a);
        }
        else
        {
            compa = comparisonforA(a);
            key = miniA(a);
            WC = 0; //전층용
        }

        if (a < b)
        {
            UnD = 1;
            WC = 1;
        }
        else
        {
            UnD = -1;
            WC = -1;
        }

        if (compa != 0)                    //유기적으로 동작할 엘베가 있다
            addFirst(simple(WC, a), a, b); //스택에 저장

        else //유기적인 동작을 할 엘베가 없어서 대기하고있는 엘베중에서 찾는다
        {
            if (key == 0) // 대기중인 엘베가 없는경우
                addFirst(simple(WC, a), a, b);
            else //있는경우
            {
                if (qwerqwer(key)->nfloor > a)
                {
                    addFirst(simple(WC, a), a, b);
                    qwerqwer(key)->state = -1;
                    qwerqwer(key)->ofloor = a;
                }
                else if (qwerqwer(key)->nfloor < a)
                {
                    addFirst(simple(WC, a), a, b);
                    qwerqwer(key)->state = 1;
                    qwerqwer(key)->ofloor = a; // 대기중인 엘베가 현재층이 다르면 먼저 그 층에 가야함
                }                              // 움직인 그 엘레베이터에게 바로 할당할지, 아니면 스택에 그냥 보내서 확인시킬지는..
                else
                {
                    addFirst(asdfasdf(key, b), a, b); //엘레베이터에 바로 저장 (목적지 층을 기준으로)
                    qwerqwer(key)->state = UnD;
                    qwerqwer(key)->ofloor = b;
                }
                switch (key)
                {
                case 1:
                }
                pthread_cond_signal(&ele_cond1);
            }
        }
        pthread_mutex_unlock(&figure_mutex);
    }
}
void signal(int i)
{
    swtich
}

void *elevatorD1(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    D1.nfloor = 1;
    D1.ofloor = 0;
    D1.state = 0;
    //초기상태
    while (1)
    {
        pthread_mutex_lock(&ele_mutex1);
        pthread_cond_wait(&ele_cond1, &ele_mutex1);
        while (D1.state != 0)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackD1[D1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackD1[D1.nfloor].next;
                printf("\033[s");
                gotoxy(0, 44);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                removeFirst(&mystackD1[D1.nfloor]);
            }

            if (D1.nfloor == D1.ofloor)
            {
                for (int i = D1.nfloor; D1.state * (i - 5 - 5 * D1.state) <= 0; i += D1.state) // 0층은 원래없음
                {
                    if ((D1.state == 1 && !isEmpty(&nheadSP[i])) || (D1.state == -1 && !isEmpty(&nheadSM[i])))
                    {
                        check = 1;
                        D1.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    for (int i = D1.nfloor; D1.state * (i - 5 - 5 * D1.state) <= 0; i += D1.state) // 0층은 원래없음
                    {
                        if ((D1.state == 1 && !isEmpty(&nheadSP[i])) || (D1.state == -1 && !isEmpty(&nheadSM[i])))
                        {
                            check = 1;
                            D1.ofloor = i;
                            break;
                        }
                    }
                    D1.state = 0; // 대기상태로 만든다
                    break;
                }
                check = 0;
            }
            for (int i = 0; i < get_len(&nheadSP[D1.nfloor]); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(&nheadSP[D1.nfloor]);
                addNode(&mystackD1[temp->end], temp);

                if ((temp->end - D1.ofloor) * D1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    D1.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            D1.nfloor = D1.nfloor + D1.state; // ++or --
        }
        pthread_mutex_unlock(&ele_mutex1);
    }
}

void removeAllD1() // 쓰레드를 중단시킬때 실행해야될듯
{
    for (int i = 1; i < 10; i++)
    {
        NODE *temp = mystackD1[i].next;
        while (temp != NULL)
        {
            removeFirst(&mystackD1[i]);
            temp = temp->next;
        }
    }
}

void printbuilding()
{
    puts("\
                      ┌─────────┐┌─────────┐┌─────────┐┌─────────┐\n\
                      │         ││         ││         ││         │  20\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  19\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  18\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  17\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  16\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  15\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  14\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  13\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  12\n\
                      ├─────────┤├─────────┤├─────────┤├─────────┤\n\
                      │         ││         ││         ││         │  11\n\
┌─────────┐┌─────────┐└─────────┘└─────────┘├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  10\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  9\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  8\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  7\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  6\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  5\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  4\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  3\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  2\n\
├─────────┤├─────────┤                      ├─────────┤├─────────┤\n\
│         ││         │                      │         ││         │  1\n\
└─────────┘└─────────┘                      └─────────┘└─────────┘\n");
    //왼쪽모서리의 좌표 (4,40) 가로길이 11 세로길이 2
}
void printmenu()
{
    printf("로그를 위한 공간\n\n");
    puts("======= M E N U ========");
    puts("1. 시뮬레이션 실행");
    puts("2. 시뮬레이션 정지");
    puts("========================");
    puts("메뉴 선택(종료 -1): ");
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}
void marking(ELE *hi)
{
    if (hi->state == 0)
        printf("%d -", hi->nfloor);
    if (hi->state == -1)
    {
        printf("%d ▼", hi->nfloor);
    }

    if (hi->state == 1)
    {
        printf("%d ▲", hi->nfloor);
    }
}

void *printelevator(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while (1)
    {
        printf("\033[s");
        gotoxy(0, 0);
        printbuilding();
        gotoxy(4, 42 - 2 * D1.nfloor);
        marking(&D1);
        gotoxy(15, 42 - 2 * D2.nfloor);
        marking(&D2);
        gotoxy(26, 42 - 2 * U1.nfloor);
        marking(&U1);
        gotoxy(37, 42 - 2 * U2.nfloor);
        marking(&U2);
        gotoxy(48, 42 - 2 * A1.nfloor);
        marking(&A1);
        gotoxy(59, 42 - 2 * A2.nfloor);
        marking(&A2);
        printf("\033[u");
        fflush(stdout);
        Sleep(100);
    }
}

//나중에 삭제할 것들

void print_list(NODE target)
{
    NODE *newNode;
    newNode = target.next;

    while (newNode)
    {
        printf("%d ", newNode->end);
        newNode = newNode->next;
    }
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
            print_list(nheadW[i]);
        }
        Sleep(100);
    }
}
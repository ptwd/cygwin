#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <windows.h>

pthread_t el1;
pthread_t el2;
pthread_t el3;
pthread_t el4;
pthread_t el5;
pthread_t el6;
pthread_t fo;
pthread_t printel;

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

pthread_mutex_t print_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t print_mutex2 = PTHREAD_MUTEX_INITIALIZER;

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

NODE mystackD1[21];
NODE mystackD2[21];
NODE mystackU1[21];
NODE mystackU2[21];
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
void marking(ELE *hi);
void sig(int key);
void printclear(ELE *hi);
void printfunc();

int main(void)
{
    int sc = 0;

    while (sc != 2)
    {
        system("clear");
        printbuilding();
        printmenu();
        scanf("%d", &sc);

        if (sc == 1) // 시뮬레이션 실행
        {
            pthread_create(&el1, NULL, &elevatorD1, NULL); //엘레베이터함수 실행
            pthread_create(&el2, NULL, &elevatorD2, NULL); //엘레베이터함수 실행
            pthread_create(&el3, NULL, &elevatorU1, NULL); //엘레베이터함수 실행
            pthread_create(&el4, NULL, &elevatorU2, NULL); //엘레베이터함수 실행
            pthread_create(&el5, NULL, &elevatorA1, NULL); //엘레베이터함수 실행
            pthread_create(&el6, NULL, &elevatorA2, NULL); //엘레베이터함수 실행
            pthread_create(&fo, NULL, &figureout, NULL);   //입력처리함수(figure)실행
            pthread_create(&printel, NULL, &printelevator, NULL);
            fflush(stdout);
            Sleep(200);

            while (1)
            {
                int fun = 0;
                gotoxy(0, 48);
                printf("\033[J"); //입력한 숫자를 지움
                printfunc();
                scanf("%d", &fun);
                if (fun == 1)
                {
                    printf("출발층과 도착층을 입력하세요 (나가기 -1+아무 숫자)\n");
                    while (1)
                    {
                        scanf("%d %d", &a, &b);
                        fflush(stdin);
                        gotoxy(0, 56);
                        printf("\033[J"); //입력한 숫자를 지움

                        if (a >= 1 && b >= 1 && a <= 20 && b <= 20 && a != b)
                        {
                            printf("\033[s");
                            gotoxy(0, 44);
                            printf("\033[2K");
                            printf("%d층에서 %d층으로의 요청", a, b);
                            printf("\033[u");
                            fflush(stdout);

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
                }

                if (fun == 2)
                {
                    int fix = 0;
                    printf("\033[s");
                    gotoxy(0, 0);
                    printbuilding();
                    printf("\033[u");
                    fflush(stdout);
                    printf("고치려는 엘리베이터를 입력하세요 (나가기 -1)\n");
                    while (1)
                    {
                        scanf("%d", &fix);
                        fflush(stdin);
                        gotoxy(0, 56);
                        printf("\033[J"); //입력한 숫자를 지움

                        if (fix >= 1 && fix <= 6)
                        {
                            if (qwerqwer(fix)->state == 2)
                            {
                                printf("고치는 중");
                                fflush(stdout);
                                Sleep(750);
                                printf(".");
                                fflush(stdout);
                                Sleep(750);
                                printf(".");
                                fflush(stdout);
                                Sleep(750);
                                printf(".");
                                fflush(stdout);
                                Sleep(750);

                                qwerqwer(fix)->state = 0;
                                qwerqwer(fix)->nfloor = 1;
                                gotoxy(0, 56);
                                printf("\033[J");
                            }
                            else
                                printf("고장나지 않았습니다");
                        }
                        if (fix == -1)
                            break;
                    }
                }
                if (fun == 3)
                {
                    break;
                }
            }
        }
    }

    stop = 1;
    pthread_cond_signal(&figure_cond);
    pthread_cond_signal(&ele_cond1);
    pthread_cond_signal(&ele_cond2);
    pthread_cond_signal(&ele_cond3);
    pthread_cond_signal(&ele_cond4);
    pthread_cond_signal(&ele_cond5);
    pthread_cond_signal(&ele_cond6);
    pthread_join(printel, NULL);
    pthread_join(fo, NULL);
    pthread_join(el6, NULL);
    pthread_join(el5, NULL);
    pthread_join(el4, NULL);
    pthread_join(el3, NULL);
    pthread_join(el2, NULL);
    pthread_join(el1, NULL);
    //join 필요
    stop = 0;
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
        temp = abs(a - U1.nfloor);
        who = 3;
    }
    if (U2.state == 0)
    {
        if (temp > abs(a - U2.nfloor))
        {
            temp = abs(a - U2.nfloor);
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
        temp = abs(a - A1.nfloor);
        who = 5;
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
    while (!stop)
    {
        pthread_mutex_lock(&figure_mutex);
        pthread_cond_wait(&figure_cond, &figure_mutex);
        int compa, key, UnD, WC;

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
        if (a <= 10 && b <= 10)
        {
            compa = comparisonforDS(a);
            key = miniDS(a);
        }
        else if (a >= 11 && b >= 11)
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
                    printf("\033[s");
                    gotoxy(0, 45);
                    printf("\033[2K");
                    printf("엘리베이터 %d이(가) %d층에서의 요청 응답", key, a);
                    printf("\033[u");
                    fflush(stdout);

                    addFirst(asdfasdf(key, b), a, b); //엘레베이터에 바로 저장 (목적지 층을 기준으로)
                    qwerqwer(key)->state = UnD;
                    qwerqwer(key)->ofloor = b;
                }
                sig(key);
            }
        }
        pthread_mutex_unlock(&figure_mutex);
    }
}

void sig(int key)
{
    switch (key)
    {
    case 1:
        pthread_cond_signal(&ele_cond1);
        break;
    case 2:
        pthread_cond_signal(&ele_cond2);
        break;
    case 3:
        pthread_cond_signal(&ele_cond3);
        break;
    case 4:
        pthread_cond_signal(&ele_cond4);
        break;
    case 5:
        pthread_cond_signal(&ele_cond5);
        break;
    case 6:
        pthread_cond_signal(&ele_cond6);
        break;
    }
}

void *elevatorD1(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    D1.nfloor = 1;
    D1.ofloor = 0;
    D1.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex1);
        pthread_cond_wait(&ele_cond1, &ele_mutex1);
        while (D1.state != 0 && !stop && D1.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackD1[D1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackD1[D1.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackD1[D1.nfloor]);
            }

            if (D1.nfloor == D1.ofloor)
            {
                for (int i = D1.nfloor; D1.state * (i - 5 - 5 * D1.state) <= -1; i += D1.state) // 0층은 원래없음
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
                    for (int i = D1.nfloor; D1.state * (i - 5 + 5 * D1.state) >= -1; i -= D1.state) // 0층은 원래없음
                    {
                        if ((D1.state == 1 && !isEmpty(&nheadSM[i])) || (D1.state == -1 && !isEmpty(&nheadSP[i])))
                        {
                            check = 1;
                            D1.state = (-1) * D1.state;
                            D1.ofloor = i;
                            break;
                        }
                    }
                }
                if (check == 0)
                {
                    D1.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(D1.state, D1.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(D1.state, D1.nfloor));
                addNode(&mystackD1[temp->end], temp);
                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 1이(가) %d층에서의 요청 응답", D1.nfloor);
                printf("\033[u");
                fflush(stdout);

                if ((temp->end - D1.ofloor) * D1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    D1.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            D1.nfloor = D1.nfloor + D1.state; // ++or --

            if (rand() % 200 == 100)
            {
                D1.state = 2;    //에러상태; //
                D1.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                D1.ofloor = 1;
            }
        }
        pthread_mutex_unlock(&ele_mutex1);
    }
}

void *elevatorD2(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    D2.nfloor = 1;
    D2.ofloor = 0;
    D2.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex2);
        pthread_cond_wait(&ele_cond2, &ele_mutex2);
        while (D2.state != 0 &&!stop &&D2.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackD2[D2.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackD2[D2.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackD2[D2.nfloor]);
            }

            if (D2.nfloor == D2.ofloor)
            {
                for (int i = D2.nfloor; D2.state * (i - 5 - 5 * D2.state) <= 0; i += D2.state) // 0층은 원래없음
                {
                    if ((D2.state == 1 && !isEmpty(&nheadSP[i])) || (D2.state == -1 && !isEmpty(&nheadSM[i])))
                    {
                        check = 1;
                        D2.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    for (int i = D2.nfloor; D2.state * (i - 5 + 5 * D2.state) >= 0; i -= D2.state) // 0층은 원래없음
                    {
                        if ((D2.state == 1 && !isEmpty(&nheadSM[i])) || (D2.state == -1 && !isEmpty(&nheadSP[i])))
                        {
                            check = 1;
                            D2.state = (-1) * D2.state;
                            D2.ofloor = i;
                            break;
                        }
                    }
                }
                if (check == 0)
                {
                    D2.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(D2.state, D2.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(D2.state, D2.nfloor));
                addNode(&mystackD2[temp->end], temp);

                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 2이(가) %d층에서의 요청 응답", D2.nfloor);
                printf("\033[u");
                fflush(stdout);

                if ((temp->end - D2.ofloor) * D2.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    D2.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            D2.nfloor = D2.nfloor + D2.state; // ++or --
            
            if (rand() % 200 == 100)
            {
                D2.state = 2;    //에러상태; //
                D2.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                D2.ofloor = 1;
            }
            
        }
        pthread_mutex_unlock(&ele_mutex2);
    }
}

void *elevatorU1(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    U1.nfloor = 11;
    U1.ofloor = 0;
    U1.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex3);
        pthread_cond_wait(&ele_cond3, &ele_mutex3);
        while (U1.state != 0 &&!stop &&U1.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackU1[U1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackU1[U1.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackU1[U1.nfloor]);
            }

            if (U1.nfloor == U1.ofloor)
            {
                int aaf;
                if (U1.state == 1)
                    aaf = 20;
                else
                    aaf = 11;
                for (int i = U1.nfloor; U1.state * (i - aaf) <= 0; i += U1.state) // 0층은 원래없음
                {
                    if ((U1.state == 1 && !isEmpty(&nheadSP[i])) || (U1.state == -1 && !isEmpty(&nheadSM[i])))
                    {
                        check = 1;
                        U1.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    if (U1.state == 1)
                        aaf = 11;
                    else
                        aaf = 20;
                    for (int i = U1.nfloor; U1.state * (i - aaf) >= 0; i -= U1.state) // 0층은 원래없음
                    {
                        if ((U1.state == 1 && !isEmpty(&nheadSM[i])) || (U1.state == -1 && !isEmpty(&nheadSP[i])))
                        {
                            check = 1;
                            U1.state = (-1) * U1.state;
                            U1.ofloor = i;
                            break;
                        }
                    }
                }

                if (check == 0)
                {
                    U1.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(U1.state, U1.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(U1.state, U1.nfloor));
                addNode(&mystackU1[temp->end], temp);

                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 3이(가) %d층에서의 요청 응답", U1.nfloor);
                printf("\033[u");
                fflush(stdout);
                if ((temp->end - U1.ofloor) * U1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    U1.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            U1.nfloor = U1.nfloor + U1.state; // ++or --
                                              
            if (rand() % 200 == 100)
            {
                U1.state = 2;    //에러상태; //
                U1.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                U1.ofloor = 11;
            }
            
        }
        pthread_mutex_unlock(&ele_mutex3);
    }
}
void *elevatorU2(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    U2.nfloor = 11;
    U2.ofloor = 0;
    U2.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex4);
        pthread_cond_wait(&ele_cond4, &ele_mutex4);
        while (U2.state != 0 &&!stop &&U2.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackU2[U2.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackU2[U2.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackU2[U2.nfloor]);
            }

            if (U2.nfloor == U2.ofloor)
            {
                int aaf;
                if (U2.state == 1)
                    aaf = 20;
                else
                    aaf = 11;
                for (int i = U2.nfloor; U2.state * (i - aaf) <= 0; i += U2.state) // 0층은 원래없음
                {
                    if ((U2.state == 1 && !isEmpty(&nheadSP[i])) || (U2.state == -1 && !isEmpty(&nheadSM[i])))
                    {
                        check = 1;
                        U2.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    if (U1.state == 1)
                        aaf = 11;
                    else
                        aaf = 20;
                    for (int i = U2.nfloor; U2.state * (i - aaf) >= 0; i -= U2.state) // 0층은 원래없음
                    {
                        if ((U2.state == 1 && !isEmpty(&nheadSM[i])) || (U2.state == -1 && !isEmpty(&nheadSP[i])))
                        {
                            check = 1;
                            U2.state = (-1) * U2.state;
                            U2.ofloor = i;
                            break;
                        }
                    }
                }

                if (check == 0)
                {
                    U2.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(U2.state, U2.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(U2.state, U2.nfloor));
                addNode(&mystackU2[temp->end], temp);

                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 4이(가) %d층에서의 요청 응답", U2.nfloor);
                printf("\033[u");
                fflush(stdout);
                if ((temp->end - U2.ofloor) * U2.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    U2.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            U2.nfloor = U2.nfloor + U2.state; // ++or --
                                              
            if (rand() % 200 == 100)
            {
                U2.state = 2;    //에러상태; //
                U2.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                U2.ofloor = 11;
            }
            
        }
        pthread_mutex_unlock(&ele_mutex4);
    }
}

void *elevatorA1(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    A1.nfloor = 1;
    A1.ofloor = 0;
    A1.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex5);
        pthread_cond_wait(&ele_cond5, &ele_mutex5);
        while (A1.state != 0 &&!stop &&A1.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackA1[A1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackA1[A1.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackA1[A1.nfloor]);
            }

            if (A1.nfloor == A1.ofloor)
            {
                for (int i = A1.nfloor; A1.state * (i - 10 - 10 * A1.state) <= -1; i += A1.state) // 0층은 원래없음
                {
                    if ((A1.state == 1 && !isEmpty(&nheadSP[i])) || (A1.state == -1 && !isEmpty(&nheadSM[i])) || !isEmpty(&nheadW[i]))
                    {
                        check = 1;
                        A1.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    for (int i = A1.nfloor; A1.state * (i - 10 + 10 * A1.state) >= 1; i -= A1.state) // 0층은 원래없음
                    {
                        if ((A1.state == 1 && !isEmpty(&nheadSM[i])) || (A1.state == -1 && !isEmpty(&nheadSP[i])) || !isEmpty(&nheadW[i]))
                        {
                            check = 1;
                            A1.state = (-1) * A1.state;
                            A1.ofloor = i;
                            break;
                        }
                    }
                }
                if (check == 0)
                {
                    A1.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(0, A1.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(0, A1.nfloor));
                addNode(&mystackA1[temp->end], temp);

                if ((temp->end - A1.ofloor) * A1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    A1.ofloor = temp->end; // 목적지를 변경함
                }
            }
            for (int i = 0; i < get_len(simple(A1.state, A1.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(A1.state, A1.nfloor));
                addNode(&mystackA1[temp->end], temp);

                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 5이(가) %d층에서의 요청 응답", A1.nfloor);
                printf("\033[u");
                fflush(stdout);
                if ((temp->end - A1.ofloor) * A1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    A1.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            A1.nfloor = A1.nfloor + A1.state; // ++or --

            if (rand() % 200 == 100)
            {
                A1.state = 2;    //에러상태; //
                A1.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                A1.ofloor = 1;
            }
        }
        pthread_mutex_unlock(&ele_mutex5);
    }
}
void *elevatorA2(void *sth) // 일단 하층용 생각
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    A2.nfloor = 1;
    A2.ofloor = 0;
    A2.state = 0;

    srand(time(NULL));
    //초기상태
    while (!stop)
    {
        pthread_mutex_lock(&ele_mutex6);
        pthread_cond_wait(&ele_cond6, &ele_mutex6);
        while (A2.state != 0 &&!stop &&A2.state != 2)
        {
            int check = 0;
            for (int i = 0; i < get_len(&mystackA2[A2.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
            {
                NODE *temp = mystackA2[A2.nfloor].next;
                printf("\033[s");
                gotoxy(0, 46);
                printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
                printf("\033[u");
                fflush(stdout);
                removeFirst(&mystackA2[A2.nfloor]);
            }

            if (A2.nfloor == A2.ofloor)
            {
                for (int i = A2.nfloor; A2.state * (i - 10 - 10 * A2.state) <= -1; i += A2.state) // 0층은 원래없음
                {
                    if ((A2.state == 1 && !isEmpty(&nheadSP[i])) || (A2.state == -1 && !isEmpty(&nheadSM[i])) || !isEmpty(&nheadW[i]))
                    {
                        check = 1;
                        A2.ofloor = i;
                        break;
                    }
                }
                if (check == 0)
                {
                    for (int i = A2.nfloor; A2.state * (i - 10 + 10 * A2.state) >= 1; i -= A2.state) // 0층은 원래없음
                    {
                        if ((A2.state == 1 && !isEmpty(&nheadSM[i])) || (A2.state == -1 && !isEmpty(&nheadSP[i])) || !isEmpty(&nheadW[i]))
                        {
                            check = 1;
                            A2.state = (-1) * A2.state;
                            A2.ofloor = i;
                            break;
                        }
                    }
                }
                if (check == 0)
                {
                    A2.state = 0; // 대기상태로 만든다
                    break;
                }
            }
            for (int i = 0; i < get_len(simple(0, A2.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(0, A2.nfloor));
                addNode(&mystackA2[temp->end], temp);
                printf("\033[s");
                gotoxy(0, 45);
                printf("\033[2K");
                printf("엘리베이터 5이(가) %d층에서의 요청 응답", A2.nfloor);
                printf("\033[u");
                fflush(stdout);

                if ((temp->end - A2.ofloor) * A2.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    A2.ofloor = temp->end; // 목적지를 변경함
                }
            }
            for (int i = 0; i < get_len(simple(A2.state, A2.nfloor)); i++) // 현재층 스택에서 값을 모두 불러온다
            {

                NODE *temp = popfromin(simple(A2.state, A2.nfloor));
                addNode(&mystackA2[temp->end], temp);

                if ((temp->end - A2.ofloor) * A2.state > 0) // 목적지를 초과하는 목적입력이 들어오면
                {
                    A2.ofloor = temp->end; // 목적지를 변경함
                }
            }
            fflush(stdout);
            Sleep(1000);
            A2.nfloor = A2.nfloor + A2.state; // ++or --

            if (rand() % 200 == 100)
            {
                A2.state = 2;    //에러상태; //
                A2.nfloor = 100; //100층으로 고정시켜서 figrueout에서 파악하지 못하게함
                //comparisonfor함수에서 state * (a-floor )>0이면 1이되므로 -가되게하기위함
                A2.ofloor = 1;
            }
        }
        pthread_mutex_unlock(&ele_mutex6);
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
    pthread_mutex_lock(&print_mutex1);
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
     pthread_mutex_unlock(&print_mutex1);
}
void printmenu()
{
    pthread_mutex_lock(&print_mutex1);
    puts("----log----\n\n\n\n");
    puts("======= M E N U ========");
    puts("1. 시뮬레이션 실행");
    puts("2. 프로그램 종료");
    puts("========================");
    puts("메뉴 선택: ");
    pthread_mutex_unlock(&print_mutex1);
}

void printfunc()
{
    pthread_mutex_lock(&print_mutex1);
    puts("======= M E N U ========");
    puts("1. 입력하기");
    puts("2. 엘리베이터 고치기");
    puts("3. 시뮬레이션 나가기");
    puts("========================");
    puts("기능 선택: ");
    pthread_mutex_unlock(&print_mutex1);
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

    while (!stop)
    {
        printf("\033[s");
        printclear(&D1);
        printclear(&D2);
        printclear(&U1);
        printclear(&U2);
        printclear(&A1);
        printclear(&A2);
        printf("\033[u");
        fflush(stdout);
        Sleep(100);
    }
}

void printclear(ELE *hi)
{
    int k = 0;
    if (hi == &D1)
        k = 0;
    if (hi == &D2)
        k = 1;

    if (hi == &U1)
        k = 2;

    if (hi == &U2)
        k = 3;

    if (hi == &A1)
        k = 4;

    if (hi == &A2)
        k = 5;

    if (hi->state == 2)
    {
        gotoxy(4 + 11 * k, 40);
        printf("\033[31;40mERROR");
        printf("\033[0m");
    }
    else
    {
        if (hi->nfloor == 20)
        {

            gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor + 2);
            printf("      ");
        }
        else if (hi->nfloor == 1)
        {
            gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor - 2);
            printf("      ");
        }
        else
        {
            gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor - 2);
            printf("      ");

            gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor + 2);
            printf("      ");
        }
        gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor);
        printf("      ");

        gotoxy(4 + 11 * k, 42 - 2 * hi->nfloor);
        marking(hi);
    }
}

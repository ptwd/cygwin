void *real2(void *sth)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int sit, a, b;
    while (!stop) // rand()함수는 실제로 랜덤하지 않으므로 시간있으면 다른함수로
    {
        sit = rand() % 4; //case가 0이면 -- 1이면 -+ 2면 +- 3이면 ++
        a = rand() % 10 + 1;
        b = rand() % 9 + 1; //시작층과 도착층이 같게하지 않기위함

        swtich(sit)
        {
        case 0:
            if (comparisonforDS(a) == 1)
            {
                if (a <= b)
                {
                    b++;
                    addFirst(&nheadSP[a], a, b);
                }
                else
                    addFirst(&nheadSM[a], a, b);
            }
            else
            {
                switch (miniDS(a))
                {
                case 0: // 대기인 상태가 없다
                    if (a <= b)
                    {
                        b++;
                        addFirst(&nheadSP[a], a, b);
                    }
                    else
                        addFirst(&nheadSM[a], a, b);
                    break;
                case 1:
                    //D1 을 작동시킴!
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
            break; //걍 DS를 위한 함수 한번에 만들어야할듯?

        case 1:
            b += 10;
            addFirst(&nheadWP[a], a, b);
            break;
        case 2:
            a += 10;
            addFirst(&nheadWM[a], a, b);
            break;
        case 3:
            a += 10;
            b += 10;
            if (a <= b)
            {
                b++;
                addFirst(&nheadSP[a], a, b);
            }
            else
                addFirst(&nheadSM[a], a, b);
            break;
        }
        Sleep(100);
    }
}

void real()
{

    int a, b;
    while (!stop)
    {
        a = rand() % 20 + 1;
        b = rand() % 19 + 1; //시작층과 도착층이 같게하지 않기위함
        if (a <= b)          //P의 조건
        {
            b = b + 1; // 같지않게
            if (a <= 10)
            {
                if (b <= 10)
                {

                    if (comparsionforDS(a) != 0)
                        addFirst(&nheadSP[a], a, b);
                    else
                    {
                        switch (miniDS(a))
                        {
                        case 0: // 대기인 상태가 없다
                            addFirst(&nheadSP[a], a, b);
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
                else
                    addFirst(&nheadWP[a], a, b);
            }
            else
            {
                if (b >= 11)
                    addFirst(&nheadSP[a], a, b);
                else
                    addFirst(&nheadWP[a], a, b);
            }
        }
        else
        { //M의 조건
            if (a <= 10)
            {
                if (b <= 10)
                    addFirst(&nheadSM[a], a, b);
                else
                    addFirst(&nheadWM[a], a, b);
            }
            else
            {
                if (b >= 11)
                    addFirst(&nheadSM[a], a, b);
                else
                    addFirst(&nheadWM[a], a, b);
            }
        }

        Sleep(100);
    }
}

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
                    switch (miniDS(a))
                    {
                    case 0: // 대기인 상태가 없다

                        addFirst(&nheadSP[a], a, b);
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

int main(void)
{

    //각 원소마다 기준이될 층을 입력
    /*for (int i = 1; i <= 20; i++)
    {
        nheadSP[i].start = i;
        nheadSM[i].start = i;
        nheadW[i].start = i;
    }
    */
    //초기화 굳이?

    int sc;

    while (sc != -1)
    {
        system("clear");
        printmenu();
        scanf("%d", &sc);

        if (sc == 1)
        {
            system("clear");
            printbuilding(); //뼈대 출력

            pthread_create(&el, NULL, &elevatorD1, NULL);         //엘레베이터 쓰레드 실행 X6
            pthread_create(&fo, NULL, &figureout, NULL);          //입력받는 쓰레드 실행
            pthread_create(&printel, NULL, &printelevator, NULL); //6개의 엘레베이터 상황을 찍어주는 쓰레드
            //pthread_create(&printing, NULL, &print, NULL);
            for (int i = 0; i < 50; i++)
            {
                pthread_cond_signal(&thread_cond);
                printf("%d", D1.nfloor);
                fflush(stdout);
                Sleep(100);
            }
            gotoxy(0, 44);
            fflush(stdout);

            system("clear");

            stop = 1;
            pthread_cancel(el);
            printf("2");
            pthread_cancel(printel);
            printf("3");

            int rc = pthread_join(el, NULL);
            if (rc == 0)
            {
                printf("Completed join with thread1");
            }
            else
            {
                printf("ERROR; return code from pthread_join()1");
                return -1;
            }
            rc = pthread_join(printel, NULL);
            if (rc == 0)
            {
                printf("Completed join with thread2");
            }
            else
            {
                printf("ERROR; return code from pthread_join()2");
                return -1;
            }
            rc = pthread_join(fo, NULL);
            if (rc == 0)
            {
                printf("Completed join with thread2");
            }
            else
            {
                printf("ERROR; return code from pthread_join()2");
                return -1;
            }

            //pthread_cond_destroy(&thread_cond);
            fflush(stdout);
            Sleep(1000);
        }
        if (sc == 2)
        {

            //종료
        }
    }

    stop = 1; // stop변수는 쓰레드 종료하려고 넣었음... 쓰레드에있는 while문을 while(1)로 돌리면 종료가 안되서.. 그냥 전역변수로 while문이 끝나게했음.. cancel로 종료가 안되요 ㅠㅠ

    return 0;
}

pthread_mutex_lock(&mutex_lock);
pthread_cond_wait(&thread_cond, &mutex_lock);
printf("\033[s");
gotoxy(4, 42 - 2 * D1.nfloor);
printf("here");
gotoxy(15, 42 - 2 * D2.nfloor);
printf("here");
gotoxy(26, 42 - 2 * U1.nfloor);
printf("here");
gotoxy(37, 42 - 2 * U2.nfloor);
printf("here");
gotoxy(48, 42 - 2 * A1.nfloor);
printf("here");
gotoxy(59, 42 - 2 * A2.nfloor);
printf("here");
printf("\033[u");
fflush(stdout);
pthread_mutex_unlock(&mutex_lock);

while (D1.state != 0)
{
    for (int i = 0; i < get_len(&mystackD1[D1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
    {
        NODE *temp = mystackD1[D1.nfloor].next;
        printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
        removeFirst(&mystackD1[D1.nfloor]);
    }



    if (D1.nfloor == D1.ofloor)
    {
        for (int i = D1.nfloor; i <= 5 + 5 * D1.state; i += D1.state) // 0층은 원래없음
        {
            if ((D1.state == 1 && !isEmpty(&nheadSP[i])) || (D1.state == -1 && !isEmpty(&nheadSM[i])))
                D1.ofloor = i;
        }
        D1.state = 0; // 대기상태로 만든다
        break;
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

    D1.nfloor = D1.nfloor + D1.state; // ++or --
    Sleep(1000);
}

if (D1.state != 0)
{
    int dothis = 1;
    while (dothis) //while (D1.nfloor != D1.ofloor)
    {

        for (int i = 0; i < get_len(&nheadSP[D1.nfloor]); i++)
        {
            NODE *temp = popfromin(&nheadSP[D1.nfloor]);
            addNode(&mystackD1[temp->end], temp);

            if ((temp->end - D1.ofloor) * D1.state > 0) // 목적지를 초과하는 목적입력이 들어오면
            {
                D1.ofloor = temp->end; // 목적지를 변경함
            }
        }
        for (int i = 0; i < get_len(&mystackD1[D1.nfloor]); i++)
        {
            NODE *temp = mystackD1[D1.nfloor].next;
            printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
            removeFirst(&mystackD1[D1.nfloor]);
        }
        //현재층에 해당하는 입력들을 (목적지가 현재층인 입력들을) 모두 출력후 삭제
        D1.nfloor = D1.nfloor + D1.state;

        if (D1.nfloor == D1.ofloor)
        {
            for (int i = D1.nfloor; i <= 5 + 5 * D1.state; i += D1.state) // 0층은 원래없음
            {
                if ((D1.state == 1 && !isEmpty(&nheadSP[i])) || (D1.state == -1 && !isEmpty(&nheadSM[i])))
                    D1.ofloor = i;
            }
            dothis = 0; // 대기상태로 만든다
        }

        Sleep(1000);
    }

    D1.state = 0;
}


 while (D1.state != 0)
    {
        printf("정신차려\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        fflush(stdout);

        for (int i = 0; i < get_len(&mystackD1[D1.nfloor]); i++) // 현재층 엘베 스택에서 값을 모두 내보냄
        {
            NODE *temp = mystackD1[D1.nfloor].next;
            printf("출발 층이 %d 이고 도착 층이 %d인 사람이 내렸습니다", temp->start, temp->end);
            removeFirst(&mystackD1[D1.nfloor]);
        }

        if (D1.nfloor == D1.ofloor)
        {
            for (int i = D1.nfloor; i <= 5 + 5 * D1.state; i += D1.state) // 0층은 원래없음
            {
                if ((D1.state == 1 && !isEmpty(&nheadSP[i])) || (D1.state == -1 && !isEmpty(&nheadSM[i])))
                    D1.ofloor = i;
            }
            D1.state = 0; // 대기상태로 만든다
            break;
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

        D1.nfloor = D1.nfloor + D1.state; // ++or --
        Sleep(1000);
    }

            printf("\033[s");
        gotoxy(4, 42 - 2 * D1.nfloor);
        printf("     ");
        D1.nfloor = D1.nfloor + D1.state; // ++or --
        gotoxy(4, 40 - 2 * D1.nfloor);
        marking(&D1);
        printf("\033[u");
        fflush(stdout);
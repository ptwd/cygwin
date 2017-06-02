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



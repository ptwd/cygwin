#include<stdio.h>
#include<unistd.h>

struct elevator{
	int nfloor; //현재 층
	int ofloor; //목표 층
	int state; //현재 상태 1 ,0,-1
}el;

void elevator(int object); //엘리베이터 구현함수 (전체 구현 시에는 object 스택에서 받아옴)

//시간 지나는 함수

int main()
{	

	int sc_ofloor; //전체 적용시에는 전역변수로 설정
	el.nfloor=1; //현재층 1층 초기화
	el.state=0; //현재 멈춤 상태
	
	while(1)
	{
		printf(" 목표층 입력 :");
	       	scanf("%d",&sc_ofloor); //전체 적용시에는 쓰레드 사용하여
                                        //입력받고 스택에 저장

		elevator(sc_ofloor); 
	}	
	
	
	return 0;
}

void elevator(int object)
{
	
	while(1) 
	{

		if(object<el.nfloor) //목표층<현재층 시
		{
			el.state=-1;
			el.ofloor=object;
		}
		else if(object>el.nfloor) //목표층>현재층 시
		{
			el.state=1;
			el.ofloor=object;
		}
		else 
		{
			el.state=0;
		}
		
		if(el.state==1)
		{
			el.nfloor++; //1층 ++
			printf("엘리베이터는 현재 %d층 입니다.\n",el.nfloor);
		}
	
		else if(el.state==-1)
		{
			el.nfloor--; //1층 --
			printf("엘리베이터는 현재 %d층 입니다.\n",el.nfloor);

		}
		else if(el.state==0) //상태 0 (멈춤) 시 반복문 종료 (도착함)
			break;

		sleep(1); //(1초 sleep)
	}
	printf("%d 층에 도착하였습니다.\n",el.ofloor);
	el.ofloor=-1; // 목표값 없음.

}


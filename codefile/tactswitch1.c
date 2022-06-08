#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys.types.h>
#include<asm/ioctls.h>

#define clcd "/dev/clcd"

static char tactswDev[] = "/dev/tactsw";
static int tactswFd = (-1);

unsigned char tactsw_get(int tmo)	//tach스위치 클릭값
{
	unsigned char b;
	if (tmo) {
		if (tmo<0)
			tmo = ~tmo*1000;
		else
			tmo *= 1000000;
		while (tmo>0) {
			usleep(10000);
			read(tactswFd, &b, sizeof(b));
			if (b) return(b);
				tmo -= 10000;
		}
		return(-1);
	}
	else{
		read(tactswFd, &b, sizeof(b));
		return(b);
	}
}

int tact_switch_listener(){		// tact_switch값 반환
	unsigned char c;
	int selected_tact = 0;
	
	if((tactswFd = open(tactswDev, O_RDONLY)) < 0){
		perror("tact error");
		exit(-1);
	}
	
	while(1){
		c = tactsw_get(10);
		switch (c) {
			case 1: selected_tact = 1;
			case 2: selected_tact = 2;
			case 3: selected_tact = 3;
			case 4: selected_tact = 4;
			case 5: selected_tact = 5;
			case 6: selected_tact = 6;
			case 7: selected_tact = 7;
			case 8: selected_tact = 8;
			case 9: selected_tact = 9;
			case 10: selected_tact = 10;	// - 버튼으로 사용 예정
			case 11: selected_tact = 0;
			case 12: selected_tact = 11;	// 일단은 확인버튼으로 사용
		}
		return selected_tact;
	}
}

void clcd_input(char clcd_text[]){			// clcd 글자 입력 및 출력
	int clcd_d;

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0){
		printf("clcd error\n");
	}

	write(clcd_d, clcd_text, strlen(clcd_text));
	close(clcd_d);
}

int main()
{
	int user_max_input=0, user_cal_input=0, score=0, tact, count[2], trigger, i=0;
	int user_cal[10] = { 0 };
	int cycle1=1, cycle2=2, com_answer, user_answer;
	char op;
	
	clcd_input("암산을 위한 숫자의 최댓값을 입력해주세요");
	usleep(5000 * 1000);		// 5초의 대기시간
	clcd_input("최대 99까지 가능합니다");
	usleep(5000 * 1000);
	while(1){
		count[user_max_input] = tact_switch_listener();  // 숫자 입력받기

		if ((count[user_max_input] == 10 || count[user_max_input] == 11) && user_max_input == 0) {		// 숫자입력없이 바로 확인버튼을 눌렀을때 오류처리
			clcd_input("숫자를 입력해주세요!!!");
			user_max_input -= 1;
		}
		else if(count[user_max_input] == 11 && user_max_input >= 0){		// 확인 버튼 누르면 함수 탈출
			count[1] = count[0];	// 입력받은 숫자를 count[1]에 저장해서 아래서 사용
			break;
		}

		if(user_max_input==1){			// 숫자 두개(십의 자리)일때 실행
			count[0]=count*10;
			count[1]=count[0]+count[1];
			clcd_input("최댓값으로 %d를 선택하셨습니다.", count[1]);		// 최대값 화면에 출력
			break;		// 숫자 두개를 입력받았으니 반복문 탈출
			}
		}

		user_max_input+=1;
	}
	
	
	clcd_input("암산을 위해 출력할 숫자의 개수를 입력해주세요");
	usleep(5000 * 1000);
	clcd_input("1~9까지의 숫자를 누르면 다음화면으로 넘어갑니다");
	usleep(5000 * 1000);

	trigger = tact_switch_listener();

	while (1) {
		if (trigger == 0 || trigger == 10 || trigger == 11) {		// 1~9이외의 버튼을 눌렀을시 error표시
			clcd_input("error! 1~9까지의 숫자를 입력해주세요");
			trigger = tact_switch_listener()
		else
			break;
		}
	}

	clcd_input("최대값은 %d, 암산갯수는 %d입니다", count[1], trigger);		// 위에서 입력한 정보들 출력

	// 아래에 랜덤함수 기능 들어가야함



	// 아래부분은 숫자 보여주는 부분


}

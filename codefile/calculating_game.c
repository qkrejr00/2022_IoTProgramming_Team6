#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include<asm/ioctls.h>
#include<time.h>

#define clcd "/dev/clcd"

static char tactswDev[] = "/dev/tactsw";
static int tactswFd = (-1);

unsigned char tactsw_get(int tmo)	//tact����ġ Ŭ����
{
	unsigned char b;
	if (tmo) {
		if (tmo < 0)
			tmo = ~tmo * 1000;
		else
			tmo *= 1000000;
		while (tmo > 0) {
			usleep(10000);
			read(tactswFd, &b, sizeof(b));
			if (b) return(b);
			tmo -= 10000;
		}
		return(-1);
	}
	else {
		read(tactswFd, &b, sizeof(b));
		return(b);
	}
}

int tact_switch_listener() {		// tact_switch�� ��ȯ
	unsigned char c;
	int selected_tact = 0;

	if ((tactswFd = open(tactswDev, O_RDONLY)) < 0) {
		perror("tact error");
		exit(-1);
	}

	while (1) {
		c = tactsw_get(12);		// tactswitch�� �� �ο�
		switch (c) {
		case 1: selected_tact = 1; break;
		case 2: selected_tact = 2; break;
		case 3: selected_tact = 3; break;
		case 4: selected_tact = 4; break;
		case 5: selected_tact = 5; break;
		case 6: selected_tact = 6; break;
		case 7: selected_tact = 7; break;
		case 8: selected_tact = 8; break;
		case 9: selected_tact = 9; break;
		case 10: selected_tact = 10; break;	// - ��ư �� ����� ��ư
		case 11: selected_tact = 0; break;
		case 12: selected_tact = 11; break;	// Ȯ�� ��ư
		}
		return selected_tact;
	}
}

void clcd_input(char clcd_text[]) {			// clcd ���� �Է� �� ���
	int clcd_d;
	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) {
		printf("clcd error\n");
	}

	write(clcd_d, clcd_text, strlen(clcd_text));
	close(clcd_d);
}

int number;

int countLCD1(int number) {		// ���ڸ� ����ϴµ� �� �Լ�
	int clcd_d;
	char count[2];  //clcd�� ����� ���� char�� ���� 
	char clcd_text1[30] = "max : ";  //clcd�� ����� ���� ���� 

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) { printf("clcd error\n"); }

	sprintf(count, "%d", number); //������ number�� count�� ���� 
	strcat(clcd_text1, count); //clcd_text�� count�̾ ���ڿ��� ���� 

	write(clcd_d, clcd_text1, strlen(clcd_text1)); //clcd�� ���� clcd_text���
	close(clcd_d);
}

int countLCD2(int number) {		// ���ڸ� ����ϴµ� �� �Լ�
	int clcd_d;
	char max[1];
	char clcd_text2[30] = "how many num? : ";

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) { printf("clcd error\n"); }

	sprintf(max, "%d", number);
	strcat(clcd_text2, max);

	write(clcd_d, clcd_text2, strlen(clcd_text2));
	close(clcd_d);
}

int countLCD3(int number) {		// ���ڸ� ����ϴµ� �� �Լ�
	int clcd_d;
	char num[10];
	char clcd_text3[10] = " ";

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) { printf("clcd error\n"); }

	sprintf(num, "%d", number);
	strcat(clcd_text3, num);

	write(clcd_d, clcd_text3, strlen(clcd_text3));
	close(clcd_d);
}

int scoreLCD(int number) {		// ������� ������ Ȯ���ϴµ� ���� �Լ�	
	int clcd_d;
	char score[10];
	char clcd_text4[30] = "score : ";

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) { printf("clcd error\n"); }

	sprintf(score, "%d", number);
	strcat(clcd_text4, score);

	write(clcd_d, clcd_text4, strlen(clcd_text4));
	close(clcd_d);
}

int usransLCD(int number) {		// ����ڰ� ���� ����� �����ִµ� ���� �Լ�
	int clcd_d;
	char usrans[10];
	char clcd_text5[30] = "your answer : ";

	clcd_d = open(clcd, O_RDWR);
	if (clcd_d < 0) { printf("clcd error\n"); }

	sprintf(usrans, "%d", number);
	strcat(clcd_text5, usrans);

	write(clcd_d, clcd_text5, strlen(clcd_text5));
	close(clcd_d);
}

int main()
{
	int user_max_input = 0, count[2], trigger, i = 0, number;

	clcd_input("input max number");
	usleep(3000 * 1000);		// 3���� ���ð�
	clcd_input("maximum is 99");
	usleep(3000 * 1000);
	while (1) {
		count[user_max_input] = tact_switch_listener();  // ���� �Է¹ޱ�
		usleep(500000);

		if ((count[user_max_input] == 10 || count[user_max_input] == 11) && user_max_input == 0) {		// �����Է¾��� �ٷ� Ȯ�ι�ư�� �������� ����ó��
			clcd_input("input number!!!");
			user_max_input = user_max_input - 1;
		}
		else if (count[user_max_input] == 11 && user_max_input == 1) {		// Ȯ�� ��ư ������ �Լ� Ż��
			count[1] = count[0];	// �Է¹��� ���ڸ� count[1]�� �����ؼ� �Ʒ��� ���
			number = count[1];		// ���ڸ��� ���� ���
			countLCD1(number);
			usleep(2000 * 1000);
			break;
		}
		else if (count[user_max_input] == 11 && user_max_input == 2) {
			count[0] = count[0] * 10;		// ���ڸ� ���� ���
			count[1] = count[0] + count[1];
			number = count[1];
			usleep(2000 * 1000);
			countLCD1(number);
			break;
		}
		user_max_input = user_max_input + 1;
	}


	clcd_input("choose level");
	usleep(3000 * 1000);
	clcd_input("maximum is 9");
	usleep(3000 * 1000);

	trigger = tact_switch_listener();		// ������ ���� ���� �Է�

	while (1) {
		if (trigger == 0 || trigger == 10 || trigger == 11) {		// 1~9�̿��� ��ư�� �������� errorǥ��
			clcd_input("put correct num!");
			trigger = tact_switch_listener();
		}
		else
			break;
	}

	number = trigger;

	countLCD2(number);			// �Է¹��� ���� �����ֱ�
	usleep(3000 * 1000);

	clcd_input("inform input!");
	usleep(1000 * 3000);
	clcd_input("game start!");
	usleep(1000 * 3000);

	int lastscore = 0;
	int q;

	for (q = 0; q < 5; q++) {				// ������ 5�� �ݺ�

		int data[trigger];
		int op2[trigger - 1];

		srand((int)time(NULL));						// �������� �迭�� ����
		for (i = 0; i < trigger; i++) {
			data[i] = rand() % count[1] + 1;		//data[i]�� ���� ����
		}

		for (i = 0; i < trigger - 1; i++) {	
			op2[i] = rand() % 2;			// op2�� ��ȣ ����
		}

		i = 0;
		int ans = data[0];
		int clcd_d;
		int number;

		while (1) {										// �ý��ۻ� ���� ���
			if (op2[i-1] == 1 && (i-1) >= 0) {						// 1 �϶��� +
				ans = ans + data[i];
			}
			else if (op2[i-1] == 0 && (i-1) >= 0) {					// 0 �϶��� -
				ans = ans - data[i];
			}

			if (i == (trigger - 1)) {
				break;
			}
			i += 1;
		}

		i = 0;

		while (1) {	// ���� �� ��ȣ ���

			number = data[i];
			countLCD3(number);			// ������ ���� ���
			usleep(2000 * 1000);

			if (op2[i] == 1) {			// ������ ��ȣ ���
				if (i == (trigger-1)) {
					break;
				}
				clcd_input(" + ");
				usleep(2000 * 1000);
			}
			else {
				if (i == (trigger-1)) {
					break;
				}
				clcd_input(" - ");
				usleep(2000 * 1000);
			}
			i += 1;
		}

		clcd_input("input answer");
		usleep(3000 * 1000);

		int usr_ans = 0, mark=0;
		int count_input_num = 0, counting[4] = { 0 }; // counting[4] �迭ũ�Ⱑ 4�� ���� 99*9�ϸ� �ִ밪�� 891�̶� 3�ڸ��� �ִ�

		usleep(500000);

		while (1) {
			counting[count_input_num] = tact_switch_listener();  // ���� �Է¹ޱ�
			usleep(500000);

			if (counting[0] == 11) {		// �����Է¾��� �ٷ� Ȯ�ι�ư�� �������� ����ó��
				clcd_input("input number!!!");
				count_input_num = count_input_num - 1;
			}
			else if (counting[0] == 10 && mark == 0) {			// mark�� 1�϶� ����
				mark = 1;
				count_input_num = count_input_num - 1;
			}
			else if (counting[0] == 10 && mark == 1) {			// mark�� 0�϶� ���
				mark = 0;
				count_input_num = count_input_num - 1;
			}
			else if (count_input_num >= 1 && counting[count_input_num] == 10) {			// ����� ��ư
				counting[count_input_num - 1] = 0;
				count_input_num = count_input_num - 2;
			}
			else if (counting[count_input_num] == 11 && count_input_num == 1) {		// Ȯ�� ��ư ������ �Լ� Ż��
				counting[1] = counting[0];	// �Է¹��� ���ڸ� counting[1]�� �����ؼ� �Ʒ��� ���
				number = counting[1];		// ���ڸ� ���� ���
				usr_ans = number;
				if (mark == 1) {			// Ȯ�� ��ư�� �Է¹ް� mark�� 1�϶� �Է¹��� ���� ������ ��ȯ
					number = -number;
				}
				usransLCD(number);
				usleep(2000 * 1000);
				break;
			}
			else if (counting[count_input_num] == 11 && count_input_num == 2) {		// �� �ڸ� ���� ���
				counting[0] = counting[0] * 10;
				counting[1] = counting[0] + counting[1];
				number = counting[1];
				usr_ans = number;
				if (mark == 1) {
					number = -number;
				}
				usransLCD(number);
				usleep(2000 * 1000);
				break;
			}
			else if (counting[count_input_num] == 11 && count_input_num == 3) {		// �� �ڸ� ���� ���
				counting[0] = counting[0] * 100;
				counting[1] = counting[0] + (counting[1] * 10);
				counting[2] = counting[1] + counting[2];
				number = counting[2];
				usr_ans = number;
				if (mark == 1) {
					number = -number;
				}
				usransLCD(number);
				usleep(2000 * 1000);
				break;
			}

			count_input_num = count_input_num + 1;
		}

		if (mark == 1) {		// ������ �Է¹��� ���� ������ ��ȯ
			usr_ans = -usr_ans;
		}

		if (ans == usr_ans) {	
			lastscore = lastscore + 1;
			clcd_input("correct!");
			usleep(2000 * 1000);
		}
		else {
			clcd_input("wrong answer");
			usleep(2000 * 1000);
		}
	}
	number = lastscore;
	scoreLCD(number);
	usleep(2000 * 1000);

	clcd_input("FINISH");
	return 0;

}

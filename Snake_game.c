#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


#define	LEFT_END 20
#define RIGHT_END 40
#define BOTTOM_END 40
#define TOP_END 20   //�� ������ boundary ��ǥ ����
#define MAX_SNAKELEN 200 //���� �ִ� ����

int score = 0;

typedef enum dirertion {
	LEFT = 0x4B,
	RIGHT = 0x4D,
	UP=0x48,
	DOWN= 0x50
}DIRECTION; //���� �̵����� ���������� ����


typedef struct point2d {
	int x;
	int y;
}point2d; //2���� ��ǥ ����ü�� ����


typedef struct snake {

	DIRECTION snake_dir; //���� ���� �������
	int snake_len; //���� ����
	point2d snake_body[MAX_SNAKELEN]; //���� ������ ��ǥ���� ������ �迭


}snake; //���� ����ü ����




void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
} //��ǥ �̵��� �ϱ����� �Լ� (���۸� ����...)
void printBackground() {
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 208 | BACKGROUND_INTENSITY);//��׶��� �׸��� �۾� �� ����(���۸� ����..)


	gotoxy(LEFT_END, TOP_END);
	for (int i = LEFT_END; i < RIGHT_END + 1; i++) {
		printf(" ");
	}//���� ���
	for (int i = TOP_END; i < BOTTOM_END; i++) {
		gotoxy(LEFT_END, i + 1);
		printf(" ");
		gotoxy(RIGHT_END, i + 1);
		printf(" \n");
	}//�¿� �� ���
	gotoxy(LEFT_END, BOTTOM_END + 1);
	for (int i = LEFT_END; i < RIGHT_END + 1; i++) {
		printf(" ");
	}//�Ʒ��� ���
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);//��� �׸���

	printf("SCORE IS %d", score); //���� ����Ʈ
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//�� ���󺹱�
}
void print_score() {
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);
	printf("                      ");//���� ����ϴ� ��ǥ�� ���� ����Ͽ� ����ϰ� �����
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);
	printf("SCORE IS %d", score);
}//�ش� ��ǥ�� ���� ���� ���

point2d generatefood(snake snk) {
	srand((unsigned)time(NULL)); //���� �����ϱ� ���� ó��
	int boolean = 1; 
	point2d food;
	while (boolean) { // food �� ��ǥ�� ���� ����� ��ġ�� ������ ���� ���� ����
		boolean = 0;
		int x = rand() % (RIGHT_END - LEFT_END-1) + LEFT_END+1;
		int y = rand() % (BOTTOM_END - TOP_END-1) + TOP_END+1; //���� ���� �� �Ҵ�
		food.x = x;
		food.y = y;
		for (int i = 0; i < snk.snake_len; i++) {
			if (snk.snake_body[i].x == x && snk.snake_body[i].y == y) {
				boolean = 1; //����� ��ġ�� ��ǥ�� ��� ������ ���Բ� �ϴ� flag
				break;
			}
		}
		
	}


	return food; //���� ����� ��ġ�� �ʴ� ��ǥ�� �����ϰ� food ����� �ش� ��ǥ ����ü return
}
void print_food(point2d food) {
	gotoxy(food.x, food.y);
	printf("X");
}// food �� ��ǥ�� �Է¹޾� �ش� ��ǥ�� food ���


void initialize_snake(snake* snk) {//���� �ʱ�ȭ �ϴ� �Լ�
	snk->snake_len = 3 ; //���̴� 3���� �ʱ�ȭ
	snk->snake_dir = UP; //�ʱ� �̵������� ������
	snk->snake_body[0].x = (RIGHT_END + LEFT_END) / 2;
	snk->snake_body[0].y= (TOP_END + BOTTOM_END) / 2;
	snk->snake_body[1].x = (RIGHT_END + LEFT_END) / 2 ;
	snk->snake_body[1].y = (TOP_END + BOTTOM_END) / 2+1;
	snk->snake_body[2].x = (RIGHT_END + LEFT_END) / 2;
	snk->snake_body[2].y = (TOP_END + BOTTOM_END) / 2 +2;
	
}

point2d move_snake(snake* snk,DIRECTION dir) {//���� �޸� �ּҿ� �Է����� ���� ���� ���� ���ڷ� �޾Ƽ� ���� �����̴� �Լ�
	int len = snk->snake_len;
	point2d tail; //���� �κ��� ��ǥ�� ����ؼ� ���߿�  �����κп� ���� ���
	tail.x = snk->snake_body[len - 1].x;
	tail.y = snk->snake_body[len - 1].y;

	//�Է��� ���⿡ ���� ���� �̵����� �����ϴ� if��
	if ((snk->snake_dir == UP || snk->snake_dir == DOWN) && (dir == LEFT || dir == RIGHT)) snk->snake_dir = dir;
	else if((snk->snake_dir == LEFT || snk->snake_dir == RIGHT) && (dir == UP || dir == DOWN)) snk->snake_dir = dir;
	// ���� �� ��찡 �ƴϸ� �ùٸ��� ���� �Է� ����
	else snk->snake_dir = snk->snake_dir;
	
	int tmpx = snk->snake_body[0].x; //���� �Ӹ��κ� ��ǥ ����
	int tmpy = snk->snake_body[0].y;

	switch (snk->snake_dir) {//���� �̵����⿡ ���� �Ӹ� �κ� ��ǥ ��ȭ

	case UP:
		snk->snake_body[0].y -= 1; //������ �̵�
		break;
	case DOWN:
		snk->snake_body[0].y += 1;//�Ʒ����� �̵�
		break;
	case RIGHT:
		snk->snake_body[0].x += 1; //������ ���� �̵�
		break;
	case LEFT:
		snk->snake_body[0].x -= 1;//���� ���� �̵�
		break;
	}
	
	for (int i = len - 2; i >= 1; i--) {//���� �κ� ��ǥ ��ȭ
		snk->snake_body[i + 1].x = snk->snake_body[i].x;
		snk->snake_body[i + 1].y = snk->snake_body[i].y;
	}
	snk->snake_body[1].x = tmpx;
	snk->snake_body[1].y = tmpy;
	
	return tail;//������ ���� �κ� ��ȯ: �Ʒ��� print_snake �Լ����� �� return ���� ���ڷ� �޾� ������ �����κп� ������ ���!
	

}
void print_snake(snake* snk,point2d tail) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//���������� ���
	gotoxy(tail.x, tail.y); //���� move snake �Լ����� �����κ� ��ǥ�� �Է¹޾� �ش�κп� ���� ���
	printf(" ");
	for (int i = 0; i < snk->snake_len-1; i++) { //���� ����κ� ���
		gotoxy(snk->snake_body[i].x,snk->snake_body[i].y);
		printf("O");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//���� ���󺹱�
}


int check_death(snake snk) { //�׾����� check�ϴ� �Լ�
	for (int i = 1; i < snk.snake_len-1; i++){//�ڱ��� ����� �΋H������ Ȯ���ϴ� ����
		if (snk.snake_body[0].x == snk.snake_body[i].x&&snk.snake_body[0].y == snk.snake_body[i].y)
			return 1;
	}
	if (snk.snake_body[0].x == RIGHT_END || snk.snake_body[0].x == LEFT_END || snk.snake_body[0].y == TOP_END || snk.snake_body[0].y == BOTTOM_END+1)
		return 1;//���� �΋H������ Ȯ���ϴ� if��
	return -1;//���� �ʾ����� -1 return
}
int iseatfood(snake* snk,point2d food) {//���̸� ������  ������ �߰��ϰ� 1��ȯ, �ȸԾ����� -1 ��ȯ �ϴ� �Լ�
	int  len = snk->snake_len;
	if (snk->snake_body[0].x == food.x&&snk->snake_body[0].y == food.y) {//���� �Ӹ��� ������ �Ծ����� check �ϴ� if��
		 //���� �̵����⿡ ���� ������ ���� �߰����� �����ϴ� if��
		if (snk->snake_body[len - 1].x+1 == snk->snake_body[len - 2].x ) {//������ ���������� �̵��ϰ� �־��� ��� ���ʿ� �߰�
			snk->snake_body[len].x = snk->snake_body[len - 1].x - 1;
			snk->snake_body[len].y = snk->snake_body[len - 1].y;
			snk->snake_len++;
		}
		else if (snk->snake_body[len - 1].x -1 == snk->snake_body[len - 2].x) {//������ �������� �̵��ϰ� �־��� ��� �����ʿ� �߰�
			snk->snake_body[len].x = snk->snake_body[len - 1].x + 1;
			snk->snake_body[len].y = snk->snake_body[len - 1].y;
			snk->snake_len++;
		}
		else if (snk->snake_body[len - 1].y + 1 == snk->snake_body[len - 2].y) {//�Ʒ��� ���ϰ� �־��� ��� ���ʿ� �߰�
			snk->snake_body[len].x = snk->snake_body[len - 1].x ;
			snk->snake_body[len].y = snk->snake_body[len - 1].y-1;
			snk->snake_len++;
		}
		else {//���� ���ϰ� �־��� ��� �Ʒ��ʿ� �߰�
			snk->snake_body[len].x = snk->snake_body[len - 1].x;
			snk->snake_body[len].y = snk->snake_body[len - 1].y+1;
			snk->snake_len++;
		}
		return 1;//food�� �Ծ����� 1��ȯ


	}
	return -1; //�ȸԾ����� -1 ��ȯ
}




int main(void) {
	system("cls");
	gotoxy((RIGHT_END+ LEFT_END) / 2,(BOTTOM_END+TOP_END)/2);
	printf("2015170740 ������ �����\n");
	printf("����Ϸ��� �ƹ� Ű�� ��������. (���Ḧ ���ҽ� esc)\n");//�ʱ� ȭ�� ����Ʈ
	int start_or_not = 1; //������ ������� ������ �����ϴ� flag
	while (start_or_not) {
		start_or_not = 0;
		if (_getch() != 27) { //esc Ű �Է½� ����
			system("cls");
			printBackground(); //��� ���
			snake game_snake; // �� ��ü ����
			point2d food; //food ��ü ����
			

			initialize_snake(&game_snake); //�� �ʱ�ȭ
			food = generatefood(game_snake); //�ʱ� food ����
			score = 0; //�ʱ� score=0
			while (1) {//���� ������ ���� ���� ���ѷ���


				char c = 'a'; //Ű���忡�� �Է¹��� ���� �ʱ�ȭ
				if (_kbhit()) {//Ű���尡 ���ȴ��� Ȯ��
					c = _getch();
				}


				if (iseatfood(&game_snake, food) == 1) { //���̸� �Ծ����� Ȯ��
					food = generatefood(game_snake); //�Ծ��ٸ� ���ο� food ����
					score += 10; //���� +10
				}
				print_food(food); //food ���
				print_score(); //���� ���
				print_snake(&game_snake, move_snake(&game_snake, c)); //�� �� ������ ���� ���

				if (check_death(game_snake) == 1) { //�׾����� Ȯ��
					gotoxy(RIGHT_END + 10, (BOTTOM_END +TOP_END) / 2+5);
					printf("�й��ϼ̽��ϴ�! (�ٽ�: r �Է�, ������ �ƹ�Ű�� �Է�)");

					char ch = _getch();
					if (ch == 'r' || ch == 'R') start_or_not = 1;//r �� �Է¹����� ���Ǵ�
					else start_or_not = 0;//r ���� Ű �Է½� ���� 
					break;
				}

				Sleep(200); //���� �ӵ� ����



			}

		}
		
	}
	gotoxy((LEFT_END + RIGHT_END) / 2, BOTTOM_END + 5);
	printf("�����մϴ�\n");
	system("pause");//���� ����Ʈ
	return 0;
	
	
	

}
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>


#define	LEFT_END 20
#define RIGHT_END 40
#define BOTTOM_END 40
#define TOP_END 20   //뱀 게임의 boundary 좌표 정의
#define MAX_SNAKELEN 200 //뱀의 최대 길이

int score = 0;

typedef enum dirertion {
	LEFT = 0x4B,
	RIGHT = 0x4D,
	UP=0x48,
	DOWN= 0x50
}DIRECTION; //뱀의 이동방향 열거형으로 정의


typedef struct point2d {
	int x;
	int y;
}point2d; //2차원 좌표 구조체로 정의


typedef struct snake {

	DIRECTION snake_dir; //뱀의 현재 진행방향
	int snake_len; //뱀의 길이
	point2d snake_body[MAX_SNAKELEN]; //뱀의 몸통의 좌표들을 가지는 배열


}snake; //뱀의 구조체 정의




void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
} //좌표 이동을 하기위한 함수 (구글링 복붙...)
void printBackground() {
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 208 | BACKGROUND_INTENSITY);//백그라운드 그릴때 글씨 및 배경색(구글링 복붙..)


	gotoxy(LEFT_END, TOP_END);
	for (int i = LEFT_END; i < RIGHT_END + 1; i++) {
		printf(" ");
	}//윗벽 출력
	for (int i = TOP_END; i < BOTTOM_END; i++) {
		gotoxy(LEFT_END, i + 1);
		printf(" ");
		gotoxy(RIGHT_END, i + 1);
		printf(" \n");
	}//좌우 벽 출력
	gotoxy(LEFT_END, BOTTOM_END + 1);
	for (int i = LEFT_END; i < RIGHT_END + 1; i++) {
		printf(" ");
	}//아랫벽 출력
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);//배경 그리기

	printf("SCORE IS %d", score); //점수 프린트
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//색 원상복구
}
void print_score() {
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);
	printf("                      ");//점수 출력하는 좌표에 공백 출력하여 깔끔하게 만들기
	gotoxy(RIGHT_END + 10, (BOTTOM_END + TOP_END) / 2);
	printf("SCORE IS %d", score);
}//해당 좌표로 가서 점수 출력

point2d generatefood(snake snk) {
	srand((unsigned)time(NULL)); //난수 생성하기 위한 처리
	int boolean = 1; 
	point2d food;
	while (boolean) { // food 의 좌표가 뱀의 몸통과 겹치지 않을때 까지 도는 루프
		boolean = 0;
		int x = rand() % (RIGHT_END - LEFT_END-1) + LEFT_END+1;
		int y = rand() % (BOTTOM_END - TOP_END-1) + TOP_END+1; //난수 생성 및 할당
		food.x = x;
		food.y = y;
		for (int i = 0; i < snk.snake_len; i++) {
			if (snk.snake_body[i].x == x && snk.snake_body[i].y == y) {
				boolean = 1; //몸통과 겹치는 좌표면 계속 루프를 돌게끔 하는 flag
				break;
			}
		}
		
	}


	return food; //뱀의 몸통과 겹치지 않는 좌표에 랜덤하게 food 만들고 해당 좌표 구조체 return
}
void print_food(point2d food) {
	gotoxy(food.x, food.y);
	printf("X");
}// food 의 좌표를 입력받아 해당 좌표에 food 출력


void initialize_snake(snake* snk) {//뱀을 초기화 하는 함수
	snk->snake_len = 3 ; //길이는 3으로 초기화
	snk->snake_dir = UP; //초기 이동방향은 윗방향
	snk->snake_body[0].x = (RIGHT_END + LEFT_END) / 2;
	snk->snake_body[0].y= (TOP_END + BOTTOM_END) / 2;
	snk->snake_body[1].x = (RIGHT_END + LEFT_END) / 2 ;
	snk->snake_body[1].y = (TOP_END + BOTTOM_END) / 2+1;
	snk->snake_body[2].x = (RIGHT_END + LEFT_END) / 2;
	snk->snake_body[2].y = (TOP_END + BOTTOM_END) / 2 +2;
	
}

point2d move_snake(snake* snk,DIRECTION dir) {//뱀의 메모리 주소와 입력으로 받은 방향 값을 인자로 받아서 뱀을 움직이는 함수
	int len = snk->snake_len;
	point2d tail; //꼬리 부분의 좌표를 기억해서 나중에  꼬리부분에 공백 출력
	tail.x = snk->snake_body[len - 1].x;
	tail.y = snk->snake_body[len - 1].y;

	//입력한 방향에 따라 뱀의 이동방향 설정하는 if문
	if ((snk->snake_dir == UP || snk->snake_dir == DOWN) && (dir == LEFT || dir == RIGHT)) snk->snake_dir = dir;
	else if((snk->snake_dir == LEFT || snk->snake_dir == RIGHT) && (dir == UP || dir == DOWN)) snk->snake_dir = dir;
	// 위의 두 경우가 아니면 올바르지 못한 입력 방향
	else snk->snake_dir = snk->snake_dir;
	
	int tmpx = snk->snake_body[0].x; //뱀의 머리부분 좌표 저장
	int tmpy = snk->snake_body[0].y;

	switch (snk->snake_dir) {//뱀의 이동방향에 따라 머리 부분 좌표 변화

	case UP:
		snk->snake_body[0].y -= 1; //윗방향 이동
		break;
	case DOWN:
		snk->snake_body[0].y += 1;//아랫방향 이동
		break;
	case RIGHT:
		snk->snake_body[0].x += 1; //오른쪽 방향 이동
		break;
	case LEFT:
		snk->snake_body[0].x -= 1;//왼쪽 방향 이동
		break;
	}
	
	for (int i = len - 2; i >= 1; i--) {//꼬리 부분 좌표 변화
		snk->snake_body[i + 1].x = snk->snake_body[i].x;
		snk->snake_body[i + 1].y = snk->snake_body[i].y;
	}
	snk->snake_body[1].x = tmpx;
	snk->snake_body[1].y = tmpy;
	
	return tail;//없어진 꼬리 부분 반환: 아래의 print_snake 함수에서 이 return 값을 인자로 받아 없어진 꼬리부분에 공백을 출력!
	

}
void print_snake(snake* snk,point2d tail) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);//빨간색으로 출력
	gotoxy(tail.x, tail.y); //위의 move snake 함수에서 꼬리부분 좌표을 입력받아 해당부분에 공백 출력
	printf(" ");
	for (int i = 0; i < snk->snake_len-1; i++) { //뱀의 몸통부분 출력
		gotoxy(snk->snake_body[i].x,snk->snake_body[i].y);
		printf("O");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);//색깔 원상복귀
}


int check_death(snake snk) { //죽었는지 check하는 함수
	for (int i = 1; i < snk.snake_len-1; i++){//자기의 몸통과 부딫혔는지 확인하는 루프
		if (snk.snake_body[0].x == snk.snake_body[i].x&&snk.snake_body[0].y == snk.snake_body[i].y)
			return 1;
	}
	if (snk.snake_body[0].x == RIGHT_END || snk.snake_body[0].x == LEFT_END || snk.snake_body[0].y == TOP_END || snk.snake_body[0].y == BOTTOM_END+1)
		return 1;//벽과 부딫혔는지 확인하는 if문
	return -1;//죽지 않았으면 -1 return
}
int iseatfood(snake* snk,point2d food) {//먹이를 먹으면  꼬리에 추가하고 1반환, 안먹었으면 -1 반환 하는 함수
	int  len = snk->snake_len;
	if (snk->snake_body[0].x == food.x&&snk->snake_body[0].y == food.y) {//뱀의 머리가 음식을 먹었는지 check 하는 if문
		 //뱀의 이동방향에 따라 꼬리를 어디다 추가할지 결정하는 if문
		if (snk->snake_body[len - 1].x+1 == snk->snake_body[len - 2].x ) {//꼬리가 오른쪽으로 이동하고 있었을 경우 왼쪽에 추가
			snk->snake_body[len].x = snk->snake_body[len - 1].x - 1;
			snk->snake_body[len].y = snk->snake_body[len - 1].y;
			snk->snake_len++;
		}
		else if (snk->snake_body[len - 1].x -1 == snk->snake_body[len - 2].x) {//꼬리가 왼쪽으로 이동하고 있었을 경우 오른쪽에 추가
			snk->snake_body[len].x = snk->snake_body[len - 1].x + 1;
			snk->snake_body[len].y = snk->snake_body[len - 1].y;
			snk->snake_len++;
		}
		else if (snk->snake_body[len - 1].y + 1 == snk->snake_body[len - 2].y) {//아래로 향하고 있었을 경우 위쪽에 추가
			snk->snake_body[len].x = snk->snake_body[len - 1].x ;
			snk->snake_body[len].y = snk->snake_body[len - 1].y-1;
			snk->snake_len++;
		}
		else {//위로 향하고 있었을 경우 아래쪽에 추가
			snk->snake_body[len].x = snk->snake_body[len - 1].x;
			snk->snake_body[len].y = snk->snake_body[len - 1].y+1;
			snk->snake_len++;
		}
		return 1;//food를 먹었으면 1반환


	}
	return -1; //안먹었으면 -1 반환
}




int main(void) {
	system("cls");
	gotoxy((RIGHT_END+ LEFT_END) / 2,(BOTTOM_END+TOP_END)/2);
	printf("2015170740 이재현 뱀게임\n");
	printf("계속하려면 아무 키나 누르세요. (종료를 원할시 esc)\n");//초기 화면 프린트
	int start_or_not = 1; //게임을 계속할지 안할지 결정하는 flag
	while (start_or_not) {
		start_or_not = 0;
		if (_getch() != 27) { //esc 키 입력시 종료
			system("cls");
			printBackground(); //배경 출력
			snake game_snake; // 뱀 객체 생성
			point2d food; //food 객체 생성
			

			initialize_snake(&game_snake); //뱀 초기화
			food = generatefood(game_snake); //초기 food 생성
			score = 0; //초기 score=0
			while (1) {//게임 진행을 위해 도는 무한루프


				char c = 'a'; //키보드에서 입력받을 변수 초기화
				if (_kbhit()) {//키보드가 눌렸는지 확인
					c = _getch();
				}


				if (iseatfood(&game_snake, food) == 1) { //먹이를 먹었는지 확인
					food = generatefood(game_snake); //먹었다면 새로운 food 생성
					score += 10; //점수 +10
				}
				print_food(food); //food 출력
				print_score(); //점수 출력
				print_snake(&game_snake, move_snake(&game_snake, c)); //뱀 을 움직인 다음 출력

				if (check_death(game_snake) == 1) { //죽었는지 확인
					gotoxy(RIGHT_END + 10, (BOTTOM_END +TOP_END) / 2+5);
					printf("패배하셨습니다! (다시: r 입력, 끝내기 아무키나 입력)");

					char ch = _getch();
					if (ch == 'r' || ch == 'R') start_or_not = 1;//r 을 입력받으면 한판더
					else start_or_not = 0;//r 외의 키 입력시 종료 
					break;
				}

				Sleep(200); //게임 속도 조절



			}

		}
		
	}
	gotoxy((LEFT_END + RIGHT_END) / 2, BOTTOM_END + 5);
	printf("종료합니다\n");
	system("pause");//종료 프린트
	return 0;
	
	
	

}
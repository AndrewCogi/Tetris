#define _CRT_SECURE_NO_WARNINGS

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72

#define x_size 10
#define y_size 21

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<process.h>
#include<conio.h>
#include<time.h>

//coordination
typedef struct {
	int y;
	int x;
}dest;

//static var
dest coord = { 1,0 };
int next_blockNum;
int blockNum; int shape = 1;
int speed = 1000; // 블럭 내려오는 속도 제어
int first; // 처음 블럭 지우지않고 써주기만 하기 위해 사용
int score = 0; // score
int scoreCheck = 500; // for speed up

//field
int arr[y_size + 1][x_size + 1] = { 0, };

unsigned _stdcall Thread_1(void*);
unsigned _stdcall Thread_2(void*);
unsigned _stdcall Thread_3(void*);

//cursor setting

void gotoxy(int x, int y) {
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//blocks
int block1[1][4] = { 1,1,1,1 };
int block2_1[2][3] = { {1,0,0},{1,1,1} }; int block2_2[3][2] = { {1,1},{1,0},{1,0} };
int block2_3[2][3] = { {1,1,1},{0,0,1} }; int block2_4[3][2] = { {0,1},{0,1},{1,1} };
int block3_1[2][3] = { {0,0,1},{1,1,1} }; int block3_2[3][2] = { {1,0},{1,0},{1,1} };
int block3_3[2][3] = { {1,1,1},{1,0,0} }; int block3_4[3][2] = { {1,1},{0,1},{0,1} };
int block4[2][2] = { {1,1},{1,1} };
int block5_1[2][3] = { {0,1,1},{1,1,0} }; int block5_2[3][2] = { {1,0},{1,1},{0,1} };
int block6_1[2][3] = { {0,1,0},{1,1,1} }; int block6_2[3][2] = { {1,0},{1,1},{1,0} };
int block6_3[2][3] = { {1,1,1},{0,1,0} }; int block6_4[3][2] = { {0,1},{1,1},{0,1} };
int block7_1[2][3] = { {1,1,0},{0,1,1} }; int block7_2[3][2] = { {0,1},{1,1},{1,0} };

//remove cursor
void removeCursor() {//커서삭제
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

//show field
void show(int arr[y_size + 1][x_size + 1]) {
	gotoxy(1, 1);
	removeCursor();
	for (int i = 0; i < y_size + 1; i++) {
		for (int a = 0; a < x_size + 1; a++) {
			if (arr[i][a] == 0) printf("  ");
			else if (arr[i][a] == 1) printf("■");
			else if (arr[i][a] == 2) printf("▣");
		}
		printf("\n");
	}
}

//block editor
void blockEdit(dest *pos, int blockNum, int m_d, int shape) {
	//블록의 시작위치 / 블록의 종류(1~7) / Make(1) or Delete(0) / 모양의 종류(1~4)
	switch (blockNum) {
		case 1: // I
			if(shape == 1) // 모양이 1번 모양일 때
				for (int i = 0; i < 4; i++) {
					if (block1[0][i] == 1) {
						if (m_d == 1) {
							arr[pos->x][pos->y + i] = 2; // make
						}
						else {
							arr[pos->x][pos->y + i] = 0; // delete
						}
					}
				}
			else if(shape == 2) // 모양이 2번 모양일 때

				for (int i = 0; i < 4; i++) {

					if (block1[0][i] == 1) {

						if (m_d == 1) arr[pos->x + i][pos->y] = 2; // make

						else arr[pos->x + i][pos->y] = 0; // delete

					}

				}

			break;

		case 2: // J

			if(shape == 1)//모양이 1번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block2_1[i][a] == 1) {

							if(m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if(shape == 2)//모양이 2번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block2_2[i][a] == 1) {

							if(m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if(shape == 3)//모양이 3번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block2_3[i][a] == 1) {

							if(m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if(shape == 4)//모양이 4번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block2_4[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			break;

		case 3: // L

			if (shape == 1)//모양이 1번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block3_1[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 2)//모양이 2번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block3_2[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 3)//모양이 3번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block3_3[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 4)//모양이 4번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block3_4[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			break;

		case 4: // O

			for (int i = 0; i < 2; i++) { // 얜 모양이 하나야 아주편해

				for (int a = 0; a < 2; a++) {

					if (block4[i][a] == 1) {

						if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

						else arr[pos->x + i][pos->y + a] = 0; // delete

					}

				}

			}

			break;

		case 5: // N

			if(shape == 1)//모양이 1번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block5_1[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if(shape == 2)//모양이 2번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block5_2[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			break;

		case 6: // T

			if (shape == 1)//모양이 1번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block6_1[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 2)//모양이 2번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block6_2[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 3)//모양이 3번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block6_3[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 4)//모양이 4번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block6_4[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			break;

		case 7: // S

			if (shape == 1)//모양이 1번 모양일 때

				for (int i = 0; i < 2; i++) {

					for (int a = 0; a < 3; a++) {

						if (block7_1[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			else if (shape == 2)//모양이 2번 모양일 때

				for (int i = 0; i < 3; i++) {

					for (int a = 0; a < 2; a++) {

						if (block7_2[i][a] == 1) {

							if (m_d == 1) arr[pos->x + i][pos->y + a] = 2; // make

							else arr[pos->x + i][pos->y + a] = 0; // delete

						}

					}

				}

			break;

	}

}







//한 줄이 완성되었으면 없애주고 모두 한칸씩 내리는 함수

void checkPoint() {

	int cnt = 0;

	for (int i = 0; i < y_size; i++) { // 2의 갯수 세기

		for (int a = 1; a < x_size; a++) {

			if (arr[i][a] == 2) cnt++;

		}

		if (cnt == x_size - 1) {

			for(int k=1; k<10; k++) arr[i][k] = 0; // 줄 없애주기



			for (int c = i - 1; c >= 0; c--) { // 윗줄 차례로 당겨오기

				for (int v = 1; v < x_size; v++) {

					arr[c + 1][v] = arr[c][v];

					arr[c][v] = 0;

				}

			}

			//100점 추가

			score += 100; 

		}

		cnt = 0;

	}

}





//블록이 바닥에 닿았는지 확인하는 함수

int checkFloor_Block(dest* pos, int blockNum, int shape) {

	switch (blockNum) {

		case 1: 

			if (shape == 1) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 1][pos->y + 1] == 0 &&

						arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 3] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 4][pos->y] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 2: 

			if (shape == 1) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 3][pos->y] == 0 && arr[pos->x + 1][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 3) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 1][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 4) {

				if (arr[pos->x + 3][pos->y] == 0 && arr[pos->x + 3][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 3: 

			if (shape == 1) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 3][pos->y] == 0 && arr[pos->x + 3][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 3) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 1][pos->y + 1] == 0 &&

						arr[pos->x + 1][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 4) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 3][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 4: 

			if (shape == 1) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 5: 

			if (shape == 1) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 1][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 3][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 6:

			if (shape == 1) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 3][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 3) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 1][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 4) {

				if (arr[pos->x + 2][pos->y] == 0 && arr[pos->x + 3][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

		case 7: 

			if (shape == 1) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y + 2] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			else if (shape == 2) {

				if (arr[pos->x + 3][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 0; // not floor

				else return 1; // floor or contact the block

			}

			break;

	}

}



//회전 가능한지 판단해주는 함수

int checkSpin(dest* pos, int blockNum, int shape) { // 가능하면 1, 불가능하면 0

	switch (blockNum) {

		case 1: 

			if (shape == 1) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y] == 0 &&

						arr[pos->x + 3][pos->y] == 0) return 1; // possible

				else return 0; // impossible

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x][pos->y + 2] == 0 &&

						arr[pos->x][pos->y + 3] == 0) return 1; // possible

				else return 0; // impossible

			}

			break;

		case 2: 

			if (shape == 1) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 2][pos->y] == 0) return 1;

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0) return 1;

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x + 1][pos->y + 1] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 2][pos->y] == 0) return 1;

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y] == 0 &&

						arr[pos->x + 1][pos->y + 2] == 0) return 1;

				else return 0;

			}

			break;

		case 3: 

			if (shape == 1) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 2][pos->y] == 0 &&

						arr[pos->x + 2][pos->y + 1] == 0) return 1;

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x][pos->y + 2] == 0) return 1;

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x + 1][pos->y + 1] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 1;

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 1][pos->y + 2] == 0 &&

						arr[pos->x][pos->y + 2] == 0) return 1;

				else return 0;

			}

			break;

		case 4: 

			//nothing

			break;

		case 5: 

			if (shape == 1) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 1;

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x][pos->y + 2] == 0) return 1;

				else return 0;

			}

			break;

		case 6: 

			if (shape == 1) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 2][pos->y] == 0) return 1;

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x][pos->y + 2] == 0) return 1;

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y + 1] == 0) return 1;

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x + 1][pos->y + 2] == 0) return 1;

				else return 0;

			}

			break;

		case 7: 

			if (shape == 1) {

				if (arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y] == 0) return 1;

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y + 2] == 0) return 1;

				else return 0;

			}

			break;

	}

}



//블록회전함수(shape변경)

void spin(int bNum) {

	switch (bNum) { // 블록 종류에 따라 다르기 때문에 설정.

		//4가지종류

		case 2:case 3:case 6:

			if (checkSpin(&coord, blockNum, shape)) { // 회전 가능한지 체크

				blockEdit(&coord, blockNum, 0, shape);

				shape = (shape % 4) + 1;

				blockEdit(&coord, blockNum, 1, shape);

			}

			break;



			//2가지종류

		case 1:case 5:case 7:

			if (checkSpin(&coord, blockNum, shape)) { // 회전 가능한지 체크

				blockEdit(&coord, blockNum, 0, shape);

				shape = (shape % 2) + 1;

				blockEdit(&coord, blockNum, 1, shape);

			}

			break;



			//1가지 종류

		case 4:

			//nothing

			shape = 1;

			break;

	}

}





//움직일 수 있는지 확인해주는 함수

int checkMove(dest* pos, int blockNum, int shape, int l_r) { //l_r = 왼쪽움직임인지(1), 오른쪽움직임인지(0)

	switch (blockNum) {

		case 1:

			if (shape == 1) {

				if (arr[pos->x][pos->y - 1] == 0 && l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 4] == 0 && l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y - 1] == 0 && 

						arr[pos->x + 3][pos->y - 1] == 0 && l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y + 1] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						arr[pos->x + 3][pos->y + 1] == 0 && l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 2:

			if (shape == 1) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y + 3] == 0 && l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y - 1] == 0 && 

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 1] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y + 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 3] == 0 && arr[pos->x + 1][pos->y + 3] == 0 && 

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 3:

			if (shape == 1) {

				if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 3] == 0 && arr[pos->x + 1][pos->y + 3] == 0 && 

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y - 1] == 0 && 

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y + 1] == 0 && arr[pos->x + 2][pos->y + 2] == 0 && 

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 3] == 0 && arr[pos->x + 1][pos->y + 1] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y] == 0 && arr[pos->x + 2][pos->y] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 4: 

			if (shape == 1) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 5:

			if (shape == 1) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 3] == 0 && arr[pos->x + 1][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 6:

			if (shape == 1) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && 

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 3] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 1] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 3) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 3] == 0 && arr[pos->x + 1][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 4) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 2] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

		case 7:

			if (shape == 1) {

				if (arr[pos->x][pos->y - 1] == 0 && arr[pos->x + 1][pos->y] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 3] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			else if (shape == 2) {

				if (arr[pos->x][pos->y] == 0 && arr[pos->x + 1][pos->y - 1] == 0 && arr[pos->x + 2][pos->y - 1] == 0 &&

						l_r == 1) return 1; //left check

				else if (arr[pos->x][pos->y + 2] == 0 && arr[pos->x + 1][pos->y + 2] == 0 && arr[pos->x + 2][pos->y + 1] == 0 &&

						l_r == 0) return 1; //right check

				else return 0;

			}

			break;

	}

}



//블럭선택해주는 함수(랜덤)

void chooseBlock() {

	int random;

	random = rand() % 7;

	next_blockNum = random + 1;

}



//다음 블럭 보여주기

void showNextBlock(int next) {

	switch (next) {

		case 1:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("▣▣▣▣"); // make

			break;

		case 2:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("▣"); // make

			gotoxy(30, 7); printf("▣▣▣"); // make

			break;

		case 3:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("    ▣"); // make

			gotoxy(30, 7); printf("▣▣▣"); // make

			break;

		case 4:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("▣▣"); // make

			gotoxy(30, 7); printf("▣▣"); // make

			break;

		case 5:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("  ▣▣"); // make

			gotoxy(30, 7); printf("▣▣"); // make

			break;

		case 6:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("  ▣"); // make

			gotoxy(30, 7); printf("▣▣▣"); // make

			break;

		case 7:

			gotoxy(30, 6); printf("        "); // erase

			gotoxy(30, 7); printf("        "); // erase

			gotoxy(30, 6); printf("▣▣"); // make

			gotoxy(30, 7); printf("  ▣▣"); // make

			break;

	}

}



void show_data() { // 게임마다 바뀌는 변수 출력

	//show next block(30, 6)
	showNextBlock(next_blockNum);


	//score & speed
	gotoxy(27, 11);
	printf("your score : [ %d ]", score);
	gotoxy(27, 13);
	printf("speed level : [ %d ]", scoreCheck/500);

}


void show_bg() { // 바뀌지 않는, 기본적인 값 배경에 찍어주는 함수
	//
	//gotoxy(46, 4);
	//printf("Made by");
	//gotoxy(46, 5);
	//printf("charlie_cho");
	//

	//version
	gotoxy(46, 4);
	printf("ver. 1.4\n");


	//title
	gotoxy(26, 2);
	printf("▦ T E T R I S  ▦");


	//next
	gotoxy(27, 4); printf("+-- N E X T --+");
	gotoxy(27, 5); printf("|             |");
	gotoxy(27, 6); printf("|             |");
	gotoxy(27, 7); printf("|             |");
	gotoxy(27, 8); printf("|             |");
	gotoxy(27, 9); printf("+-------------+");


	//show how to play
	gotoxy(27, 16);
	printf("< M O V E >");
	gotoxy(27, 18);
	printf("↑: Block Spin  ↓: Hard Drop ");
	gotoxy(27, 20);
	printf("←: Left Move  →: Right Move ");
	gotoxy(27, 22);
	printf("X: Game Quit  V: Block Spin ");
}



void checkGameOver() {
	for (int i = 1; i < x_size - 1; i++) {
		if (arr[1][i] != 0) {
			gotoxy(3, y_size + 3);
			printf("G A M E   O V E R !");
			gotoxy(3, y_size + 6);
			printf("Exit to press X.");

			char c;
			while (1) {
				if (_kbhit()) {
					c = _getch();

					switch (c) {
						case 'x':
						case 'X':
							exit(2);
							break;
					}
				}
			}
		}
	}
}


//thread_1 (화면송출담당)
unsigned _stdcall Thread_1(void* arg) {
	while (1) {
		show(arr);
		show_data();
		Sleep(50); // 0.05초당 송출
	}
}


//thread_2 (입력반응담당)
unsigned _stdcall Thread_2(void* arg) {

	char c;
	while (1) {
		if (_kbhit()) {
			c = _getch();

			switch (c) {

				case UP: // spin
					spin(blockNum);
					break;

				case DOWN: // hard drop
					blockEdit(&coord, blockNum, 0, shape); // 원래 자리꺼 지우고, 
					// 바닥에 닿을 때 까지 내리기
					while (!checkFloor_Block(&coord, blockNum, shape)) {
						coord.x++;
					}

					blockEdit(&coord, blockNum, 1, shape); // 밑자리에 찍어주기
					//게임오버체크
					checkGameOver();
					//점수체크
					checkPoint();

					//값 초기화 & 변경
					coord.x = 0; coord.y = 1;
					shape = 1; blockNum = next_blockNum; chooseBlock();
					//다시 내려오는 블럭 찍어주기
					blockEdit(&coord, blockNum, 1, shape);
					break;

				case LEFT: // move left
					if (checkMove(&coord, blockNum, shape, 1)) {
						blockEdit(&coord, blockNum, 0, shape);
						coord.y--;
						blockEdit(&coord, blockNum, 1, shape);
					}
					break;

				case RIGHT: // move right
					if (checkMove(&coord, blockNum, shape, 0)) {
						blockEdit(&coord, blockNum, 0, shape);
						coord.y++;
						blockEdit(&coord, blockNum, 1, shape);
					}
					break;

				case 'v': // 회전
				case 'V':
					spin(blockNum);
					break;

				case 'x': // 종료
				case 'X':
					exit(1);
					break;
			}
		}

		//동기화
		Sleep(1);
	}
}


//thread_3(하향블럭조정담당)
unsigned _stdcall Thread_3(void* arg) {

	first = 1;

	while (1) {
		if (!checkFloor_Block(&coord, blockNum, shape)) {
			if (first) {
				blockEdit(&coord, blockNum, 1, shape);

				//speed up
				if (score >= scoreCheck && scoreCheck <= 10000) {
					speed -= 100; scoreCheck += 500; // check
				}
				Sleep(speed);
				first = 0;
			}

			else {
				blockEdit(&coord, blockNum, 0, shape);
				coord.x++;
				blockEdit(&coord, blockNum, 1, shape);

				//speed up
				if (score >= scoreCheck && scoreCheck <= 10000) {
					speed -= 100; scoreCheck += 500; // check
				}
				Sleep(speed);
			}
		}

		// 바닥에 닿았을 때
		else {
			// 찍어주고
			blockEdit(&coord, blockNum, 1, shape);

			// 점수체크
			checkPoint();

			//값 초기화 & 변경
			coord.x = 0; coord.y = 1;
			shape = 1; blockNum = next_blockNum; chooseBlock();
			first = 1;

			//게임오버체크
			checkGameOver();
		}
	}
}


int main(void) {
	srand(time(NULL));

	//background print
	show_bg();
	show_data();

	//깨짐방지
	Sleep(1);

	//initialization : arr
	for (int i = 0; i < y_size + 1; i++) {
		for (int a = 0; a < x_size + 1; a++) {
			if (a == 0 || a == x_size || i == y_size)
				arr[i][a] = 1;
		}
	}

	//blockNum 초기화
	chooseBlock();
	blockNum = next_blockNum;
	chooseBlock();

	_beginthreadex(NULL, 0, Thread_1, 0, 0, NULL);
	_beginthreadex(NULL, 0, Thread_2, 0, 0, NULL);
	_beginthreadex(NULL, 0, Thread_3, 0, 0, NULL);

	// start
	while (1) {}
	return 0;
}

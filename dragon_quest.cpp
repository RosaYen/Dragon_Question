#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <math.h>
#include<iostream>/**/
#include<windows.h>/**/


#define SCREEN_HEIGHT 500 //�]�w�C���������� 
#define SCREEN_WIDTH 500 //�]�w�C�������e��
#define GRID_SIDE 40 //�]�w�C����}�C���l�ƶq 
#define LEFT_MARGINE 30 //�]�w����� 
#define TOP_MARGINE 40 //�]�w�W��� 
#define FOOD_AMOUT 2 //�]�w�C�����ͭ����ƶq 
#define INIT_SPEED  80 //�]�w��l�c�s���ʳt�� 
#define MAX_QUEUE_SIZE 1600 //�]�w�W�C�j�p 

//�ŧi�e�i��V�C�|��� 
enum Direction {
	RIGHT, 
	LEFT, 
	UP, 
	DOWN 
};

//�ŧi�C�����X�{����C�|��� 
enum Object {
	EMPTY, //�ť� 
	WALL, //��λ�ê�� 
	FOOD //���� 
};

//�ŧi�c�s����`�I���c 
struct Node {
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C 
	Direction direct; //�Ӹ`�I���e�i��V 
	struct Node *next;	//���V�U�@�Ӹ`�I 
}; 

//�w�q���V�`�I���c�������ܼ� 
typedef struct Node *NodePointer;

//�w�q�y�е��c 
struct Location {
	int row;
	int col;
};

typedef struct PathNode *PathPointer;

//�w�q���|�`�I���c�A�Ψӫإ߲��ʸ��| 
struct PathNode {
	int cost; //�Z�����I���B�� 
	int steps; //�Z���ؼЪ��B�� 
	Location loc;
	PathPointer parent;
	PathPointer next;
};

void openWindow(); //�}�Ҵ������� 
void closeGame(NodePointer dragon); //�B�z�C�������޿� 
int playGame(int field[][GRID_SIDE], NodePointer dragon, NodePointer player); //�C���i���޿� 
bool IsGameOver(NodePointer dragon, NodePointer player, int field[][GRID_SIDE]); //�c�s�O�_���`(���`����G����M����ۤv����)
int showGameStartMsg(); //�C�������T�� /**/ 
int showGameOverMsg(); //�C�������T��
void showInfo(); //��ܹC��������T 
void drawGameField(int field[][GRID_SIDE]); //ø�s�C���ϰ�
void drawEdgeSquare(int row, int col, int color); //ø�s��� /**/
void drawFoodSquare(int row, int col, int color); //ø�s��� /**/ 
void drawSquare(int row, int col, int color); //ø�s���
void controlDragonDirection(int field[][GRID_SIDE], NodePointer dragon, NodePointer player); //Ū��AI��J�A�ó]�w��Ҧ��c�s�`�I 
void controlPlayerDirection(int field[][GRID_SIDE], NodePointer player, NodePointer dragon); //Ū����L��V��J�A�Ϊ�AI��J 
void moveNext(NodePointer dragon, NodePointer player); //ø�s�c�s�C�e�i�@�B���������  
void createFood(int field[][GRID_SIDE], NodePointer dragon); //���ͭ��� 
bool IsAtWall(int field[][GRID_SIDE], int row, int col); //�P�_�O�_������  
bool IsAtDragon(NodePointer dragon, int row, int col); //�P�_�O�_����c�s�ۤv������
void dragonEatFood(int field[][GRID_SIDE], NodePointer dragon); //�B�z�c�s�Y�쭹���޿� 
void playerEatFood(int field[][GRID_SIDE], NodePointer player); //�B�z���a�Y�쭹���޿� 
void addNode(NodePointer dragon); //�W�[�c�s����`�I
Location findNearestFood(int field[][GRID_SIDE], NodePointer dragon); //�M��̱��񭹪����y�� 
Direction safeDirection(int field[][GRID_SIDE], NodePointer dragon); //���o�w����V�A�u�Ҽ{�|���|���� 
Direction playersafeDirection(int field[][GRID_SIDE], NodePointer player, NodePointer dragon); //���o�w����V�A�u�Ҽ{�|���|���� 
bool IsNearDragon(NodePointer dragon, int row, int col); //�P�_�O�_����c�s�ۤv������
Location nextStepLoc(NodePointer node, Direction direct); //���o�U�@�B�y�� 

PathPointer findPath(int field[][GRID_SIDE], Location startLoc, Location goalLoc); //�M����I�����i��F�����| 
void addPathQueue(PathNode pathNode); //�N����n���X���`�I��J��C�� 
PathPointer popPathQueue(); //�Ǧ^�W�C���������A�ñN���q�W�C���R�� 
bool isPathQueueEmpty(); //�P�_�W�C�O�_���� 
void resetPathQueue(); //���]�W�C 
void sortPathQueue(); //��W�C���������i��Ƨ� 
bool IsInPathQueue(PathNode pathNode); //�P�_�Ӥ����O�_�b�W�C���� 
PathPointer buildPath(PathPointer goal); //�^�Ǩ�ؼЦ�m�����|��C 
int calcSteps(Location start, Location goal); //�p����I�����ݭn���ʪ��B�� 
bool visited(Location loc); //�P�_�O�_�Ӹ`�I�w�g���X�L 
Direction getDirectionByPath(NodePointer dragon, PathPointer path); //�q���|��ƧP�_�U�@�B��V 
Direction playergetDirectionByPath(NodePointer player, NodePointer dragon, PathPointer path);

Direction dragonAI(int field[][GRID_SIDE], NodePointer dragon, NodePointer player); //�c�sAI 
Direction playerAI(int field[][GRID_SIDE], NodePointer player, NodePointer dragon); //�i��AI 

struct PathNode pathQueue[MAX_QUEUE_SIZE]; //�ŧi�N�n���X���`�I�W�C 
int front; //queue �Ĥ@�Ӥ������e�@�Ӧ�m 
int rear; //queue �̫�@�Ӥ�������m

int speed; //�C�����ʳt�� 
int scoreSum = 0; //�������� 
int totalTime = 0; //�����C���ɶ� 
int const scorePerFood = 1; //�C�@�������i�o���� 
bool meetSelfWillDie = false; //�M�w�c�s����ۤv����O�_�|���� 
bool IFPlayAI = false; //�O�_�}��AI�Ҧ� 

// �D�{��      
int main( ){  	

	openWindow();
	showGameStartMsg();/**/
	while(TRUE){
	
		Node headPlayer = {2, 4, RIGHT, NULL}; //�]�w�i�̪�l��m�M��V 
		Node headDragon = {15, 15, RIGHT, NULL}; //�]�w�s�Y��l��m�M��V 
		NodePointer dragon = &headDragon;
		NodePointer player = &headPlayer;
		speed = INIT_SPEED;
		
		//�]�w�C�����M��ê�� 
	   	int field[GRID_SIDE][GRID_SIDE] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		   								   {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
										   {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
										   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
										   {1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1},
										   {1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1},
										   {1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1},/**/ 
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
										   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
										   {1,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1},
										   {1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,2,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
										   {1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
										   {1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1},
										   {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}; 
	   		
		//playSound("C:\\Users\\yenro\\Desktop\\MIS\\�굲\\����\\edit\\Star Wars Main Theme (Full).wav",NULL,SND_FILENAME);/**/
		char key;
		key = playGame(field, dragon, player); //�i��C��
		if (key == 'q' || key == 'Q')
			closeGame(dragon); //�p�G���a��J'q'���}�C��	
		else if (key == 's' || key == 'S')
			continue; //�p�G���a��J's' �~��C�� 		    
	}
}

//�}�Ҵ�������
void openWindow(){
	initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Hungry dragon Game");
	
}

//�B�z�C�������޿� 
void closeGame(NodePointer dragon){
	free(dragon);
	exit(0);
}

//�C���i���޿�
int playGame(int field[][GRID_SIDE], NodePointer dragon, NodePointer player) {
	
	drawGameField(field); //ø�s�C���ϰ� 
	createFood(field, dragon); //���ͲĤ@������ 
	
	while(true){
				
		int key = 0;

		controlDragonDirection(field, dragon, player); 
		controlPlayerDirection(field, player, dragon);//Ū�����a��J��V��A�ñN�s��V�]�w��U�c�s�`�I 
		moveNext(dragon, player); //�̾ڸ`�I����V�Aø�s�s���c�s��m 
		dragonEatFood(field, dragon); //�P�_�c�s�O�_���Y�쭹���A�p�G���s�W�`�I�����
		playerEatFood(field, player); //�P�_���a�O�_���Y�쭹���A�p�G���W�[���� 
		 
		showInfo(); //��ܮɶ��M���Ƹ�T 
		
		if(IsGameOver(dragon, player, field)) //�P�_�O�_�ŦX�C����������A 
			return showGameOverMsg(); //��ܹC�������T���A�õ��ݪ��a��J�ﶵ 

		//���F�Y�쭹���|���ͷs�����A�t�Τ]�H�����ͷs���� 
		srand((unsigned)time(NULL));
		if (rand() % 20 == 0)
			createFood(field, dragon);
			
		if(key != 0)
			return key;
		
		delay(speed); //�M�w���󲾰ʳt�סAspeed�V�p���ʶV�� 
		
		//Ū���D��V�䪺��L��L��J 
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			
			//�u����J�j�p�g��a, q �M s�A�t�Τ~�������ñN��J���G�ߨ�~�h���ݳB�z 
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S')
				return key; 				
			else if (key == 'a') //�M�w�O�_���ܼҦ� �A�D�n�����a�Ҧ��M�q���ޱ���AI�Ҧ� 
				IFPlayAI = !IFPlayAI;
		}
		
	}
}

//ø�s�C���ϰ�A�̾ڹC�����x�}�]�wø�s���� 
void drawGameField(int field[][GRID_SIDE]){
   	int row = 0, col = 0;
   	cleardevice(); //�M�z�ù��e�� 
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
				switch(field[row][col])	{
					case WALL: //��b�x�}�����ȬO1 
						drawEdgeSquare(row, col, YELLOW);
						break;
					case FOOD: //�����b�x�}�����ȬO2
						drawFoodSquare(row, col, GREEN);
						break;	
				}	
		   }
	}
}

//ø�s��ɤ��
void drawEdgeSquare(int row, int col, int color){/**/
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(XHATCH_FILL,color); //�]�wø�s���󪺬���ߩM�C�� /**/
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}
//ø�s�������
void drawFoodSquare(int row, int col, int color){/**/
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(BKSLASH_FILL,color); //�]�wø�s���󪺬���ߩM�C�� /**/
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}
//ø�s���
void drawSquare(int row, int col, int color){/**/
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� /**/
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}
//ø�s�c�s�C�e�i�@�B��������� 
void moveNext(NodePointer dragon, NodePointer player) {
	int currRow, currCol;
	
	while (dragon != NULL){
		currRow = dragon->row;
		currCol = dragon->col;
		
		//�̾ڸ`�I����V�ݩʡA�]�w���ʤU�@�B����m	
		switch(dragon->direct){
			case RIGHT:
				dragon->col++;
				drawSquare(dragon->row, dragon->col, LIGHTCYAN);/**/
				break;			
			case LEFT:
				dragon->col--;
				drawSquare(dragon->row, dragon->col, CYAN);/**/
				break;
			case UP:
				dragon->row--;
				drawSquare(dragon->row, dragon->col, RED);/**/
				break;			
			case DOWN:
				dragon->row++;
				drawSquare(dragon->row, dragon->col, LIGHTRED);/**/
				break;
		}

		//drawSquare(dragon->row, dragon->col, RED);/**/
		
		if (!IsAtDragon(dragon, currRow, currCol))
			drawSquare(currRow, currCol, BLACK);
			
		dragon = dragon -> next;	
	}
	
	if (player != NULL){
		currRow = player->row;
		currCol = player->col;
		
		switch(player->direct){
			case RIGHT:
				player->col++;
				break;			
			case LEFT:
				player->col--;
				break;
			case UP:
				player->row--;
				break;			
			case DOWN:
				player->row++;
				break;
		}
		drawSquare(player->row, player->col, BROWN);/**/
		drawSquare(currRow, currCol, BLACK);
	}
}

//�c�s�O�_���`(���`����G����M����ۤv����) 
bool IsGameOver(NodePointer dragon, NodePointer player, int field[][GRID_SIDE]){
	NodePointer head = dragon;
	
	//�P�_�O�_������ 
	if (IsAtWall(field, head->row, head->col))
		return true;
	if (IsAtWall(field, player->row, player->col))
		return true;
	
	
	//�P�_�O�_����c�s�ۤv 
	if(meetSelfWillDie)
		return IsAtDragon(dragon->next, head->row, head->col);
	
	//�ˬd�O�_AI�����s 
	if(player != NULL) {
		if(IsAtDragon(dragon, player->row, player->col))
			return true;
	}
	
	return false;
}

//�P�_�O�_������
bool IsAtWall(int field[][GRID_SIDE], int row, int col){
		if (field[row][col] == WALL)
			return true;
		return false;
}

//�P�_�O�_����c�s�ۤv������
bool IsAtDragon(NodePointer dragon, int row, int col){

	while(dragon != NULL){
		if (row == dragon->row && col == dragon->col)
			return true;
		dragon = dragon -> next;
	}
	return false;
}
int showGameStartMsg(){/**/
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[30] = "Hungry dragon Game!!";
	char msg2[30] = "press ENTER to start!!";
   	for(; ; i++){
	   	setcolor(i%14);
	   	settextstyle(EUROPEAN_FONT, HORIZ_DIR , 4);/**/
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(WHITE);
	   	settextstyle(EUROPEAN_FONT, HORIZ_DIR , 2);/**/
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 70, msg2);
	   	
	   	delay(200);
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);	   
		   	
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			if (key == 13) {/**/
				return key; 				
			}
		}
	}	
}
//�C�������T��
int showGameOverMsg(){
   	//cleardevice(); //�M�z�Ҧ��ù���ơA�p�G�Ʊ�u��ܰT���ɡA�������� 
	int i = 0;
	char msg1[15] = "Game Over!!";
	char msg2[40] = "press [q] to quit or [s] to restart!!";
   	for(; ; i++){
	   	setcolor(i%14);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);
	   	
	   	setcolor(WHITE);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 2);
	   	outtextxy(20, SCREEN_HEIGHT / 2 + 70, msg2);
	   	
	   	delay(200);
	   	
	   	setcolor(BLACK);
	   	settextstyle(TRIPLEX_FONT, HORIZ_DIR , 7);
	   	outtextxy(0, SCREEN_HEIGHT / 2, msg1);	   
		   	
	   	if(kbhit()) {
	   		char key;
			key = getch();	
			if (key == 'q' || key == 'Q' || key == 's' || key == 'S') {
				return key; 				
			}
		}
	}	
}

//��ܹC��������T
void showInfo(){
	totalTime += speed;
	char timeMsg[45] = " Time:";
	char scoreMsg[45] = "Score:";
	char modeMsg[20] = "";
	char optMsg1[50] = "press [q] to quit, [s] to restart or";
	char optMsg2[50] = "press [a] to change mode.";
	
	char time[10];
	char score[10];
	
	sprintf(time, "%5d", totalTime/1000);
	strcat(timeMsg, time);
	strcat(timeMsg, " sec.");
	
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1); //�]�w�r���A�����Ϋ����M�r���j�p 
   	outtextxy(0, 0, timeMsg); //�̾ڧ��п�X��r��ù�	
	
	sprintf(score, "%5d", scoreSum);
	strcat(scoreMsg, score);
	strcat(scoreMsg, " point.");

	setcolor(WHITE);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, 19, scoreMsg);
   	
   	if(IFPlayAI){
   		strcat(modeMsg, "AI Mode    ");
	}else{
   		strcat(modeMsg, "Player Mode");
	}
			
   	setcolor(LIGHTMAGENTA);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(SCREEN_HEIGHT - LEFT_MARGINE * 2, 0, modeMsg);
   	
   	setcolor(LIGHTGREEN);
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 1);
   	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE, optMsg1);
	outtextxy(0, TOP_MARGINE + (GRID_SIDE + 2) * SCREEN_HEIGHT / GRID_SIDE + 20, optMsg2);	      	
}

//Ū����L��V��J�A�ó]�w��Ҧ��c�s�`�I 
void controlPlayerDirection(int field[][GRID_SIDE], NodePointer player, NodePointer dragon) {
	
	Direction playerDirect = player -> direct;
	
	//get key code by pressing keybord
	int key;
	if(kbhit()) 
		key = getch();
		
	//decide dragon's moving direction	
	switch(key){
		case KEY_RIGHT:
			playerDirect = RIGHT;
			break;			
		case KEY_LEFT:
			playerDirect = LEFT;
			break;		
		case KEY_UP:
			playerDirect = UP;
			break;				
		case KEY_DOWN:
			playerDirect = DOWN;
			break;				
	}
		
	if (IFPlayAI)
		playerDirect = playerAI(field, player, dragon);
	
	
	player -> direct = playerDirect;
			
}


//Ū����L��V��J�A�ó]�w��Ҧ��c�s�`�I 
void controlDragonDirection(int field[][GRID_SIDE], NodePointer dragon, NodePointer player) {
		
	Direction dragonDirect = dragonAI(field, dragon, player);
	//���F�F�칳�s�@�˲��ʤ覡�A���s���ܤ�V�ɡA�ä��O��Ө���@�_���ܡA
	//�ӬO����ʦa�A�U�Ӹ`�I�v�@���ܡA��@�Ӹ`�I����s���e�i��V�ɡA
	//�|�N�ۤv�ثe����V�ǵ��U�@�ӡC 
	Direction temp;
	while(dragon != NULL){
		temp = dragon -> direct;
		dragon -> direct = dragonDirect;
		dragonDirect = temp;
		dragon = dragon -> next;
	}
}

//���ͭ���
void createFood(int field[][GRID_SIDE], NodePointer dragon){
	int row, col, i, foodAmount = FOOD_AMOUT;
	srand((unsigned)time(NULL)); //���ثe�t�ήɶ��@���üƺؤl 

	for(i=0; i< foodAmount; i++){
		//�p�G�üƲ��ͪ���m�O�b��M�s���魫�|�A�h���s���͡A����ŦX���󬰤� 
		do{
			row = rand() % GRID_SIDE;
			col = rand() % GRID_SIDE;	
		}while(IsAtWall(field, row, col) || IsAtDragon(dragon, row, col));
	
		field[row][col] = FOOD;
		drawFoodSquare(row,col, GREEN);/**/			
	}
}

//�B�z�c�s�Y�쭹���޿�
void dragonEatFood(int field[][GRID_SIDE], NodePointer dragon){
	//�p�G�s�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[dragon->row][dragon->col] == FOOD){
		field[dragon->row][dragon->col] = EMPTY; //�N�ӭ����M�� 
		printf("The dragon has eaten food at row: %d, col: %d\n", dragon->row, dragon->col);
		addNode(dragon); //�W�[�`�I  
		createFood(field, dragon); //���ͷs������ 
	}
}

//�B�z���a�Y�쭹���޿�
void playerEatFood(int field[][GRID_SIDE], NodePointer player){
	//�p�G�s�Y�P������m���|�A�N�O�Y�쭹�� 
	if(field[player->row][player->col] == FOOD){
		field[player->row][player->col] = EMPTY; //�N�ӭ����M�� 
		printf("The player has eaten food at row: %d, col: %d\n", player->row, player->col);
		scoreSum = scoreSum + scorePerFood; //�������� 
		createFood(field, player); //���ͷs������
		Beep(523,50);/**/
		Beep(1060,100);/**/ 
	}
}

//�W�[�c�s����`�I
void addNode(NodePointer dragon){
	NodePointer tail = NULL;
	NodePointer newNode = (NodePointer) malloc(sizeof(Node)); //��l�Ƥ@�ӷs�`�I 
	//�M���s������ 
	while(dragon != NULL){
		tail = dragon;
		dragon = dragon -> next;
	}
	//�N�s���ڪ���V�ݩʵ��s�`�I 
	newNode -> direct = tail -> direct;
	
	//�M�w�s�`�I����m�A�̾ڧ��ڪ����ʤ�V�ӱ��W�s�`�I 
	switch(tail->direct){
		case RIGHT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col - 1;
			break;			
		case LEFT:
			newNode -> row = tail -> row;
			newNode -> col = tail -> col + 1;			
			break;
		case UP:
			newNode -> row = tail -> row + 1;
			newNode -> col = tail -> col;			
			break;
		case DOWN:
			newNode -> row = tail -> row - 1;
			newNode -> col = tail -> col;			
			break;
	}
	
	tail -> next = newNode; //�N���ڸ`�I�s����s�`�I 
	newNode -> next = NULL;		
}

//�s��AI���� 
Direction dragonAI(int field[][GRID_SIDE], NodePointer dragon, NodePointer player) {
	Direction dragonDirect = dragon -> direct;
	Location start = {dragon -> row, dragon -> col};
	Location nearestFood = findNearestFood(field, dragon);
	if(nearestFood.row != -1 && nearestFood.col != -1){ 
		PathPointer path = findPath(field, start, nearestFood);
		if(path != NULL)
			dragonDirect = getDirectionByPath(dragon, path);
		else
			dragonDirect = safeDirection(field, dragon);
	}
	return dragonDirect;
}

//�q���|��ƧP�_�U�@�B��V 
Direction getDirectionByPath(NodePointer head, PathPointer path){
	if(head != NULL && path != NULL){
		PathPointer nextPath = path->next;
		int horizontal = nextPath->loc.col - head->col;
		int vertical = nextPath->loc.row - head->row;
		if(horizontal == 1)
			return RIGHT;
		else if(horizontal == -1)
			return LEFT;
		
		if(vertical == 1)
			return DOWN;
		else if(vertical == -1)
			return UP;
	}
	//�p�G�L�k���`�B�z�A�~��ӭ쥻��V�e�i 
	return 	head -> direct;		
}

//���o�w����V�A�u�Ҽ{�|���|����
Direction safeDirection(int field[][GRID_SIDE], NodePointer dragon){
	int i;
	int dirSize = 4;
	Location loc;
	
	loc = nextStepLoc(dragon, UP);
	if(!IsAtWall(field, loc.row, loc.col))
		return UP;
	loc = nextStepLoc(dragon, DOWN);
	if(!IsAtWall(field, loc.row, loc.col))
		return DOWN;
	loc = nextStepLoc(dragon, RIGHT);
	if(!IsAtWall(field, loc.row, loc.col))
		return RIGHT;
	loc = nextStepLoc(dragon, LEFT);
	if(!IsAtWall(field, loc.row, loc.col))
		return LEFT;						
	return dragon->direct;
}

//���o�U�@�B�y�� 
Location nextStepLoc(NodePointer node, Direction direct){
	int currRow =  node -> row;
	int currCol = node -> col;
	int nextRow, nextCol;
	Location loc;
	switch(direct){
		case RIGHT:
			nextRow = currRow;
			nextCol = currCol + 1;
			break;	
		case LEFT:
			nextRow = currRow;
			nextCol = currCol - 1;
			break;
		case UP:
			nextRow = currRow - 1;
			nextCol = currCol;
			break;				
		case DOWN:
			nextRow = currRow + 1;
			nextCol = currCol;		
			break;	
	}
	loc.row = nextRow;
	loc.col = nextCol;
	return loc;
}

//�M����I�����i��F���̵u���| 
PathPointer findPath(int field[][GRID_SIDE], Location startLoc, Location goalLoc){
	resetPathQueue();
	int steps = calcSteps(startLoc, goalLoc);
	PathNode start = {0, steps, startLoc, NULL, NULL};
	addPathQueue(start);
	while(!isPathQueueEmpty()){
		sortPathQueue();
		PathPointer current = popPathQueue();
		if(current == NULL)
			return NULL;
		if(current->loc.row == goalLoc.row && current->loc.col == goalLoc.col)
			return buildPath(current);
		int dirSize = 4;
		int iDir[] = {1, 0, -1, 0};
		int jDir[] = {0, 1, 0, -1};
		int i,j;
		for(i=0, j=0; i<dirSize; i++, j++){
			Location neighborLoc = {current->loc.row + iDir[i], current->loc.col + jDir[j]};
			if(!visited(neighborLoc) && !IsAtWall(field, neighborLoc.row, neighborLoc.col)){
				int steps = calcSteps(neighborLoc, goalLoc);
				int cost = 	current->cost + 1;
				PathNode neighbor = {cost, steps, neighborLoc, current, NULL};
				if(!IsInPathQueue(neighbor)){
					addPathQueue(neighbor);
				}				
			}
		}
	}
	return NULL;
}

//�N����n���X���`�I��J��C��
void addPathQueue(PathNode pathNode){
	if(rear == MAX_QUEUE_SIZE - 1){
		printf("the queue is full");
		return;
	}
	rear += 1;
	pathQueue[rear]	= pathNode;
}

//�Ǧ^�W�C���������A�ñN���q�W�C���R��
PathPointer popPathQueue(){
	if(front == rear){
		printf("the queue is empty");
		return NULL;
	}
	front ++;
	PathPointer node = (PathPointer)malloc(sizeof(PathNode));
	node->cost = pathQueue[front].cost;
	node->steps = pathQueue[front].steps;
	node->loc = pathQueue[front].loc;
	node->parent = pathQueue[front].parent;
	node->next = pathQueue[front].next;
	return node;
}

//�P�_�W�C�O�_����
bool isPathQueueEmpty(){
	return front == rear;
}

//���]�W�C 
void resetPathQueue(){
	front = -1;
	rear = -1;	
}

//��W�C���������i��Ƨ� 
void sortPathQueue(){
	if(front == rear)
		return;
	int i, j;
	int nowTotal, nextTotal;	
	for(i=front + 1; i<rear; i++){
		for(j=i+1; j<=rear; j++){

			nowTotal = pathQueue[i].cost + pathQueue[i].steps;
			nextTotal = pathQueue[j].cost + pathQueue[j].steps;

			if(nowTotal > nextTotal){
				PathNode temp =  pathQueue[i];
				pathQueue[i] = pathQueue[j];
				pathQueue[j] = temp;
			}
		}
	}		
}

//�P�_�Ӥ����O�_�b�W�C����
bool IsInPathQueue(PathNode pathNode){
	int i;
	if(front == rear)
		return false;
	for(i=front;i<=rear;i++){
		if(pathQueue[i].loc.row == pathNode.loc.row && pathQueue[i].loc.col == pathNode.loc.col)
			return true;
	}
	return false;
}

//�^�Ǩ�ؼЦ�m�����|��C 
PathPointer buildPath(PathPointer goal){
	if(goal == NULL || goal->parent == NULL)
		return NULL;
	PathPointer head = goal;
	head->next = NULL;
	PathPointer temp = head;
	
	while(head->parent){
		head = head->parent; 
		head->next = temp;
		temp = head;
	}
	return head;
}

//�p����I�����ݭn���ʪ��B�� 
int calcSteps(Location start, Location goal){
	int steps = abs(start.row - goal.row) + abs(start.col - goal.col);
	return steps;
}

//�P�_�O�_�Ӹ`�I�w�g���X�L 
bool visited(Location loc){
	int i;
	for(i=0;i<=front;i++){
		if(pathQueue[i].loc.row == loc.row && pathQueue[i].loc.col == loc.col)
			return true;
	}
	return false;
}

//�M��̱��񭹪����y�� 
Location findNearestFood(int field[][GRID_SIDE], NodePointer me){
	int rowDis, colDis, row, col, nearest = 100000;
	Location nearestFood;
	for(row = 0; row < GRID_SIDE; row++){
		for(col = 0; col < GRID_SIDE; col++){
			if(field[row][col] == FOOD){
				rowDis = abs(row - me -> row);
				colDis = abs(col - me -> col);
				if(nearest > (rowDis + colDis)){
					nearest = (rowDis + colDis);
					nearestFood.row = row;
					nearestFood.col = col;
				}
			}
		}
	}
	return nearestFood;
}

//�ЦP�ǹ�@�i�̪�AI 
Direction playerAI(int field[][GRID_SIDE], NodePointer player, NodePointer dragon){
	Direction playerDirect = player -> direct;
	Location start = {player -> row, player -> col};
	Location nearestFood = findNearestFood(field, player);
	if(nearestFood.row != -1 && nearestFood.col != -1){
		PathPointer path = findPath(field, start, nearestFood);
		if(path != NULL){
			playerDirect = playergetDirectionByPath(player, dragon, path);
		}
		else
			playerDirect = playersafeDirection(field, player, dragon);
	}
	
	return playerDirect;
}

//���o�w����V�A�u�Ҽ{�|���|����
Direction playersafeDirection(int field[][GRID_SIDE], NodePointer player, NodePointer dragon){
	int i;
	int dirSize = 4;
	Location loc;
	Direction playerDirect = player -> direct;
	
	loc = nextStepLoc(player, UP);
	if(!IsAtWall(field, loc.row, loc.col)&&!IsNearDragon(dragon, loc.row, loc.col)){
		return UP;
	}
	loc = nextStepLoc(player, DOWN);
	if(!IsAtWall(field, loc.row, loc.col)&&!IsNearDragon(dragon, loc.row, loc.col)){
		return DOWN;
	}
	loc = nextStepLoc(player, RIGHT);
	if(!IsAtWall(field, loc.row, loc.col)&&!IsNearDragon(dragon, loc.row, loc.col)){
		return RIGHT;
	}
	loc = nextStepLoc(player, LEFT);
	if(!IsAtWall(field, loc.row, loc.col)&&!IsNearDragon(dragon, loc.row, loc.col)){
		return LEFT;
	}						
	return player->direct;
}

//�P�_�O�_����c�s�ۤv������
bool IsNearDragon(NodePointer dragon, int row, int col){
	
	int nearRow,nearCol;
	while(dragon != NULL){
		nearRow = dragon->row - row;
		nearCol = dragon->col - col;
		nearRow = abs(nearRow);
		nearCol = abs(nearCol);
		if (nearRow <= 1 && nearCol <= 1)/**/
			return true;
		dragon = dragon -> next;
	}
	return false;
}

//�i�̤U�@�B��V 
Direction playergetDirectionByPath(NodePointer player, NodePointer dragon, PathPointer path){
	if(player != NULL && path != NULL){
		PathPointer nextPath = path->next;
		int horizontal = nextPath->loc.col - player->col;
		int vertical = nextPath->loc.row - player->row;
		Direction playerDirect = player -> direct;
		if(horizontal == 1){
			if(IsNearDragon(dragon, nextPath->loc.row, nextPath->loc.col)){
				if(playerDirect == UP)
					return DOWN;
				if(playerDirect == DOWN)
					return UP;
				if(playerDirect == LEFT)
					return RIGHT;
				if(playerDirect == RIGHT)
					return LEFT;
			}
			else
				return RIGHT;
		}
			
		else if(horizontal == -1){
			if(IsNearDragon(dragon, nextPath->loc.row, nextPath->loc.col)){
				if(playerDirect == UP)
					return DOWN;
				if(playerDirect == DOWN)
					return UP;
				if(playerDirect == LEFT)
					return RIGHT;
				if(playerDirect == RIGHT)
					return LEFT;
			}
			else
				return LEFT;
		}
		if(vertical == 1){
			if(IsNearDragon(dragon, nextPath->loc.row, nextPath->loc.col)){
				if(playerDirect == UP)
					return DOWN;
				if(playerDirect == DOWN)
					return UP;
				if(playerDirect == LEFT)
					return RIGHT;
				if(playerDirect == RIGHT)
					return LEFT;
			}
			else
				return DOWN;
		}
		else if(vertical == -1){
			if(IsNearDragon(dragon, nextPath->loc.row, nextPath->loc.col)){
				if(playerDirect == UP)
					return DOWN;
				if(playerDirect == DOWN)
					return UP;
				if(playerDirect == LEFT)
					return RIGHT;
				if(playerDirect == RIGHT)
					return LEFT;
			}
			else
				return UP;
		}
	}
	//�p�G�L�k���`�B�z�A�~��ӭ쥻��V�e�i 
	return 	player -> direct;		
}

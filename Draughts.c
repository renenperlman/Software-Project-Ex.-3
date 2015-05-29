#include "Draughts.h"
#include <limits.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include <ctype.h>
#include<limits.h>
#include<float.h>


int main()
{
	board_t board = (board_t)malloc(BOARD_SIZE*sizeof(char*));
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = (char*)malloc(BOARD_SIZE*sizeof(char));
	}
	init_board(board);
	print_board(board);
	print_message(WRONG_MINIMAX_DEPTH);
	perror_message("TEST");
	linkedList list = { (listNode*)malloc(sizeof(listNode)) };
	linkedList list2 = { (listNode*)malloc(sizeof(listNode)) };
	location loc = { 'b', 4 };
	list2.first->data = (location*)&loc;
	steps step = { list2, list2.first, 0 };
	printf("%d",isBlocked(board, 'w', step, &list, 1));
	return 0;
}


void print_line(){
	int i;
	printf("  |");
	for (i = 1; i < BOARD_SIZE*4; i++){
		printf("-");
	}
	printf("|\n");
}

void print_board(board_t board)
{
	int i,j;
	print_line();
	for (j = BOARD_SIZE-1; j >= 0 ; j--)
	{
		printf((j < 9 ? " %d" : "%d"), j+1);
		for (i = 0; i < BOARD_SIZE; i++){
			printf("| %c ", board[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}



void init_board(board_t board){
	int i,j;
	for (i = 0; i < BOARD_SIZE; i++){
		for (j = 0; j < BOARD_SIZE; j++){
			if ((i + j) % 2 == 0){
				if (j <= 3){
					board[i][j] = WHITE_M;
				}
				else if (j >= 6){
					board[i][j] = BLACK_M;
				}
				else{
					board[i][j] = EMPTY;
				}
			}
			else{
				board[i][j] = EMPTY;
			}
		}
	}
}

int Max(int depth, char player, board_t board, steps** bestStep){
	if (depth == 0){
		return score(board, player);
	}
	char otherPlayer = ('b' + 'w' - player);
	int currMin = INT_MAX;
	int stepScore;
	linkedList* moves = setMoveList(otherPlayer, board);
	listNode* node = moves->first;
	while (node!=NULL)
	{
		stepScore = score(Min(depth - 1, otherPlayer, moveDisc(&board,*(steps*)node->data),bestStep),player);
		if (stepScore < currMin){
			currMin = stepScore;
			*bestStep = (steps*)node->data;
		}
	}
	return currMin;
}

int Min(int depth, char player, board_t board, steps** bestStep){
	if (depth == 0){
		return score(board, player);
	}
	char otherPlayer = ('b' + 'w' - player);
	int currMin = INT_MIN;
	int stepScore;
	linkedList* moves = setMoveList(otherPlayer, board);
	listNode* node = moves->first;
	while (node != NULL)
	{
		stepScore = score(max(depth - 1, otherPlayer, moveDisc(&board,*(steps*)node->data), bestStep),player);
		if (stepScore > currMin){
			currMin = stepScore;
			*bestStep = (steps*)node->data;
		}
	}
	return currMin;
}

steps* minmax(char player){
	steps* bestStep = NULL;
	max(minmaxDepth, player, board, &bestStep);
	return bestStep;
}


void addStep(location* loc, steps* thisSteps){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->data = loc;
	thisSteps->last->next = node;
	thisSteps->last = node;
}

void addNodeTOList(listNode** node, linkedList* list){
	(*node)->next = list->first;
	list->first = *node;
}

listNode* newNode(void* data){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->data = data;
	return node;
}

//assumes that the disc in loc is own by player
//assumes that the disc in loc is a man
//assumes moving is possible
void mMoveStepLeftRight(board_t board, char player, steps thisSteps, linkedList* possibleSteps, int leftRight){
	location lastLoc = *(location*)thisSteps.last;
	int direction;
	if (player=='w')
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	location* locAfterStep = (location*)malloc(sizeof(location));
	locAfterStep->x = lastLoc.x - 'a' + leftRight;
	locAfterStep->y = lastLoc.y + direction;
	addStep(locAfterStep, &thisSteps);
	listNode* newPossibleSteps = newNode(&thisSteps);
	addNodeTOList(&newPossibleSteps, possibleSteps);
}

void moveEat(board_t board, char player, steps thisSteps, linkedList* possibleSteps, int leftRight){
	location lastLoc = *(location*)thisSteps.last;
	int direction;
	if (player == 'w')
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	location* locAfterStep = (location*)malloc(sizeof(location));
	locAfterStep->x = lastLoc.x - 'a' + 2*leftRight;
	location* locAfterStep = (location*)malloc(sizeof(location));
	locAfterStep->x = lastLoc.x - 'a' + leftRight;
	locAfterStep->y = lastLoc.y + direction;
	addStep(locAfterStep, &thisSteps);
	listNode* newPossibleSteps = newNode(&thisSteps);
	thisSteps.numberOfEats++;
	addNodeTOList(&newPossibleSteps, possibleSteps);
	steps nextStep = {NULL}; // change this to be the next step to be taken!
	mMove(moveDisc(&board,nextStep), player, thisSteps, possibleSteps);
}

//return 1 if blocked
//return 0 if not blocked
//return -1 if can eat
int isBlocked(board_t board, char player, steps thisSteps, steps* possibleSteps,  int leftRight){
	location lastLoc = *(location*)((listNode*)thisSteps.last->data);
	int direction;
	if (player == 'w')
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	if (board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == ' '){ //not blocked
		return 0;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == BLACK_M 
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == BLACK_K) && player == 'b'){ // blocked
		return 1;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == WHITE_M 
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == WHITE_K) && player == 'w'){ // blocked
		return 1;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == BLACK_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == BLACK_K) && player == 'w'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y + 2 * direction] == ' '){ // eat forwards
		return -1;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == WHITE_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction] == WHITE_K) && player == 'b'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y + 2 * direction] == ' '){
		return -1;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction] == BLACK_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction] == BLACK_K) && player == 'w'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y - 2 * direction] == ' '){ // eat backwards
		return -1;
	}
	if ((board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction] == WHITE_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction] == WHITE_K) && player == 'b'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y - 2 * direction] == ' '){
		return -1;
	}
}

// updates all possible moves for a given disc
//if this is the first time we try to move it then thisSteps includes only the location of the disc
void mMove(board_t board, char player, steps thisSteps, linkedList* possibleSteps){
	if (isBlocked(board,player,thisSteps,possibleSteps,1)==0)
	{
		mMoveStepLeftRight(board, player, thisSteps, possibleSteps, 1);
	}
	if (isBlocked(board, player, thisSteps, possibleSteps, -1) == 0)
	{
		mMoveStepLeftRight(board, player, thisSteps, possibleSteps, -1);
	}
	if (isBlocked(board, player, thisSteps, possibleSteps, 1) == -1)
	{
		moveEat(board, player, thisSteps, possibleSteps, 1);
	}
	if (isBlocked(board, player, thisSteps, possibleSteps, -1) == -1)
	{
		moveEat(board, player, thisSteps, possibleSteps, -1);
	}
	return;
}

void updatePossibleSteps(linkedList* possibleSteps){
	int maxEat = 0;
	listNode* node = possibleSteps->first;
	if (node == NULL){
		return;
		while (node != NULL)
		{
			if (((steps*)node->data)->numberOfEats > maxEat){
				maxEat = ((steps*)node->data)->numberOfEats;
			}
			node = possibleSteps->first;
			while (((steps*)node->data)->numberOfEats < maxEat)
			{
				possibleSteps->first = node->next;
				freeNode(node);
				node = possibleSteps->first;
			}
			while (node->next != NULL)
			{
				if (((steps*)node->next->data)->numberOfEats < maxEat){
					node->next = node->next->next;
				}
			}
		}
	}
}

void freeNode(listNode* node){
		free(node->data);
		free(node);
	}





int score(board_t b, char player){// not complete just for compilation
	if (setMoveList(player,b)->first == NULL) // no possible moves
	{
		return -100; // losing score
	}
	if (setMoveList('w' + 'b' - player, b)->first == NULL) // other player has no possible moves
	{
		return 100; // winning score
	}
	int score = 0;
	int playerInt = player == 'w' ? 1 : -1;
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (b[i][j]== WHITE_M)
			{
				score += playerInt;
			}
			if (b[i][j] == BLACK_M)
			{
				score -= playerInt;
			}
			if (b[i][j] == WHITE_K)
			{
				score += 3*playerInt;
			}
			if (b[i][j] == BLACK_K)
			{
				score -= 3*playerInt;
			}
		}
	}
	return score;
}

int compareStrings(char* str1,int l1,int r1,char* str2,int l2,int r2){
	if(r1-l1 != r2-l2){
		return 0;
	}
	int i;
	for(i=0;i<r1-l1+1;i++){
		if(str1[l1+i]!=str2[l2+i]){
			return 0;
		}
	}return 1;
}

void analysis(char* input,board_t b){

	int size=0;
	while(input[size]!='\0'){//compute the length of the input
			size++;
	}

	if(input==strstr(input,"minmax_depth")){
		if(input[12]!=' '){//illegal
			printf(ILLEGAL_COMMAND);
		}
		char* temp = input;
		temp+=12;
		int value = (int)atoi(temp);
		setMinmaxDepth(value);
	}
	else if(input==strstr(input,"user_color")){
		if(input[10]!=' '){//illegal
			printf(ILLEGAL_COMMAND);
		}char* color = (char*)malloc(5);
		if(1){
			printf("you should free me - im in analisis");
		}
		strncpy(color,input+11,5);
		setUserColor(color);

	}else if(input==strstr(input,"rm")){//remove Disk command
		if(input[2]!=' '){//illegal
			printf(ILLEGAL_COMMAND);
		}
		int i=2;
		char column;
		int row;
		while(input[i]!='\0'){
			if(input[i]=='<'){
				column= input[++i];
				i+=2;
				char* temp = input+i;
				row =(int)atoi(temp);
			}i++;
		}
		location loc;
		loc.x=column;
		loc.y=row;
		removeDisc(loc);
	}
	else if(input==strstr(input,"clear")){
		if(input[5]!='\0'){//illegal
			printf(ILLEGAL_COMMAND);
		}claer();

	}else if(input==strstr(input,"print")){
		if(input[5]!='\0'){//illegal
			printf(ILLEGAL_COMMAND);
		}print_board(b);
	}else if(input==strstr(input,"quit")){
		if(input[4]!='\0'){//illegal
			printf(ILLEGAL_COMMAND);
		}quit();
	}else if(input==strstr(input,"start")){
		if(input[5]!='\0'){//illegal
			printf(ILLEGAL_COMMAND);
		}start();
	}else if(input==strstr(input,"move")){
		if(input[4]!=' '){//illegal
			printf(ILLEGAL_COMMAND);
		}
		//replace with your code

	}else if(input==strstr(input,"get_moves")){
		if(input[9]!='\0'){//illegal
			printf(ILLEGAL_COMMAND);
		}printMoveList(setMoveList(userColor, b));

	}else{//illegal command
		printf(ILLEGAL_COMMAND);
	}
}

void kMoveStep(board_t board, char player, steps thisSteps, linkedList* possibleSteps){

}

void kMove(board_t board, char player, steps thisSteps, linkedList* possibleSteps){}

steps* locationToSteps(location* loc){
	steps* thisSteps = (steps*)malloc(sizeof(steps));
	thisSteps->numberOfEats = 0;
	linkedList listOfSteps;
	listNode* firstStep = (listNode*)malloc(sizeof(listNode));
	firstStep->data = loc;
	listOfSteps.first = firstStep;
	thisSteps->last = firstStep;
	thisSteps->listOfSteps = listOfSteps;
	return thisSteps;
}

linkedList* setMoveList(char player, board_t board){
	linkedList* possibleSteps = (linkedList*)malloc(sizeof(linkedList));
	possibleSteps->first = NULL;
	location* loc = (location*)malloc(sizeof(loc));
	steps* thisSteps;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++){
			loc->x = 'a' + i;
			loc->y = j;
			thisSteps = locationToSteps(loc);
			if (board[i][j] == WHITE_M && player == 'w' || board[i][j] == BLACK_M && player == 'b')
			{
				mMove(board, player, *thisSteps, possibleSteps);	
			}
			if (board[i][j] == WHITE_K && player == 'w' || board[i][j] == BLACK_K && player == 'b')
			{
				kMove(board, player, *thisSteps, possibleSteps);
			}
		}
	}
	updatePossibleSteps(possibleSteps);
	free(thisSteps); // change to completly free the struct!
}

void printLocation(location* loc){
	printf("<%n,%d>", loc->x, loc->y);
}

void printSteps(steps* thisSteps){
	linkedList listofSteps = thisSteps->listOfSteps;
	listNode* node = listofSteps.first;
	printLocation((location*)node->data);
	printf(" to ");
	node = node->next;
	while (node != NULL)
	{
		printLocation((location*)node->data);
		node = node->next;
	}
}

void printMoveList(linkedList* list){
	listNode* node = list->first;
	while (node != NULL)
	{
		printSteps((steps*)node->data);
		node = node->next;
		if (node!=NULL)
		{
			printf("\n");
		}
	}
}

board_t moveDisc(board_t* board, steps thisSteps){
	int i = ((location*)thisSteps.listOfSteps.first->data)->x - 'a';
	int j = ((location*)thisSteps.listOfSteps.first->data)->y;
	char type = board[i][j];
	board[i][j] = ' ';
	i = ((location*)thisSteps.last->data)->x;
	j = ((location*)thisSteps.last->data)->y;
	board[i][j] = type;
	return *board;
}
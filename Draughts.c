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
	char* userInput;
	printf("Welcome to Draughts!\n");
	printf("Enter game settings:\n");
	steps* bestPlay;
	int startFlag = 0;
	while (!startFlag)
	{
		userInput = getInput(stdin, 128);
		startFlag = analysis(userInput, &board);
		free(userInput);
	}
	if (startFlag==2)
	{
		freeBoard(&board);
		return 0;
	}
	int whiteScore = score(board, 'w');
	while (whiteScore!=100 && whiteScore!=-100)
	{
		startFlag = 0;
		if (currentPlayer == userColor){
			while (startFlag==0)
			{
				startFlag = 0;
				printf("Enter your move:\n");
				steps* userMove = newSteps();
				userInput = getInput(stdin, 128);
				startFlag = analysisGame(userInput, &board, userMove);
					if (startFlag==1)
					{
						moveDisc(&board, *userMove);
					}
				free(userInput);
				free(userMove); // change this to free better
			}
			if (startFlag==2)
			{
				break;
			}
		}
		else
		{
			printf("Computer: move ");
			bestPlay = minmax(board, 'w' + 'b' - userColor);
			moveDisc(&board, *bestPlay);
			printSteps(bestPlay);
			free(bestPlay);
		}
		print_board(board);
		currentPlayer = 'w' + 'b' - currentPlayer;
	}
	if (whiteScore==100)
	{
		printf("white player wins!\n");
	}
	else if (whiteScore == -100)
	{
		printf("black player wins!\n");
	}
	freeBoard(&board);
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
	int currMax = INT_MIN;
	int stepScore;
	linkedList* moves = setMoveList(player, board);
	listNode* node = moves->first;
	while (node!=NULL)
	{
		board_t* newBoard = copyBoard(board);
		moveDisc(newBoard, *(steps*)node->data);
		stepScore = Min(depth - 1, player, *newBoard, bestStep);
		if (stepScore > currMax){
			currMax = stepScore;
			*bestStep = (steps*)node->data;
		}
		freeBoard(newBoard);
		node = node->next;
	}
	return currMax;
}

int Min(int depth, char player, board_t board, steps** bestStep){
	char otherPlayer = ('b' + 'w' - player);
	if (depth == 0){
		print_board(board);
		return score(board, otherPlayer);
	}
	int currMin = INT_MAX;
	int stepScore;
	linkedList* moves = setMoveList(otherPlayer, board);
	listNode* node = moves->first;
	while (node != NULL)
	{
		moveDisc(&board, *(steps*)node->data);
		board_t* newBoard = copyBoard(board);
		stepScore = Max(depth - 1, player, *newBoard, bestStep);
		if (stepScore < currMin){
			currMin = stepScore;
			*bestStep = (steps*)node->data;
		}
		freeBoard(newBoard);
		node = node->next;
	}
	return currMin;
}

steps* minmax(board_t board, char player){
	steps* bestStep = NULL;
	printf("%d",Max(minmaxDepth, player, board, &bestStep));
	return bestStep;
}


void addStep(location* loc, steps* thisSteps){
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->data = loc;
	node->next = NULL;
	if (thisSteps->last == NULL)
	{
		thisSteps->last = node;
		thisSteps->listOfSteps.first = node;
		return;
	}
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

listNode* cloneNode(listNode node, size_t n){
	listNode* clone = (listNode*)malloc(sizeof(listNode));
	char* data = (char*)malloc(n);
	memcpy(data, node.data, n);
	clone->data = data;
	clone->next = NULL;
	return clone;
}

linkedList* cloneLinkedList(linkedList list, size_t n){ // fix me!
	linkedList* clone = (linkedList*)malloc(sizeof(linkedList));
	listNode* node = list.first;
	if (node == NULL)
	{
		clone->first = NULL;
		return clone;
	}
	listNode* first = cloneNode(*node, n);
	clone->first = first;
	node = node->next;
	while (node != NULL)
	{
		listNode* next = cloneNode(*node, n);
		first->next = next;
		first = first->next;
		node = node->next;
	}
	return clone;
}

steps* cloneSteps(steps s){
	steps* clone = (steps*)malloc(sizeof(steps));
	clone->listOfSteps = *cloneLinkedList(s.listOfSteps, sizeof(steps));
	listNode* node = clone->listOfSteps.first;
	while (node->next != NULL)
	{
		node = node->next;
	}
	clone->last = node;
	clone->numberOfEats = s.numberOfEats;
	clone->eats = cloneLinkedList(*s.eats, sizeof(steps));
	return clone;
}

//assumes that the disc in loc is own by player
//assumes that the disc in loc is a man
//assumes moving is possible
void mMoveStepLeftRight(board_t board, char player, steps thisSteps, linkedList* possibleSteps, int leftRight){
	location lastLoc = *(location*)thisSteps.last->data;
	int direction;
	if (player=='w')
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	steps* newMove = cloneSteps(thisSteps);
	location* locAfterStep = (location*)malloc(sizeof(location));
	locAfterStep->x = lastLoc.x + leftRight;
	locAfterStep->y = lastLoc.y + direction;
	addStep(locAfterStep, newMove);
	listNode* newPossibleSteps = newNode(newMove);
	addNodeTOList(&newPossibleSteps, possibleSteps);
}

void moveEat(board_t board, char player, steps thisSteps, linkedList* possibleSteps, int leftRight, int direction){
	location lastLoc = *(location*)thisSteps.last->data;
	steps* newMove = cloneSteps(thisSteps);
	location* locAfterStep = (location*)malloc(sizeof(location));
	locAfterStep->x = lastLoc.x + 2*leftRight;
	locAfterStep->y = lastLoc.y + 2*direction;
	location* eatLoc = (location*)malloc(sizeof(location));
	eatLoc->x = lastLoc.x + leftRight;
	eatLoc->y = lastLoc.y + direction;
	listNode* newEat = newNode(eatLoc);
	addNodeTOList(&newEat, newMove->eats); // adds the new eat location
	newMove->numberOfEats++;
	addStep(locAfterStep, newMove); // update the move locations
	listNode* newPossibleSteps = newNode(newMove);
	addNodeTOList(&newPossibleSteps, possibleSteps); // adds new possible move
	board_t* newBoard = copyBoard(board);
	if (isBlocked(board, player, *newMove, 1) == -1){
		moveDisc(newBoard, *newMove);
		moveEat(*newBoard, player, *newMove, possibleSteps, 1, direction);
	}
	if (isBlocked(board, player, *newMove, -1) == -1){
		moveDisc(newBoard, *newMove);
		moveEat(*newBoard, player, *newMove, possibleSteps, -1, direction);
	}
	if (isBlocked(board, player, *newMove, 1) == -2){
		moveDisc(newBoard, *newMove);
		moveEat(*newBoard, player, *newMove, possibleSteps, 1, -direction);
	}
	if (isBlocked(board, player, *newMove, -1) == -2){
		moveDisc(newBoard, *newMove);
		moveEat(*newBoard, player, *newMove, possibleSteps, -1, -direction);
	}
	freeBoard(newBoard);
}

//return 1 if blocked
//return 0 if not blocked
//return -1 if can eat
int isBlocked(board_t board, char player, steps thisSteps,  int leftRight){
	location lastLoc = *(location*)((listNode*)thisSteps.last->data);
	location newLoc = { lastLoc.x, lastLoc.y };
	int direction;
	if (player == 'w')
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}
	newLoc.x = lastLoc.x + leftRight;
	newLoc.y = lastLoc.y + direction;
	if (isLegalPosition(newLoc) && board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction - 1] == ' '){ //not blocked
		return 0;
	}
	else if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction - 1] == BLACK_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction-1] == BLACK_K) && player == 'b'
		){ // blocked
		return 1;
	}
	else if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction - 1] == WHITE_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction-1] == WHITE_K) && player == 'w'
		){ // blocked
		return 1;
	}
	newLoc.x = lastLoc.x + 2*leftRight;
	newLoc.y = lastLoc.y + 2*direction;
	if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction - 1] == BLACK_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction-1] == BLACK_K) && player == 'w'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y + 2 * direction-1] == ' '
		){ // eat forwards
		return -1;
	}
	else if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction - 1] == WHITE_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y + direction-1] == WHITE_K) && player == 'b'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y + 2 * direction-1] == ' '
		){
		return -1;
	}
	newLoc.y = lastLoc.y - 2 * direction;
	if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction - 1] == BLACK_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction-1] == BLACK_K) && player == 'w'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y - 2 * direction-1] == ' '
		){ // eat backwards
		return -2;
	}
	else if (isLegalPosition(newLoc) && (board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction - 1] == WHITE_M
		|| board[lastLoc.x - 'a' + leftRight][lastLoc.y - direction-1] == WHITE_K) && player == 'b'
		&& board[lastLoc.x - 'a' + 2 * leftRight][lastLoc.y - 2 * direction-1] == ' '
		){
		return -2;
	}
	return 2; //illegal move
}

// updates all possible moves for a given disc
//if this is the first time we try to move it then thisSteps includes only the location of the disc
void mMove(board_t board, char player, steps thisSteps, linkedList* possibleSteps){
	int direction = player == 'w' ? 1 : -1;
	if (isBlocked(board,player,thisSteps,1)==0)
	{
		mMoveStepLeftRight(board, player, thisSteps, possibleSteps, 1);
	}
	if (isBlocked(board, player, thisSteps, -1) == 0)
	{
		mMoveStepLeftRight(board, player, thisSteps, possibleSteps, - 1);
	}
	if (isBlocked(board, player, thisSteps, 1) == -1)
	{
		moveEat(board, player, thisSteps, possibleSteps, 1, direction);
	}
	if (isBlocked(board, player, thisSteps, -1) == -1)
	{
		moveEat(board, player, thisSteps, possibleSteps, -1, direction);
	}
	if (isBlocked(board, player, thisSteps, 1) == -2)
	{
		moveEat(board, player, thisSteps, possibleSteps, 1, -direction);
	}
	if (isBlocked(board, player, thisSteps, -1) == -2){
		moveEat(board, player, thisSteps, possibleSteps, -1, -direction);
	}
	return;
}

void updatePossibleSteps(linkedList* possibleSteps){
	int maxEat = 0;
	listNode* node = possibleSteps->first;
	if (node == NULL){
		return;
	}
	while (node != NULL) { // find maxEat
		if (((steps*)node->data)->numberOfEats > maxEat){
			maxEat = ((steps*)node->data)->numberOfEats;
		}
		node = node->next;
	}
	node = possibleSteps->first;
	while (((steps*)node->data)->numberOfEats < maxEat){ // makes the first node to have maxEat
		possibleSteps->first = node->next;
		free(node); // change to better free
		node = possibleSteps->first;
	}
	node = possibleSteps->first;
	while (node->next != NULL){
		int i = ((steps*)node->next->data)->numberOfEats;
		if (((steps*)node->next->data)->numberOfEats < maxEat){
			node->next = node->next->next;
		}
		if (node->next == NULL){
			break;
		}
		node = node->next;
	}
}


void freeNode(listNode* node){
		free(node->data);
		free(node);
	}





int score(board_t b, char player){
	int playerLoseFlag = 1;
	int otherPlayerLoseFlag = 1;
	linkedList listOfSteps = { NULL };
	location* loc = (location*)malloc(sizeof(location));
	steps* thisSteps = locationToSteps(loc);
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			((location*)(thisSteps->last->data))->x = i + 'a';
			((location*)(thisSteps->last->data))->y = j + 1;
			if (isBlocked(b, player, *thisSteps, 1) != 1){ // has a possible move
				playerLoseFlag = 0;
			}
			if (isBlocked(b, 'w'+'b'-player, *thisSteps, 1) != 1){ // has a possible move
				otherPlayerLoseFlag = 0;
			}
		}
	}
	if (playerLoseFlag) // no possible moves
	{
		return -100; // losing score
	}
	if (otherPlayerLoseFlag) // other player has no possible moves
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
	free(loc);
	free(thisSteps); //update to free steps!
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

int analysis(char* input, board_t* b){
	int size = 0;
	while (input[size] != '\0'){//compute the length of the input
		size++;
	}
	if (input == strstr(input, "minimax_depth")){
		if (input[13] != ' '){//illegal
			printf(ILLEGAL_COMMAND);
			return 0;
		}
		char* temp = input;
		temp += 13;
		int value = (int)atoi(temp);
		setMinmaxDepth(value);
	}
	else if (input == strstr(input, "user_color")){
		if (input[10] != ' '){//illegal
			printf(ILLEGAL_COMMAND);
			return 0;
		}char* color = (char*)malloc(5);
		if (DBUG == 1){
			printf("you should free me - im in analisis");
		}
		strncpy(color, input + 11, 5);
		setUserColor(color);

	}
	else if (input == strstr(input, "rm")){//remove Disk command
		if (input[2] != ' '){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}
		int i = 2;
		char column;
		int row;
		while (input[i] != '\0'){
			if (input[i] == '<'){
				column = input[++i];
				i += 2;
				char* temp = input + i;
				row = (int)atoi(temp);
			}i++;
		}
		location loc;
		loc.x = column;
		loc.y = row;
		removeDisc(b, loc);
	}
	else if (input == strstr(input, "clear")){
		if (input[5] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}clear(b);
	}
	else if (input == strstr(input, "print")){
		if (input[5] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}print_board(*b);
	}
	else if (input == strstr(input, "quit")){
		if (input[4] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}return 2;
	}
	else if (input == strstr(input, "start")){
		if (input[5] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		} return 1;
	}
	else if (input == strstr(input, "set")){
		if (input[3] != ' '){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}
		int i = 3;
		char column;
		int row;
		char* color = (char*)malloc(5);;
		char type;

		while (input[i] != '\0'){
			if (input[i] == '<'){
				column = input[++i];
				i += 2;
				char* temp = input + i;
				row = (int)atoi(temp);
			}i++;
		}
		location loc;
		loc.x = column;
		loc.y = row;
		int flag = 0;
		i = 0;
		while (input[i] != '\0'){
			if (input[i] == '>'){
				i++;
				while (input[i] != '\0'){
					if (input[i] != ' '&&flag == 0){
						strncpy(color, input + i, 5);
						i += 5;
						flag = 1;
					}if (flag == 1 && input[i] != ' '){
						type = input[i];
						break;
					}
					i++;
				}
			}
			i++;
		}
		setDisc(b, loc, color, type);
	}
	else{//illegal command
		print_message(ILLEGAL_COMMAND);
		return 0;
	}
	return 0;
}

int containCurrentPlayerPiece(board_t board,location loc){
	if(currentPlayer=='w'){
		if((board[loc.x-'a'][loc.y-1]!='m')&&(board[loc.x-'a'][loc.y-1]!='k')){
			return 0;
		}
		return 1;
	}else{
		if((board[loc.x-'a'][loc.y-1]!='M')&&(board[loc.x-'a'][loc.y-1]!='K')){
			return 0;
		}
		return 1;
	}
}

void updateListMoves(steps* mySteps,location loc){

	listNode* node=(listNode*)newNode(&loc);
	if(mySteps->listOfSteps.first==NULL){
		linkedList list = { node };
		mySteps->last=node;
		mySteps->listOfSteps=list;;
	}else{
		mySteps->last->next=node;
		mySteps->last=node;
		mySteps->numberOfEats++;
	}
}



void setMinmaxDepth(int depth){// checks if 1<=int<=6

	if(depth<1||depth>6){
		print_message(WRONG_MINIMAX_DEPTH);
		return;
	}minmaxDepth=depth;
}

void setUserColor(char* color){ // dosen't check if color == white || black
	userColor=color[0];
}

void clear(board_t* board){
	int i;
	int j;
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			(*board)[i][j]=EMPTY;
		}
	}
}

void removeDisc(board_t* board,location loc){// checks if legal location (1<=x<=10 'a'<=y<='j')
	if(isLegalPosition(loc)==0){
		print_message(WRONG_POSITION);
		return;
	}
	(*board)[loc.x-'a'][loc.y]=EMPTY;

}

int isLegalPosition(location loc){
	if(loc.x<'a' || loc.x>'j'){
		return 0;
	}else if(loc.y<0||loc.y>9){
		return 0;
	}return 1;
}

void setDisc(board_t* board,location loc, char* color, char type){ // checks if legal location (1<=x<=10 'a'<=y<='j')
	if(isLegalPosition(loc)==0){
		print_message(WRONG_POSITION);
		return;
	}char disk;
	if(color==strstr(color,"white")){
		if(type=='m'){
			(*board)[loc.x-'a'][loc.y-1]=WHITE_M;
		}else{
			(*board)[loc.x-'a'][loc.y-1]=WHITE_K;
		}
	}else{
		if(type=='m'){
			(*board)[loc.x-'a'][loc.y-1]=BLACK_M;
		}else{
			(*board)[loc.x-'a'][loc.y-1]=BLACK_K;
		}
	}
}



void kMoveStep(board_t board, char player, steps thisSteps, linkedList* possibleSteps){

}

void kMove(board_t board, char player, steps thisSteps, linkedList* possibleSteps){}

steps* locationToSteps(location* loc){
	steps* thisSteps = (steps*)malloc(sizeof(steps));
	thisSteps->numberOfEats = 0;
	linkedList listOfSteps = { NULL };
	listNode* firstStep = (listNode*)malloc(sizeof(listNode));
	firstStep->data = loc;
	firstStep->next = NULL;
	listOfSteps.first = firstStep;
	thisSteps->last = firstStep;
	thisSteps->listOfSteps = listOfSteps;
	linkedList* eats = (linkedList*)malloc(sizeof(linkedList));
	eats->first = NULL;
	thisSteps->eats = eats;
	return thisSteps;
}

linkedList* setMoveList(char player, board_t board){
	linkedList* possibleSteps = (linkedList*)malloc(sizeof(linkedList));
	possibleSteps->first = NULL;
	steps* thisSteps;	
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++){
			location* loc = (location*)malloc(sizeof(loc));
			loc->x = 'a' + i;
			loc->y = 1+j;
			thisSteps = locationToSteps(loc);
			if ((board[i][j] == WHITE_M && player == 'w') || (board[i][j] == BLACK_M && player == 'b'))
			{
				mMove(board, player, *thisSteps, possibleSteps);
			}
			if ((board[i][j] == WHITE_K && player == 'w') || (board[i][j] == BLACK_K && player == 'b'))
			{
				kMove(board, player, *thisSteps, possibleSteps);
			}
		}
	}
	updatePossibleSteps(possibleSteps);
	return possibleSteps;
	free(thisSteps); // change to completly free the struct!
}

void printLocation(location* loc){
	printf("<%c,%d>", loc->x, loc->y);
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
	printf("\n");
}

void printMoveList(linkedList* list){
	listNode* node = list->first;
	while (node != NULL)
	{
		printSteps((steps*)node->data);
		node = node->next;
	}
}

void moveDisc(board_t* board, steps thisSteps){
	int i = ((location*)thisSteps.listOfSteps.first->data)->x - 'a';
	int j = ((location*)thisSteps.listOfSteps.first->data)->y - 1;
	char type = (*board)[i][j];
	(*board)[i][j] = EMPTY;
	i = ((location*)thisSteps.last->data)->x - 'a';
	j = ((location*)thisSteps.last->data)->y - 1;
	if (type==WHITE_M && j==9) // crown white player
	{
		type = WHITE_K;
	}
	else if (type == BLACK_M && j == 0){ // crown black player
		type = BLACK_K;
	}
	(*board)[i][j] = type;
	listNode* node = thisSteps.eats->first;
	while (node!=NULL)
	{
		i = ((location*)node->data)->x - 'a';
		j = ((location*)node->data)->y - 1;
		(*board)[i][j] = EMPTY;
		node = node->next;
	}
}

char * getInput(FILE * fp, size_t size){
	char *str;
	int ch;
	size_t len = 0;
	if ((str = (char *)malloc(sizeof(char)*size)) == NULL){
		printf("Memory allocation error!");
		exit(1);
	}
	while (EOF != (ch = fgetc(fp)) && ch != '\n')
	{
		str[len++] = ch;
		if (len == size)
		{
			if ((str = realloc(str, sizeof(char)*(size += 128))) == NULL){
				printf("Memory allocation error!");
				exit(1);
			}
		}
	}
	str[len++] = '\0';
	return realloc(str, sizeof(char)*len);
}

void freeBoard(board_t* board){
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		free((*board)[i]);
	}
	free(*board);
}

board_t* copyBoard(board_t b){
	board_t* newBoard = (board_t*)malloc(sizeof(board_t));
	(*newBoard) = (char**)malloc(BOARD_SIZE*sizeof(char*));
	for (int i = 0; i < BOARD_SIZE; i++){
		(*newBoard)[i] = (char*)malloc(BOARD_SIZE*sizeof(char));
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			(*newBoard)[i][j] = b[i][j];
		}
	}
	return newBoard;
}

int compareLoc(location* loc1, location* loc2){
	if (loc1->y != loc2->y){
		return 0;
	}if ((loc1->x - 'a') != (loc2->x - 'a')){
		return 0;
	}
	return 1;
}

int isLegalMove(board_t* board, steps* mysteps){
	linkedList* list = setMoveList(userColor, *board);
	listNode* move = list->first;
	int myNOeat = mysteps->numberOfEats;
	while (move != NULL){
		int optionalNOeat = ((steps*)move->data)->numberOfEats;
		if (myNOeat != optionalNOeat){
			continue;
		}
		listNode* optional = ((steps*)move->data)->listOfSteps.first;
		listNode* myNode = mysteps->listOfSteps.first;
		int flag = 1;
		while (optional != NULL){//myNode must be != NULL cause they have the same number of eat
			if (compareLoc((location*)optional->data, (location*)myNode->data)){
				optional = optional->next;
				myNode = myNode->next;
				continue;
			}flag = 0;
			break;
		}if (flag == 0 || optional != NULL){
			flag = 1;
			continue;
		}
		return 1;
		move = move->next;
	}
	return 0;
}
int analysisGame(char* input, board_t* b, steps* mySteps){

	int size = 0;
	while (input[size] != '\0'){//compute the length of the input
		size++;
	}
	if (input == strstr(input, "move")){
		if (input[4] != ' '){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}
		int i = 4;
		char column;
		int row;
		location loc;
		mySteps->numberOfEats = 0;
		while (input[i] != '\0'){
			if (input[i] == '<'){
				column = input[++i];
				i += 2;
				char* temp = input + i;
				row = (int)atoi(temp);
				loc.x = column;
				loc.y = row;
				if (isLegalPosition(loc) == 0){
					print_message(WRONG_POSITION);//error 1
					return 0;
				}
				addStep(&loc, mySteps);
			}
			i++;
		}if (containCurrentPlayerPiece(*b, *(location*)mySteps->listOfSteps.first->data) == 0){
			print_message(NO_DICS);// error 2
			return 0;
		}if (isLegalMove(b, mySteps) == 0){
			print_message(ILLEGAL_MOVE); // error 3
			return 0;
		}
		return 1;

	}
	else if (input == strstr(input, "get_moves")){
		if (input[9] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}printMoveList(setMoveList(userColor, *b));
	}
	else if (input == strstr(input, "quit")){
		if (input[4] != '\0'){//illegal
			print_message(ILLEGAL_COMMAND);
			return 0;
		}return 2;
	}
	else{//illegal command
		print_message(ILLEGAL_COMMAND);
		return 0;
	}
	return 0;
}

steps* newSteps(){
	steps* s = (steps*)malloc(sizeof(steps));
	s->numberOfEats = 0;
	linkedList* eats = (linkedList*)malloc(sizeof(linkedList));
	eats->first = NULL;
	linkedList moves = { NULL };
	s->eats = eats;
	s->last = NULL;
	s->listOfSteps = moves;
	return s;
}
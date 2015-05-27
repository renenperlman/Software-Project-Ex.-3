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
	init_board(board);
	print_board(board);
	print_message(WRONG_MINIMAX_DEPTH);
	perror_message("TEST");
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

void print_board(char board[BOARD_SIZE][BOARD_SIZE])
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



void init_board(char board[BOARD_SIZE][BOARD_SIZE]){
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

int max(int depth, char player, board_t board, steps** bestStep){
	if (depth == 0){
		return score(board);
	}
	char otherPlayer = ('b' + 'w' - player);
	int currMin = INT_MAX;
	int stepScore;
	linkedList moves = setMoveList(otherPlayer, board);
	listNode* node = moves.first;
	while (node!=NULL)
	{
		stepScore = score(min(depth - 1, otherPlayer, moveDisc(*(steps*)node->data, player),bestStep));
		if (stepScore < currMin){
			currMin = stepScore;
			*bestStep = (steps*)node->data;
		}
	}
	return currMin;
}

int min(int depth, char player, board_t board, steps** bestStep){
	if (depth == 0){
		return score(board);
	}
	char otherPlayer = ('b' + 'w' - player);
	int currMin = INT_MIN;
	int stepScore;
	linkedList moves = setMoveList(otherPlayer, board);
	listNode* node = moves.first;
	while (node != NULL)
	{
		stepScore = score(max(depth - 1, otherPlayer, moveDisc(*(steps*)node->data, player), bestStep));
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

void analysis(char* input){

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
		if(DBUG){
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
		location loc = {};
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
		}print_board();
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
		}printMoveList(setMoveList(currentPlayer,board));

	}else{//illegal command
		printf(ILLEGAL_COMMAND);
	}
}


  

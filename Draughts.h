#ifndef DRAUGHTS_
#define DRAUGHTS_

#include<stdio.h>


#define WHITE_M 'm'
#define WHITE_K 'k'
#define BLACK_M 'M'
#define BLACK_K 'K'
#define EMPTY ' '

#define BOARD_SIZE 10

typedef char** board_t;
#define WELCOME_TO_DRAUGHTS "Welcome to Draughts!\n"
#define ENTER_SETTINGS "Enter game settings:\n" 
#define WRONG_MINIMAX_DEPTH "Wrong value for minimax depth. The value should be between 1 to 6\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NO_DICS "The specified position does not contain your piece\n"
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define ILLEGAL_MOVE "Illegal move\n"
#define WROND_BOARD_INITIALIZATION "Wrong board initialization\n"
 
#define ENTER_YOUR_MOVE "Enter your move:\n" 
#define perror_message(func_name) (fprintf(stderr, "Error: standard function %s has failed\n", func_name))
#define print_message(message) (printf("%s", message));



void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
void init_board(char board[BOARD_SIZE][BOARD_SIZE]);
typedef struct linkedList linkedList;
typedef struct listNode listNode;
typedef struct location location;
typedef struct steps steps;

struct listNode
{
	struct listNode* next;
	void* data;
};

struct linkedList
{
	struct listNode* first;
};

struct location
{
	char x;
	int y;
};

struct steps
{
	linkedList listOfSteps;
	listNode* last;
	int numberOfEats;
};



linkedList moves = { NULL }; // the global list of all possible moves
int DBUG =0;
char currentPlayer='w';
int minmaxDepth = 1;
char userColor = 'w';
char board[BOARD_SIZE][BOARD_SIZE];
void mMoveStep(board_t, char, location, steps); // adds to the list of steps any possible one step move of a man
void kMoveStep(board_t, char, location, steps);// adds to the list of steps any possible one step move of a king
void moveEat(board_t, char, location, steps); // adds to the list of steps any possible one eat

char* getInput(FILE * , size_t);
void analysis(char*);
void setMinmaxDepth(int); // checks if 1<=int<=6
void setUserColor(char*); // checks if color == white || black
void clear();
void removeDisc(location); // checks if legal location (1<=x<=10 'a'<=y<='j')
void setDisc(location, char*, char); // checks if legal location (1<=x<=10 'a'<=y<='j')
void quit(); // frees all mallocs
void start(); // checks if 1. board is empty 2. only one color 3. more than 20 discs in the same color
int isLegalPosition(location);
board_t moveDisc(steps, char);
linkedList setMoveList(char, board_t);
int isLegalMove(steps);
int compateLoc(location, location);
int score(board_t);
int max(int , char , board_t , steps** );
int min(int , char , board_t , steps** );
steps* minmax(char);
void printMoveList(linkedList moves);
int compareStrings(char*,int,int,char*,int,int);
#endif  



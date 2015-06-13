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



void print_board(board_t);
void init_board(board_t);
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
	linkedList* eats;
};



linkedList moves = { NULL }; // the global list of all possible moves
int DBUG =0;
char currentPlayer='w';
int minmaxDepth = 1;
char userColor = 'w';
void mMoveStep(board_t, char, steps, linkedList*); // adds to the list of steps any possible one step move of a man
void kMoveStep(board_t, char, steps, linkedList*);// adds to the list of steps any possible one step move of a king
void moveEat(board_t, char, steps, linkedList*, int, int); // adds to the list of steps any possible one eat
void mMove(board_t, char, steps, linkedList*);

char* getInput(FILE * , size_t);
int analysis(char*, board_t*);
void setMinmaxDepth(int); // checks if 1<=int<=6
void setUserColor(char*); // doesn't check if color == white || black
void clear(board_t*);
void removeDisc(board_t*,location); // checks if legal location (1<=x<=10 'a'<=y<='j')
void setDisc(board_t*,location, char*, char); // checks if legal location (1<=x<=10 'a'<=y<='j')
void start(); // checks if 1. board is empty 2. only one color 3. more than 20 discs in the same color
int isLegalPosition(location);
void moveDisc(board_t*,steps);
linkedList* setMoveList(char, board_t);
int isLegalMove(board_t*,steps*);
int compateLoc(location, location);
int score(board_t, char);
int Max(int , char , board_t , steps* );
int Min(int , char , board_t , steps* );
steps* minmax(board_t,char);
void printMoveList(linkedList* moves);
int isBlocked(board_t, char, steps, int);
void freeNode(listNode*);
int compareStrings(char*,int,int,char*,int,int);
int containCurrentPlayerPiece(board_t,location);
int isWin(board_t);
steps* locationToSteps(location*);
void printSteps(steps*);
void freeBoard(board_t*);
board_t* copyBoard(board_t);
void updateListMoves(steps*, location);
steps* cloneSteps(steps);
void addStep(location*, steps*);
steps* newSteps();
#endif  



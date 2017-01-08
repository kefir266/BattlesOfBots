// Game of the Amazons.cpp : Defines the entry point for the console application.
//



#include <iostream>
#include <sstream>
#include <ctime>
#define ABS(a)  ((a) < 0 ? -(a) : (a))

#define N 10

int myPlayer;
clock_t starttime;
bool onlySpace;

using namespace std;


/////////////////////////
struct Dir {
	int x,y;

} dir[8] = {{-1,-1},{-1,0},{-1,1},
{0,-1},{0,1},
{1,-1},{1,0},{1,1}};

struct StrNext{
	int pl; int n;
	int nSpace;
	bool doub;
};
//////////////////////////////////
class Move {

public:
	int x,y;
	int cost;

	Move* nextLink;
	Move() {
		nextLink = this;
		cost = -7777777;
	}
	Move(int movX, int movY) {
		x = movX;
		y = movY;
	}
	Move* next() {
		return nextLink;
	}
};


///////////////////////////////
class Field {

	Move* curMove;
	Move* bestMove;
	int player;
	int recurN;

public:
	int** mas;
	int numOfMoves;
	Move* firstMove;

	Field(int** fld, int pl){ ///New Field
		recurN = 0;
		numOfMoves = N * N;
		player = pl;
		copyInit(fld);
		
	}

	Field(Field& fld, int pl, Move* move, int recur){ /// Constructor's copier

		numOfMoves = N * N;
		player = pl;
		copyInit(fld.mas);
		
		mas[move->y][move->x] = pl;
		recurN = recur;
	}

	~Field(){
		delete []mas;
	}

	//////////////////////////////////////////////
	void copyInit(int** fld){
		firstMove = new Move();
		curMove = firstMove;
		mas = new int*[N];
		for (int y = 0; y < N; y++){
			mas[y] = new int[N];
			for (int x = 0; x < N; x++){
				if (fld[y][x] != 0) {
					numOfMoves--;

				}
				else{
					curMove->nextLink = new Move(x,y);
					curMove = curMove->nextLink;
					curMove->nextLink = firstMove;
					
				}
				mas[y][x] = fld[y][x];
			}
		}
	}


	////////////////////////////////
	int getCostDirect(StrNext* next) {
		
		if ((next->n + next->nSpace) < 5) return 0;
	

		if (player == next->pl) {
			switch (next->n) {
			case 0: return 0;
			case 1: return 10;
			case 2: return 200;
			case 3: {
				if (next->doub) return 6000;
				else return 300;}
			case 4: return 90000000;
			default: {if (myPlayer == 1) return -10; else return 99999999;}
			}
		}
		else
		{if (recurN == 0){
			switch (next->n) {
			case 0: return 0;
			case 1: return 10;
			case 2: return 100;
			case 3:if (next->doub) return 5000;
				else return 200;
			case 4: return 9000000;
			default: {if (myPlayer == 2) return -10; else return 9000000;}
			}}
		}
		return 0;
	}

	////////////////////////////////////
	StrNext* getStonesInDirection(int dx, int dy, int x, int y, StrNext* str, bool tr ) {
		
		if (str->n >5) return str;

		int n = str->n; 
		int x1 = x + dx;
		int y1 = y + dy;
		if (x1 >= 0 && y1 >= 0 && x1 < N && y1 < N){
			
			if (str->pl == 0)
				str->pl = mas[y1][x1];

			if (str->pl == 0 ) return str;

			if (n == 0 && tr){
				getStonesInDirection(-dx,-dy,x , y ,str, false);
				onlySpace = false;
			}

			if (mas[y1][x1] == 0) {
				str->nSpace++;
				str->doub = tr;
				onlySpace = true;
				getStonesInDirection(dx,dy,x1,y1,str, tr);
			}

			if (str->pl != mas[y1][x1] || onlySpace) return str;
				
			
			str->n++;
			getStonesInDirection(dx,dy,x1,y1,str, tr);
			
		}

		return str;
	}
	//////////////////////////////
	Move* calcMoves(){
		
		StrNext* next = new StrNext;
		int maxCost = -9999999;
		int curCost = 0;
		curMove = firstMove->nextLink;
		bestMove = curMove;
		while (curMove != firstMove) {
			curCost = 0;
			for (int d = 0; d < 8; d++) {
				next->pl = 0;
				next->n = 0;
				next->nSpace = 1;
				next->doub = false;
				onlySpace = false;
				next = getStonesInDirection(dir[d].x,dir[d].y,curMove->x, curMove->y,next, true);
				curCost += getCostDirect(next);
				
				if ((curCost > 10 + recurN<<4 ) &&  (curCost < 90000 )  && (starttime + 900 > clock()) ) {
					Move* nMove = nextMove(curMove, recurN, player);
					curCost -=  nMove->cost >> 3;
				}
				
			}
			curMove->cost = curCost;
			curCost += (N - ABS(curMove->x - 5) - ABS(curMove->y - 5));
			curMove->cost = curCost;
				
			if (curMove->cost > maxCost) {
				maxCost = curMove->cost;
				bestMove = curMove;
			}
			if (maxCost >= 900000000) break;
			curMove = curMove->nextLink;
		}
		return bestMove;
	}

	////////////////////////////////////////////////

	Move* getBestMove(){
		
		return bestMove;
	}

	//////////////////////////////////////////
	Move* nextMove( int recur, int player){
		

		calcMoves();



		return bestMove;
	}
	//////////////////////////////////////////
	Move* nextMove(Move* move, int recur, int player){
		if (recur > 7  || numOfMoves == 1) return move;
		Field* newField = new Field(*this, 3 - player, move, recur+1);

		newField->calcMoves();

		return newField->bestMove;
	}
};

/////////////////////////////////////////////////////

class Stones {
	int x,y;
	int player;
	int numberOfDirection[4];

public: Stones(int y, int x, int p) {
			this->x = x;
			this->y = y;
			this->player = p;
		}
		void calcNumberToDirection(){
		
		}
};


///////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	starttime = clock();

	
	int** mas;
	mas = new int*[N];
	for (int y = 0; y < N ; y++) {
		mas[y] = new int[N];
		for (int x = 0; x < N ; x++) 
			cin >> mas[y][x];
	}
	cin >> myPlayer;
	
	Field* field = new Field(mas, myPlayer);
	Move * move = field->nextMove(0, myPlayer);

	cout << move->y << " " << move->x <<endl;
	return 0;
}

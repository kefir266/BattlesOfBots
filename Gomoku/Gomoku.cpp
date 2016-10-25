// Gomoku.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <ctime>
#define ABS(a)  ((a) < 0 ? -(a) : (a))

#define N 10
#define STACK 10

int myPlayer;
clock_t starttime, t2;
clock_t calc_time = 0;
bool onlySpace;
int numOfMoves = N * N;
int maxRecurNumber = N;
double counter = 0;

using namespace std;



struct Dir {
	int x,y;

} dir[4] = {{1,1},{1,0},{0,1},{1,-1}};

int mas[N][N]={0};


//////////////////////////////////
class Move {

public:
	int x,y;
	int cost;
	int predict;

	Move* nextLink;
	Move* bestMove;
	Move* prevLink;

	Move() {
		prevLink = this;
		nextLink = this;
		cost = -1;
		predict = 0;
	}



	Move(int movX, int movY) {
		x = movX;
		y = movY;
		cost = 0;
	}
	Move* next() {
		return nextLink;
	}
}* firstMove;


///////////////////////////////
class Field {


	Move* bestMove;
	int player;
	int recurN;

public:
	
	//int numOfMoves;
	//Move* firstMove;
	Field* masFields;

	//Field(){
	//	firstMove = new Move();
	//}

	~Field(){

		
	}

	void init(int pl, int rec){
		recurN = rec;
		//numOfMoves = N * N;
		player = pl;
		//firstMove->nextLink = firstMove;
		//initField();
	}

	////////////////////////////////////
	//void initField(){
	//	
	//	Move* move = firstMove;
	//	for (int y = 0; y < N; y++){
	//		
	//		for (int x = 0; x < N; x++){
	//			if (mas[y][x] != 0) {
	//				numOfMoves--;

	//			}
	//			else{
	//				
	//				move->nextLink = new Move(x,y);
	//				move->nextLink->prevLink = move;
	//				move = move->nextLink;
	//				 
	//				move->nextLink = firstMove;
	//				
	//			}
	//			
	//		}
	//	}
	//}



	///////////////////////////////
	int getCost(Move* move) {
		
		int costAll = 0;

		for (int p =1; p <= 2; p++)
		{
			
			int rivel = 3 - p;
			int cost = 0;
			//bool cross = false;
			bool isTwo = false;
			
			///----------------------------------------------
			for (int d = 0; d < 4; d++){

				int sum = 1;
				int sumUnion = 0;
				int sumP = 0;
				bool isTwoEnds = false;
				bool v = false;
				
				//============================================
				do{ 
					int dx = v ? dir[d].x: -dir[d].x;
					int dy = v ? dir[d].y: -dir[d].y;

					int x = move->x + dx;
					int y = move->y + dy;
					//bool step = true;
					bool isUnion = true;
					int cell = -1;
					int oneWay = 0;
					
					while (x>=0 && y>=0 && x < N && y < N ){
						cell = mas[y][x];
						if ( cell == rivel) break;

						if (cell == 0) isUnion = false;
						else sumP++;
						//clock_t t1 = clock();
						sum++;
						if (isUnion) {
							oneWay++;
							sumUnion++;
							
							if (sumUnion >= 5) break;
						}
						//clock_t t2 = clock();
						//calc_time += (t2-t1);
						x += dx;
						y += dy;
					}
					

					v = !v;
					if (cell == 0){

						if (v) isTwoEnds = true;
					
					}
					else
						isTwoEnds = ((sumUnion -oneWay) >=3) &&  isTwoEnds;

				}while(v && (sumUnion < 5));
				
				//===========================================
				if (sum < 5) continue;

				
				int curCost = 0;
				switch (sumUnion) {
				case 0: /*cost += 1; */ break;
				case 1: curCost = 1 + sum; break;
				case 2: {
					// if (isTwo ||  (sumP >= 3)) cost += 500; 
					// 	else cost += 100;
					// 	if ( (p == 1 && sum <=5) || p == 2) cost += sum * 100;
						curCost = 100;
						break;
						}
				case 3: {//if( isTwo) cost +=10000; else cost += 1000; 
						//if ( (p == 1 && sum <=5) || p == 2) cost += sum * 1000;
						curCost = 10000;
						break;
				}
				case 4: {if ( player == p ) curCost = 99999999; else
							curCost = 10000000;
						break;
						}
				default: {
					if (p == 1 && sumUnion >=5) {
						//cost = 0;
						break;
						
					}
					curCost = 100000000;
						 }
				}
				if (isTwoEnds) 
					curCost *= 10;
				if (d == 0 || d == 3) 
					curCost *=  10;
				
				cost += curCost;
				if (sumUnion > 2) isTwo = true;
			}
			//if (p == player) cost += (cost>>1);
				costAll += cost;
			/////---------------------------------------------------
		}
		return costAll;
	}

	//////////////////////////////
	Move* calcMoves(){
		//counter++;
		//StrNext* next = new StrNext;
		 
		//if (recurN < 4)  {
			clock_t t1 = clock();
			if (starttime + 6000000 <  t1)
				maxRecurNumber = 2;
				// else
				// 	if (starttime + 500000 <  t1){
				// 		maxRecurNumber--;
				// 		//t2 = t1;
				// 	}
			
			
		//}
		int maxCost = -99999999;
		int curCost = 0;
		Move* curMove = firstMove->nextLink;
		Move* bestMove = curMove;
		
		while (curMove != firstMove) {

			
			curCost = getCost(curMove);
			
			curMove->predict = curCost;
			
			if (curCost > 90000000) return curMove;
			
			 //if ((recurN < maxRecurNumber)  && (curCost >= 0 + recurN) /*+ (recurN<<1) ) 
			 //	*/ ) {
			 //		Move* nMove = nextMove(curMove, recurN, player);
			 //		/*if (recurN > 0 && nMove->cost >= 1000000) { 
			 //			nMove->cost = curCost - nMove->cost;
			 //			return nMove;}*/
			 //		//curMove->predict = nMove->cost;
			 //		curCost -=  (nMove->cost /4);
			 //	}
			curCost += (N - ABS(curMove->x - 5) - ABS(curMove->y - 5));
			
				
			if (curCost > maxCost) {
				maxCost = curCost;
				bestMove = curMove;
				bestMove->cost = curCost;
				//bestMove->predict = curMove->predict;
			}
			if (maxCost >= 90000000) break;
			curMove = curMove->nextLink;
		}
		//this->bestMove = bestMove;
		return bestMove;
	}


	//////////////////////////////////////////
	Move* nextMove( int player){
		
		init(player,0);
		return calcMoves();

	}
	//////////////////////////////////////////
	Move* nextMove(Move* move, int recur, int player){
		if (numOfMoves == 1) return move;

		Field* newField = &masFields[recur+1]; 
		newField->init(3-player,recur+1);
		mas[move->y][move->x] = player;
		//newField->copyInit(mas,move);
		//new Field(*this, 3 - player, move, recur+1);

		Move* swap = move;
		swap->prevLink->nextLink = swap->nextLink;
		swap->nextLink->prevLink = swap->prevLink;

		Move* nMove = newField->calcMoves();
		
		swap->prevLink->nextLink = swap;
		swap->nextLink->prevLink = swap;

		mas[move->y][move->x] = 0;
		//newField->deleteMoves();
		return nMove;

	}
};

/////////////////////////////////////////////////////

Field objectFields[STACK];
///////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{


	starttime = clock();
	t2 = starttime;
	firstMove = new Move();

	
	for (int i= 0; i < STACK; i++){
		objectFields[i].masFields = objectFields;
		//objectFields[i].mas = new int*[N];
		//for (int y = 0; y < N ; y++)
		//	objectFields[i].mas[y] = new int[N];
	} 


	Move* move = firstMove;
	//int** mas;
	//mas = objectFields[0].mas;
	for (int y = 0; y < N ; y++) {
		for (int x = 0; x < N ; x++) {
			cin >> mas[y][x];
			
	
				if (mas[y][x] != 0) {
					numOfMoves--;

				}
				else{
					
					move->nextLink = new Move(x,y);
					move->nextLink->prevLink = move;
					move = move->nextLink;
					 
					move->nextLink = firstMove;
					
				}
		}
	}
	


	cin >> myPlayer;
	

	
	Move*  move2 = objectFields[0].nextMove( myPlayer);

	cout << move2->y << " " << move2->x <<endl /*<< counter<<starttime -  clock() <<"#" *//*<< move2->cost<<"-"<<move2->predict<<calc_time<< " "<<(starttime - clock())*/ ;//((float) calc_time)/(starttime - clock());
	return 0;
}

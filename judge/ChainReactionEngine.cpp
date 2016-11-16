

/* 
 * File:   ChainReactionEngine.cpp
 * Author: kefir
 * 
 * Created on November 15, 2016, 3:23 PM
 */


#include "ChainReactionEngine.h"
#include "ChainReactionPipeComunicator.h"

#define OUT_RANGE(x,y) (x >= 0 && y >= 0 && x < N  && y < N )

ChainReactionEngine::ChainReactionEngine(int nn, int firsrtStep)
{

	firsrtPlayer = firsrtStep;

	N = nn;

	mas = (int**) malloc(N * sizeof(int*));

	for (int i = 0; i < N; i++) {

		mas[i] = (int*) malloc(N * sizeof(int));
	}

}

ChainReactionEngine::ChainReactionEngine(const ChainReactionEngine & orig)
{
}

ChainReactionEngine::~ChainReactionEngine()
{
	for (int i = 0; i < N; i++) {
		delete(mas[i]);
	}
	delete(mas);
}

void ChainReactionEngine::init(string command1, string command2)
{


	players[0].init(1, command1);
	players[1].init(2, command2);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			mas[i][j] = 0;

	initStep();

}

void ChainReactionEngine::start(ChainReactionPipeComunicator &com)
{

	this->com = &com;
	Move currentMove;

	while (whoWon() == 0 || getCurrentStep() < 3) {

		com.sendPlacement(mas, &players[getCurrentPlayer() - 1], getCurrentStep());

		
		com.getAnswer(&currentMove);

		currentMove.player = getCurrentPlayer();
		currentPlayer8 = currentMove.player << 3;

		doMove(currentMove);

		addStep();
		changePlayer();
		sleep(1);
	}
	move(2,50);
	printw("Won %d !    Step:%d", whoWon(), getCurrentStep());
	refresh();

}

bool ChainReactionEngine::hasNextMove()
{
	return whoWon() == 0 && this->getCurrentStep() > 2;
}

int ChainReactionEngine::whoWon()
{
	int cc = 0;
	int cp = 0;
	
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
		{
			if ((mas[y][x] & 24) == 8)
				cc++;
			else if ((mas[y][x] & 24) == 16)
				cp++;
		}
	if (cc == 0) return 1;
	else if (cp == 0) return 2;

	return 0;

}

void ChainReactionEngine::doExplosion(int mx, int my)
{

	int x, y;

	mas[my][mx] = 0;

	if (mx > 0) {
		x = mx - 1;
		y = my;
		doSpark(x, y);
	}
	if (mx < N - 1) {
		x = mx + 1;
		y = my;
		doSpark(x, y);
	}
	if (my > 0) {
		x = mx;
		y = my - 1;
		doSpark(x, y);
	}
	if (my < N - 1) {
		x = mx;
		y = my + 1;
		doSpark(x, y);
	}
	com->visualOut(mas);
	

}

void ChainReactionEngine::doSpark(int x, int y)
{

	if (mas[y][x ] == 0)
		mas[y][x ] = currentPlayer8 + 1;
	else {
		mas[y][x ]++;
		mas[y][x ] &= 7;
		mas[y][x ] |= currentPlayer8;
	}

	int rez = 0;
	if (y == 0 || y == N)
		rez++;
	if (x == 0 || x == N)
		rez++;
	if ((mas[y][x ] & 7) + rez == 4)
		doExplosion(x, y);
	
	com->visualOut(mas);
}

void ChainReactionEngine::doMove(Move m)
{

	move(N,40);
	printw("Move  %d %d         %d",m.y, m.x, mas[m.y][m.x]);
	refresh();
	
	if (mas[m.y][m.x ] == 0)
		mas[m.y][m.x ] = currentPlayer8 + 1;
	else
		mas[m.y][m.x ]++;

	int rez = 0;
	if (m.y == 0 || m.y == N)
		rez++;
	if (m.x == 0 || m.x == N)
		rez++;
	if ((mas[m.y][m.x ] & 7) + rez == 4)
	{
		move(0,30);
		printw("Explosion  %d %d   %d",m.y, m.x, mas[m.y][m.x]);
		refresh();
		doExplosion(m.x, m.y);
	}
}

/* 
 * File:   ChainReactionEngine.h
 * Author: kefir
 *
 * Created on November 15, 2016, 3:44 PM
 */

#include<unistd.h>
#include<string>

#include "GameEngine.h"
//#include "pipeCommunicator.h"
#include "ChainReactionPipeComunicator.h"

#ifndef CHAINREACTIONENGINE_H
#define CHAINREACTIONENGINE_H

class ChainReactionEngine : GameEngine {
public:

    Player players[2];
    int currentPlayer8;

    ChainReactionEngine(int nn, int firsrtStep);
    ChainReactionEngine(const ChainReactionEngine& orig);
    virtual ~ChainReactionEngine();

    void init(string comand1, string command2);

    void start(ChainReactionPipeComunicator &com);

    void doMove(Move move);

private:

    ChainReactionPipeComunicator *com;
    int N;

    int **mas;

    bool hasNextMove() override;
    int whoWon();

    void doExplosion(int mx, int my);
    void doSpark(int x, int y);

};

#endif /* CHAINREACTIONENGINE_H */


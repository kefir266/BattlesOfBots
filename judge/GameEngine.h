/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameEngine.h
 * Author: kefir
 *
 * Created on July 11, 2016, 4:31 PM
 */

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Player.h"


class GameEngine {
public:
    
    Player players[];
    GameEngine();
    
    GameEngine(int nn,
            int numberOfPlayers,
            int firsrtStep);
    GameEngine(const GameEngine& orig);
    virtual ~GameEngine();
    void init();
    virtual void start();
    int getFirstPlayer();
    int firsrtPlayer;
    void addStep();
       void initStep();
   
   int getStep();
      void changePlayer();
   int getCurrentPlayer();
   int getCurrentStep();
    
private:
    int numberOfPlayers;
    
    int N;
    int nStep;
    int currentPlayer;
    
    virtual bool hasNextMove();
    

};

#endif /* GAMEENGINE_H */


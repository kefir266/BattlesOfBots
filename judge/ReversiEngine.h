/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReversiEngine.h
 * Author: kefir
 *
 * Created on July 14, 2016, 5:07 PM
 */
#include<unistd.h>
#include<string>

#include "GameEngine.h"
#include "pipeCommunicator.h"

#ifndef REVERSIENGINE_H
#define REVERSIENGINE_H

class ReversiEngine : GameEngine {
public:
    
    Player players[2];
    
    ReversiEngine(int nn,
            int firsrtStep);
    ReversiEngine(const ReversiEngine& orig);
    virtual ~ReversiEngine();
    
     void init(string comand1, string command2);
    
   void start(pipeCommunicator *com); 


   
   void placeMove(Move move);
private:

    
    int N;
    
    int **mas;
    
    
    
    bool hasNextMove() override;
    int whoWon();
    
};

#endif /* REVERSIENGINE_H */


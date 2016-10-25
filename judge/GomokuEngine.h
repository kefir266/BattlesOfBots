/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GomokuEngine.h
 * Author: kefir
 *
 * Created on July 11, 2016, 5:13 PM
 */

#ifndef GOMOKUENGINE_H
#define GOMOKUENGINE_H

#include "GameEngine.h"
#include "pipeCommunicator.h"


class GomokuEngine : GameEngine{
public:
    
    Player players[2];
    
    GomokuEngine(int nn,
            int firsrtStep);
    GomokuEngine(const GomokuEngine& orig);
    virtual ~GomokuEngine();
    
    void init(string comand1, string command2);
   
   void start(pipeCommunicator *com); 

   
   void placeMove(Move *move);
    
private:
    int N;
    
    int **mas;

    
    bool hasNextMove() override;
    

};

#endif /* GOMOKUENGINE_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GomokuEngine.cpp
 * Author: kefir
 * 
 * Created on July 11, 2016, 5:13 PM
 */

#include<unistd.h>

#include "GomokuEngine.h"

GomokuEngine::GomokuEngine(int nn,
            int firsrtStep) {
    
    firsrtPlayer = firsrtStep;
    N = nn;
    mas = (int**) malloc(N * sizeof(int*));
    for (int i = 0; i < N ; i++){
        mas[i] = (int*) malloc(N * sizeof(int));
    }
    //GameEngine(nn,2,firsrtStep);
}

GomokuEngine::GomokuEngine(const GomokuEngine& orig) {
}

GomokuEngine::~GomokuEngine() {
}

void GomokuEngine::init(string command1, string command2){

   
    players[0].init(1,command1);
    players[1].init(2,command2);
    
    for (int i = 0 ; i < N ; i++)
        for(int j = 0; j < N; j++)
            mas[i][j] = 0;
    
    initStep();
    
}

void GomokuEngine::start(pipeCommunicator *com) {
    
    Move move;
    
    while (hasNextMove()){
    
        com->sendPlacement(mas, &players[getCurrentPlayer()-1], getCurrentStep());
        com->getAnswer(&move);
        
        placeMove(&move);
        addStep();
        changePlayer();
        sleep(1);
    }
    

}

bool GomokuEngine::hasNextMove(){
    return getCurrentStep()<= N * N;
}



void GomokuEngine::placeMove(Move* move){
    mas[move->y][move->x] = getCurrentPlayer();
}
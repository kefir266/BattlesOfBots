/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ReversiEngine.cpp
 * Author: kefir
 * 
 * Created on July 14, 2016, 5:07 PM
 */

#include "ReversiEngine.h"


#define OUT_RANGE(x,y) (x >= 0 && y >= 0 && x < N  && y < N ) 

struct Dir {
	int x,y;

} dir[8] = {{1,1},{1,0},{0,1},{1,-1},{-1,-1},{-1,0},{0,-1},{-1,1}};


ReversiEngine::ReversiEngine(int nn,
            int firsrtStep) {
    
    firsrtPlayer = firsrtStep;
    N = nn;
    mas = (int**) malloc(N * sizeof(int*));
    for (int i = 0; i < N ; i++){
        mas[i] = (int*) malloc(N * sizeof(int));
    }
    
}

ReversiEngine::ReversiEngine(const ReversiEngine& orig) {
}

ReversiEngine::~ReversiEngine() {
}

void ReversiEngine::init(string command1, string command2){

   
    players[0].init(1,command1);
    players[1].init(2,command2);
    
    for (int i = 0 ; i < N ; i++)
        for(int j = 0; j < N; j++)
            mas[i][j] = 0;
    
    mas[4][4]=2;
    mas[5][4]=1;
    mas[4][5]=1;
    mas[5][5]=2;
    
    initStep();
    
}

void ReversiEngine::start(pipeCommunicator *com) {
    
    Move move;
    
    while (hasNextMove()){
        
        com->sendPlacement(mas, &players[getCurrentPlayer()-1], getCurrentStep());
        
        com->getAnswer(&move);
        
        move.player = getCurrentPlayer();
        
        placeMove(move);
        
        addStep();
        changePlayer();
        sleep(1);
    }
    cout<<"Won "<< whoWon()<<endl;

}

bool ReversiEngine::hasNextMove(){
    return getCurrentStep()<= N * N-3;
}

int ReversiEngine::whoWon(){
    int p[4]={0};
    for (int y= 0; y < N; y++)
        for(int x = 0; x < N; x++)
            p[mas[y][x]]++;
    if(p[1]>p[2]) return 1;
    else if (p[1] < p[2]) return 2;
    
    return 0;
            
}


void ReversiEngine::placeMove(Move m){
    
    int nRevers = 0;
        for (int v = 0; v < 8; v++){
        
        int dx = dir[v].x;
        int dy = dir[v].y;
        int x = m.x +dx;
        int y = m.y + dy;
        
        bool isLocked = false;
        bool isFirst = true;
        
        int nV = 0;
        
        while (OUT_RANGE(x,y)){
            if (mas[y][x] == (3 - m.player)){
                isFirst = false;
                y += dy;
                x += dx;
                continue;
            }
            isLocked = (mas[y][x] == m.player);
            break;
            
        }
        
        if (isLocked && ! isFirst){
            
            int x = m.x +dx;
            int y = m.y + dy;
            
            while (OUT_RANGE(x,y) && mas[y][x] == (3 - m.player)){
                mas[y][x] = m.player;
                nRevers++;
                y += dy;
                x += dx;
            }
        }
        

    }
    mas[m.y][m.x] = getCurrentPlayer();
}
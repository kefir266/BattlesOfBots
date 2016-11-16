/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: kefir
 *
 * Created on July 11, 2016, 5:05 PM
 */



#include<iostream>
#include<string>
#include <cstdlib>
#include<fstream>
#include<stdio.h>
#include<cstring>

//#include "ReversiEngine.h"
#include "ChainReactionEngine.h"
#include "ChainReactionPipeComunicator.h"

//#include "GomokuEngine.h"

//#define N 10
#define N 5

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    
    //GomokuEngine *gomoku = new GomokuEngine(N,1);
    //ReversiEngine *game = new ReversiEngine(N, 1);
	ChainReactionEngine game(N, 1);
    
	char *prg_player1, *prg_player2;
	if (argc == 2) {
		prg_player1 = argv[1];
		prg_player2 = prg_player1;
		}	
	else if (argc == 3) {
		prg_player1 = argv[1];
		prg_player2 = argv[2]; 
		}
	else return 1;
        
        game.init(prg_player1,prg_player2);
        
    cout << "Start game"<<endl;
    

    //pipeCommunicator *com = new pipeCommunicator();
    ChainReactionPipeComunicator com;

    game.start(com);
	
    return 0;
}


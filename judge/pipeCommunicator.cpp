/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pipeCommunicator.cpp
 * Author: kefir
 * 
 * Created on July 11, 2016, 8:08 PM
 */

#include "pipeCommunicator.h"


pipeCommunicator::pipeCommunicator() {
    
	this->stepsLog.clear();
	wnd = initscr();
}

pipeCommunicator::pipeCommunicator(const pipeCommunicator& orig) {
}

pipeCommunicator::~pipeCommunicator() {
    
    fclose(fd);
    endwin();
}

void pipeCommunicator::sendPlacement(int** mas, Player *player, int step){

    clear();
    
    fs.open("move.dat");
    
    for (int i = 0; i < 10; i++) {
        move(i,0);
        for (int j = 0; j < 10; j++) {
            fs << mas[i][j]<<" ";
            
            if (mas[i][j] == 0) printw(". ");
            else printw("%d ",mas[i][j]);
        }
        fs <<endl;
        
    }
    
    
    fs << player->getNumber()<<endl;
    fs.close();
    
    move(10,0);
    printw("%d %d", player->getNumber(),step);
    refresh();
    
	string scmd = "cat move.dat | " +  player->getCommand();
	
	fd = popen(scmd.c_str() ,"re");
                
	if (!fd) exit(1);
}

void pipeCommunicator::getAnswer(Move *mv){

    
    fscanf(fd,"%d %d %s", &mv->x, &mv->y , &mv->answer);
    stepsLog.insert(stepsLog.key_comp(),mv->answer);
    pclose(fd);
    
}

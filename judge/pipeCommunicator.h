/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pipeCommunicator.h
 * Author: kefir
 *
 * Created on July 11, 2016, 8:08 PM
 */

#include<iostream>
#include<fstream>
#include <cstdlib>
#include<stdio.h>
#include<curses.h>
#include<unistd.h>

#include "Player.h"

#ifndef PIPECOMMUNICATOR_H
#define PIPECOMMUNICATOR_H

using namespace std;

class pipeCommunicator {
public:
    pipeCommunicator();
    pipeCommunicator(const pipeCommunicator& orig);
    virtual ~pipeCommunicator();
    
    void sendPlacement(int** mas, Player *player,int step);
    void getAnswer(Move *move);
    
private:
    ofstream fs;
    FILE *fd; 
    WINDOW *wnd;
    
};

#endif /* PIPECOMMUNICATOR_H */


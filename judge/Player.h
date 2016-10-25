/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: kefir
 *
 * Created on July 11, 2016, 4:39 PM
 */


#include  <string>
#include <cstring>

#ifndef PLAYER_H
#define PLAYER_H

using namespace std;

struct Move {
    int x,y;
    int player;
};

class Player {
public:
    
    Player();
    Player(int number, string comand);
    Player(const Player& orig);
    virtual ~Player();
    
    void init(int num, string command);
    
    string getCommand();
    int getNumber();
    
private:
    int number;
    string command;

};

#endif /* PLAYER_H */


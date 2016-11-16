
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
    string answer;
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


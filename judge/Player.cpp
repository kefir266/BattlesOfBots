/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.cpp
 * Author: kefir
 * 
 * Created on July 11, 2016, 4:39 PM
 */

#include "Player.h"

Player::Player(){}

Player::Player(int number, string comand) {
    this->number = number;
}

Player::Player(const Player& orig) {
}

Player::~Player() {
}

void Player::init(int num, string command){
    number = num;
    this->command = command;
}

string Player::getCommand(){

    return command;
}

int Player::getNumber(){

    return number;
}
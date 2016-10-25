/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameEngine.cpp
 * Author: kefir
 * 
 * Created on July 11, 2016, 4:31 PM
 */

#include "GameEngine.h"

GameEngine::GameEngine(){}

GameEngine::GameEngine(int nn,
        int numberOfPlayers,
            int firsrtStep) {
    this->numberOfPlayers = numberOfPlayers;
    firsrtPlayer = firsrtStep;
    N = nn;
}

GameEngine::GameEngine(const GameEngine& orig) {
}

GameEngine::~GameEngine() {
}

void GameEngine::init(){
  
}

void GameEngine::start(){}

bool GameEngine::hasNextMove(){}

int GameEngine::getFirstPlayer(){

    return firsrtPlayer;
}

void GameEngine::addStep() {
    nStep++;
}
void GameEngine::initStep(){
    nStep = 1;
    currentPlayer = firsrtPlayer;
}

int GameEngine::getStep(){

    return nStep;
}

void GameEngine::changePlayer(){

    currentPlayer = 3 - currentPlayer;
}

int GameEngine::getCurrentPlayer(){

    return currentPlayer;
}

int GameEngine::getCurrentStep(){
    return nStep;
}
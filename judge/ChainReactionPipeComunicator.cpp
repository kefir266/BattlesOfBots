/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChainReactionPipeComunicator.cpp
 * Author: kefir
 * 
 * Created on November 15, 2016, 4:34 PM
 */

#include "ChainReactionPipeComunicator.h"

#define N 5

void ChainReactionPipeComunicator::outPlayerQuantity(int place)
{
	int p = ((place & 24) >> 3);
	int q = (place & 7);
	fs << p << q << " ";
}

void ChainReactionPipeComunicator::visualOut(int** mas)
{
	clear();
	for (int y = 0; y < N; y++) {
		move(y, 0);
		for (int x = 0; x < N; x++) {

			if (mas[y][x] == 0) printw(" .. ");
			else {
				int p = ((mas[y][x] & 24) >> 3);
				int q = (mas[y][x] & 7);
				printw("%d%d  ", p, q);
			}
		}

	}
	usleep(500);
	move(N, 0);
	printw("Player:%d   STEP:%d", currentPlayer, currentStep);
	refresh();
}

void ChainReactionPipeComunicator::sendPlacement(int** mas, Player *player, int step)
{

	currentStep = step;
	currentPlayer = player->getNumber();

	fs.open("move.dat");

	for (int y = 0; y < N; y++) {

		for (int x = 0; x < N; x++) {
			outPlayerQuantity(mas[y][x]);

		}
		fs << endl;

	}

	visualOut(mas);


	fs << player->getNumber() << endl;
	fs.close();

	

	string scmd = "cat move.dat | " + player->getCommand();

	fd = popen(scmd.c_str(), "re");

	if (!fd) exit(1);
}
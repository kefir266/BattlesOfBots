// BotSavesPrincess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include <vector>
#include <string>


using namespace std;
void displayPathtoPrincess(int n, vector <string> grid){
	
	string s = grid[0];
	string mov;
	
	if (s.c_str()[0] == 'p') mov = "UP\nLEFT\n";
	if (s.c_str()[s.length()-1] == 'p') mov = "UP\nRIGHT\n";
	s = grid[n - 1];
	if (s.c_str()[0] == 'p') mov = "DOWN\nLEFT\n";
	if (s.c_str()[s.length()-1] == 'p') mov = "DOWN\nRIGHT\n";
	n = n/2;
	while (n--){
		cout<<mov;
	}
}
int main(void) {

    int m;
    vector <string> grid;

    cin >> m;

    for(int i=0; i<m; i++) {
        string s; 
		cin >> s;
        grid.push_back(s);
    }

    displayPathtoPrincess(m,grid);

    return 0;
}
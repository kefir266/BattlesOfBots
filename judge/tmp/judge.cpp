#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#include<cstring>


using namespace std;

int main(int argc, char *argv[]){
	
	int movx,movy;

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
	cout << "Start game"<<endl;

	//////////////////
	//FILE *fd_move = fopen("move.dat","w");
	ofstream fs;
	fs.open("move.dat");

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			fs << "0 ";
		}
		fs <<endl;
	}
	fs << "1"<<endl;
	fs.close();
	/////////////////
	string str = prg_player1;
	string scmd = "cat move.dat | " + str;
	
	cout << scmd <<endl;
	FILE *fd = popen(scmd.c_str() ,"r");
	if (!fd) exit(1);

	fscanf(fd,"%d %d", &movx, &movy);
	cout << movx<<movy<<endl;
return 0;	
}

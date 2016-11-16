/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: kefir
 *
 * Created on July 14, 2016, 6:35 PM
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <ctime>
#include<map>
#include<set>

#define ABS(a)  ((a) < 0 ? -(a) : (a))

#define OUT_RANGE(x,y) (x >= 0 && y >= 0 && x < N  && y < N )

#define N 10
#define MAX_DEEP 7

clock_t endTime,warningTime;

using namespace std;

struct Dir {
        int x,y;

} dir[8] = {{1,1},{1,0},{0,1},{1,-1},{-1,-1},{-1,0},{0,-1},{-1,1}};

struct Move{

    int x,y;
    int cost;
    int player;
    Move() {
    }
    Move(int x, int y){
        this->x = x;
        this->y = y;
    }
};

class FieldReversi {

public:

    bool validate;

    int **mas;
    map<int,Move*> moves; // key is Y * NN + X
    set<int> returnable[2];

    //map<int,Move*> nextMoves; //key is a cost
    int NN;
    int recurN;

    FieldReversi(int n);

    FieldReversi(const FieldReversi& orig);

    ~FieldReversi();

    void doMove(Move m);

    void findNextMoves();

    bool isValidOnly(Move &m);

   // bool isValidMove(Move &m, int **mas, int *nRevers);

    static void loadField(FieldReversi *field);

    Move getBestMove();

    void setFirstPlayer(int currentPlayer);

    int calculateCost(Move m);

    int nextMove(Move m);

    int getBalance();

    bool timeOut();
    bool timeWarning();
    void startClock(clock_t overTime );
    int leftMoves;

private:

    int nPlayer[3];

    int currentPlayer;
    int myPlayer;

};

void FieldReversi::startClock(clock_t overTime){

    endTime = clock() + overTime;
    warningTime = clock()+ overTime/2;
}

bool FieldReversi::timeOut(){
    return (clock() > endTime);
}

bool FieldReversi::timeWarning(){
    return (clock() > warningTime);
}

bool FieldReversi::isValidOnly(Move &m){


    bool isValid = false;
    //int nRevers = 0;

    for (int v = 0; v < 8; v++){

        int dx = dir[v].x;
        int dy = dir[v].y;
        int x = m.x +dx;
        int y = m.y + dy;

        bool isLocked = false;
        bool isFirst = true;

        int nV = 0;

        while (OUT_RANGE(x,y)){
            if (mas[y][x] == (3 - m.player)){
                isFirst = false;
                nV++;

                x += dx;
                y += dy;
                continue;
            }
            isLocked = (mas[y][x] == m.player);
            break;

        }
        if (isLocked && ! isFirst){

            int x = m.x +dx;
            int y = m.y + dy;

            while (OUT_RANGE(x,y) && mas[y][x] == (5 - m.player)){

                returnable[m.player-1].insert(y*NN+x);
                y += dy;
                x += dx;
            }
            isValid = true;

        }
    }
    return isValid;
}

void FieldReversi::doMove(Move m){

    for (int v = 0; v < 8; v++){

        int dx = dir[v].x;
        int dy = dir[v].y;
        int x = m.x +dx;
        int y = m.y + dy;

        bool isLocked = false;
        bool isFirst = true;

        while (OUT_RANGE(x,y)){
            if (mas[y][x] == (3 - m.player)){
                isFirst = false;
                y += dy;
                x += dx;
                continue;
            }
            isLocked = (mas[y][x] == m.player);
            break;

        }
        if (isLocked && ! isFirst){

            int x = m.x +dx;
            int y = m.y + dy;

            while (OUT_RANGE(x,y) && mas[y][x] == (3 - m.player)){
                mas[y][x] = m.player;
                returnable[m.player-1].insert(y*NN+x);
//                nPlayer[m.player]++;
//                nPlayer[3-m.player]--;
                y += dy;
                x += dx;
            }
        }
    }
    mas[m.y][m.x ] = m.player;
}

FieldReversi::FieldReversi(int n){
        NN = n;
        recurN = 0;
        mas = (int**) malloc(n*sizeof(int*));
        for (int i = 0; i < n; i++)
            mas[i] = (int*) malloc(n* sizeof(int));
    }

FieldReversi::~FieldReversi(){
    for (int i = 0; i < NN; i++)
        delete(mas[i]);
    delete(mas);
    for(auto m : moves){
        delete(m.second);
    }
    moves.clear();
    returnable[0].clear();
    returnable[1].clear();
   /// nextMoves.clear();
}

FieldReversi::FieldReversi(const FieldReversi& orig){
    recurN = orig.recurN+1;
    NN = orig.NN;

    nPlayer[0] = 0;
    nPlayer[1] = 0;
    nPlayer[2] = 0;

    mas = (int**) malloc(NN*sizeof(int*));
    for (int y = 0; y< NN; y++){
            mas[y] = (int*) malloc(NN * sizeof(int));
            for (int x = 0; x < NN; x++){
                mas[y][x] = orig.mas[y][x];
                nPlayer[mas[y][x]]++;
            }
    }
    currentPlayer = 3 - orig.currentPlayer;




}

int FieldReversi::getBalance(){

    return  (leftMoves <= MAX_DEEP) ? 0 : ((nPlayer[currentPlayer]  - (int) returnable[currentPlayer-1].size())
            - (nPlayer[3 - currentPlayer] - (int) returnable[2 - currentPlayer].size())) + (nPlayer[currentPlayer] - nPlayer[3 - currentPlayer]) ;
}

void FieldReversi::findNextMoves(){

    Move *m = new Move();
        for (int y = 0; y < NN ; y++) {
            for (int x = 0; x < NN ; x++) {
                if (mas[y][x] == 0) {
                    m->x = x;
                    m->y = y;
                    m->player = currentPlayer;

                    if (isValidOnly(*m)){

                        //cost = getBalance() + calculateCost(m);

                        moves[y * NN+x] = m;
                        m = new Move();
                    }
                }
            }
        }
}

void FieldReversi::setFirstPlayer(int n){

    currentPlayer = n;
    myPlayer = n;

}

int FieldReversi::nextMove(Move move){

    if (recurN > MAX_DEEP ) 
        return -123456;

    FieldReversi nextField(*this);

    nextField.validate = true;

    nextField.doMove(move);


    nextField.findNextMoves();
    move.cost = nextField.getBalance() + calculateCost(move);
    int maxCost = -move.cost ;

    if (recurN < 3 || (! timeOut()) || (! timeWarning() && recurN < 4)){
        int cost = 0;

        if (nextField.moves.size() == 0 && (leftMoves > MAX_DEEP) ) 
            return -100000;

        for (auto m : nextField.moves){

            cost = nextField.nextMove(*m.second);
            int kMoves = (leftMoves <= MAX_DEEP) ? 0 : (leftMoves) *( (int)moves.size() - (int)nextField.moves.size()) * 10 ;
            if (cost == -123456){
               // maxCost = 0;
                break;
            }
            cost += kMoves;
            if (cost > maxCost)
            maxCost = cost ;

        }
    }


   //int kAttack = currentPlayer == myPlayer ? 2: 1;
    return - ((move.cost - maxCost) <<1 );
}

int FieldReversi::calculateCost(Move m){

    int cost = 0;
    if ((m.x == 0 && m.y == 0) || (m.x == 0 && m.y == NN-1 )
            ||( m.x == N -1 && m.y == 0) || (m.x == N -1 && m.y == N - 1)) 
            cost = 10000;

    return cost;
}

void FieldReversi::loadField(FieldReversi* field) {

    field->leftMoves = 0;
    field->moves.clear();
    int n[4] = {0,0,0};

    for (int y = 0; y < field->NN ; y++) {
        for (int x = 0; x < field->NN ; x++) {
            cin >> field->mas[y][x];
            n[field->mas[y][x]]++;
            if (!field->validate && field->mas[y][x] == 3){
                field->moves[y*field->NN + x] = new Move(x,y);
            }
        }
    }

    int currentPlayer;
    cin >> currentPlayer;
    field->setFirstPlayer(currentPlayer);

    if (field->validate) {
        Move *m = new Move();
        for (int y = 0; y < field->NN ; y++) {
            for (int x = 0; x < field->NN ; x++) {
                if (field->mas[y][x] == 0 || field->mas[y][x] == 3) {
                    m->x = x;
                    m->y = y;
                    m->player = currentPlayer;
                    field->leftMoves++;

                    if (field->isValidOnly(*m)){
                        //m->cost = calculateCost(m);

                        field->moves[y*field->NN+x] = m;
                        m = new Move();
                    }
                }
            }
        }

    }
    else
        field->leftMoves = n[3];

}

Move FieldReversi::getBestMove(){

    int cost;
    int maxCost = -1000000000;
    Move *bestMove;
   // bool isFirst = true;
    
    for (auto m : moves){
        if (moves.size()  == 1 ) 
            return *m.second;
        m.second->cost = calculateCost(*m.second);
        cost = - nextMove(*m.second) + m.second->cost;
        if (cost > maxCost) {
            maxCost = cost;
            bestMove = m.second;
            bestMove->cost = cost;
        }
    }
    return *bestMove;

}


/*
 *
 */
int main(int argc, char** argv) {


    FieldReversi *field = new FieldReversi(N);

    field->startClock(900000);

    field->validate = true; //for test system


    field->loadField(field);

    //field->findNextMoves();

    Move m = field->getBestMove();

    cout<< m.y<<" "<<m.x<<endl;
    cout<< m.cost<<endl;

    return 0;
}


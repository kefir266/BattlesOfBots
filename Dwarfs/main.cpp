
#include <cstdlib>
#include <sstream>
#include <iostream>

using namespace std;


void getXY(string &sS, int &x, int &y){
    
    stringstream sc;
    char c;
    char *cc = new char[3];
    char *fst;
    fst = cc;
    sc << sS;
    
    while(!sc.eof()){
        sc >> c;
        if (c >='9' ) { *cc = '\n'; break;}
        *cc++ = c;
        
    }
        
    x = atoi(fst);
    sc >> c;
    y = c - 'A' + 1;
}

int getQuad(int halfN,string &xy){
    
    int x,y;
    
    getXY(xy, x, y);
    return (halfN - x/2) + (halfN - y/2)*2;
    }

int solution(int N, string &S, string &T) {
    
    int maxInQuad = 0;
    int nQuad = 0;
    int halfN = N / 2;
    int quad = 0;
    int maxDwarfs = 0;
    int aBarrels[4]={0,0,0,0};
    //int aDwarfs[4]={0,0,0,0};
    string xy;
    
    stringstream str;
    str << S;
    while (!str.eof()) {
        str >> xy;
        quad = getQuad(halfN, xy);
        if (++aBarrels[quad] > maxInQuad){
            maxInQuad = aBarrels[quad];
            nQuad = quad;
            }
        
        }
    int n1 = (nQuad - 1) < 0 ? 3 : nQuad - 1;
    int n2 = (nQuad + 1) > 3 ? 0 : nQuad + 1;
    
    if (aBarrels[n1] > aBarrels[n2]) {
        maxDwarfs = (halfN * halfN - maxInQuad) * 2 + (halfN * halfN - aBarrels[n1])* 2;
    }
    else
        maxDwarfs = (halfN * halfN - maxInQuad) * 2 + (halfN * halfN - aBarrels[n2])* 2;
        
    int nDwarfs = 0;
    str.clear();
    str << T;
    while (!str.eof()){
        str >> xy;
        
        quad = getQuad(halfN, xy);
        if (++nDwarfs > maxDwarfs) return -1;
        
        }
    return maxDwarfs - nDwarfs;
}


int main(int argc, char** argv) {
    
    string S = "1B 1C 4B 1D 2A";
    string T = "3B 2D";
    cout << solution(4,S,T);
    return 0;
}


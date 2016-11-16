
/* 
 * File:   ChainReactionPipeComunicator.h
 * Author: kefir
 *
 * Created on November 15, 2016, 4:34 PM
 */

#include "pipeCommunicator.h"

#ifndef CHAINREACTIONPIPECOMUNICATOR_H
#define CHAINREACTIONPIPECOMUNICATOR_H

class ChainReactionPipeComunicator : public pipeCommunicator {
public:

    ChainReactionPipeComunicator():pipeCommunicator(){}
    
    void visualOut(int** mas);
    
    void sendPlacement(int** mas, Player *player,int step) ;
    //void getAnswer(Move *move) ;
    
private:
    
    void outPlayerQuantity( int);
    int currentStep;
    int currentPlayer;

};

#endif /* CHAINREACTIONPIPECOMUNICATOR_H */


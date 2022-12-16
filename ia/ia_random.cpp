#include <iostream>

pion* getRandomPion(){
    while(true){
        pion* pion = getPion(rand() % width);
        if(pion != NULL){
            possibility* possibilityList = getPossibility(pion);

            if(possibilityList->length > 0){
                return pion;
            }
        }
    }
    return NULL;
}

direction getRandomMove(pion* pion){
    possibility* possibilityList = getPossibility(pion);

    while(true){
        int move = rand() % 3;

        switch(move){
            case 0:
                if(possibilityList->forward){ return FORWARD; }
                break;

            case 1:
                if(possibilityList->diagonal_left){ return DIAGONAL_LEFT; }
                break;

            case 2:
                if(possibilityList->diagonal_right){ return DIAGONAL_RIGHT; }
                break;
        }
    }
}
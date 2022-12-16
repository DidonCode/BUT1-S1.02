#include <iostream>

pion* getEasyPion(){
    int forwardTest = 0, diagonalTest = 0;
    
    while(true){
        pion* pion = getPion(rand() % width);
        if(pion != NULL){
            possibility* possibilityList = getPossibility(pion);

            if(possibilityList->forward && table[pion->position.x][pion->position.y - 2] == NULL){
                return pion;
            }
            else if(forwardTest >= width * 2){
                if(diagonalTest >= width * 2){
                    if(possibilityList->length > 0){
                        return pion;
                    }
                }
                if(possibilityList->diagonal_left || possibilityList->diagonal_right){
                    return pion;
                }else{
                    diagonalTest++;
                }
            }else{
                forwardTest++;
            }
        }
    }
    return NULL;
}

int getEasyMove(pion* pion){
    possibility* possibilityList = getPossibility(pion);

    if(possibilityList->forward){
        return FORWARD;
    }else{
        if(possibilityList->diagonal_left){
            return DIAGONAL_LEFT;
        }
        if(possibilityList->diagonal_right){
            return DIAGONAL_RIGHT;
        }
    }
}
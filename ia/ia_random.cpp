#include <iostream>

pion* getRandomPion(int round){
    while(true){
        int id = 0;
        id = rand() % width;

        if(round % 2 == 1){ id = id + width; }

        pion* pion = getPion(id);
        if(pion != NULL){
            if(getPossibility(pion)->length > 0){
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
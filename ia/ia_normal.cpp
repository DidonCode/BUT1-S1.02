#include <iostream>

int normalThec = 0;

pion* defense(int round){
    pion* pion = NULL;

    while(pion == NULL){
        pion = getRandomPion();

        if(pion != NULL){
            possibility* possibilityList = getPossibility(pion);
            
            if(possibilityList->length > 0){
                if(round % 2 == 0){
                    if(pion->position.y >= 0 && pion->position.y < 2){
                        if(possibilityList->diagonal_left || possibilityList->diagonal_right){
                            return pion;
                        }
                    } 
                }else{
                    if(pion->position.y >= height - 2 && pion->position.y < height){
                        if(possibilityList->diagonal_left || possibilityList->diagonal_right){
                            return pion;
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

pion* rush(int round){
    pion* pion = NULL;

    while(pion == NULL){
        pion = getRandomPion();

        if(pion != NULL){
            possibility* possibilityList = getPossibility(pion);

            if(possibilityList->length > 0){
                if(round % 2 >= 1){
                    if(possibilityList->length > 0){
                        if(pion->position.y >= 0 && pion->position.y < 2){
                            if(possibilityList->length > 0){
                                return pion;
                            }
                        } 
                    }else{
                        if(pion->position.y >= height - 2 && pion->position.y < height){
                            if(possibilityList->length > 0){
                                return pion;
                            }
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

pion* getNormalPion(int round){
    pion* pion = rush(round);

    if(pion == NULL){
        pion = defense(round);

        if(pion == NULL){
            normalThec = 0;
            pion = getRandomPion();

        }else{
            normalThec = 2;
        }
    }else{
        normalThec = 1;
    }

    return pion;
}

direction getNormalMove(pion* pion){
    if(normalThec == 1){
        return FORWARD;
    }
    else if(normalThec == 2){
        possibility* possibilityList = getPossibility(pion);

        if(possibilityList->diagonal_left){
            return DIAGONAL_LEFT;
        }

        if(possibilityList->diagonal_right){
            return DIAGONAL_RIGHT;
        }
    }else{
        return getRandomMove(pion);
    }
}
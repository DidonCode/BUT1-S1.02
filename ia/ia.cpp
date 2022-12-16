#include <iostream>
#include <time.h>

#include "ia_random.cpp"
#include "ia_easy.cpp"
#include "ia_normal.cpp"
#include "ia_hard.cpp"

enum difficulty{NONE = -1, RANDOM = 0, EASY = 1, NORMAL = 2, HARD = 3 };

difficulty iaDifficulty = NONE; 

void setDifficulty(int difficulty){
     switch(difficulty){
        case RANDOM:
            iaDifficulty = RANDOM;
            cout<<"difficult: random"<<endl;
            break;

        case EASY:
            iaDifficulty = EASY;
            cout<<"difficult: easy"<<endl;
            break;

        case NORMAL:
            iaDifficulty = NORMAL;
            cout<<"difficult: normal"<<endl;
            break;

        case HARD:
            iaDifficulty = HARD;
            cout<<"difficult: hard"<<endl;
            break;
     }
}

pion* getIAPion(int round){
    if(iaDifficulty == RANDOM){ return getRandomPion(); }
    if(iaDifficulty == EASY){ return getEasyPion(); }
    if(iaDifficulty == NORMAL){ return getNormalPion(round); }
    if(iaDifficulty == HARD){ return getNormalPion(round); }
}

int getIAMove(pion* pion){
    if(iaDifficulty == RANDOM){ return getRandomMove(pion); }
    if(iaDifficulty == EASY){ return getEasyMove(pion); }
    if(iaDifficulty == NORMAL){ return getNormalMove(pion); }
    if(iaDifficulty == HARD){ return getNormalMove(pion); }
}

void iaTrain(){
    int round = 0;

    while(!verification()){
        int pion, direction;
        bool valid = false;

        if(round % 2 == 0){
            setDifficulty(RANDOM);
        }else{
            setDifficulty(NORMAL);
        }

        while(!valid){
            bool good = false;

            while(!good){
                pion = getIAPion(round)->id;
                if(round % 2 != 0){ pion = pion + width; };

                if(getPion(pion) != NULL){
                    cout<<"non"<<" "<<getPion(pion)<<endl;
                    possibility* possibilityList = getPossibility(getPion(pion));
                    if(possibilityList->length > 0){
                        direction = getIAMove(getPion(pion));
                        good = true;
                    }
                }

                //cout<<"non"<<" "<<getPion(pion)<<endl;
            }
            valid = move(getPion(pion), direction);
        }

        render(round);

        round++;
    }
}

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
            //cout<<"difficult: random"<<endl;
            break;

        case EASY:
            iaDifficulty = EASY;
            //cout<<"difficult: easy"<<endl;
            break;

        case NORMAL:
            iaDifficulty = NORMAL;
            //cout<<"difficult: normal"<<endl;
            break;

        case HARD:
            iaDifficulty = HARD;
            //cout<<"difficult: hard"<<endl;
            break;
    }
}

pion* getIAPion(int round){
    if(iaDifficulty == RANDOM){ return getRandomPion(round); }
    if(iaDifficulty == EASY){ return getEasyPion(); }
    if(iaDifficulty == NORMAL){ return getNormalPion(round); }
    if(iaDifficulty == HARD){ return getHardPion(round); }
}

direction getIAMove(pion* pion){
    if(iaDifficulty == RANDOM){ return getRandomMove(pion); }
    if(iaDifficulty == EASY){ return getEasyMove(pion); }
    if(iaDifficulty == NORMAL){ return getNormalMove(pion); }
    if(iaDifficulty == HARD){ return getHardMove(pion); }
}

void iaInit(int team){
    if(iaDifficulty == HARD){ initIaHard(team); }
}

void iaDestroy(){
    if(iaDifficulty == HARD){ destroyIaHard(); }
}

int iaHardVictory = 0;

int getHardVictory(){
    return iaHardVictory;
}

void iaTrain(){
    int round = 0;
    difficulty iaUp = RANDOM;
    difficulty iaDown = HARD;
    //render(round);

    setDifficulty(iaDown);
    iaInit(2);

    while(verification() == 0){
        int pion, direction;
        bool valid = false;

        if(round % 2 == 0){
            setDifficulty(iaUp);
        }else{
            setDifficulty(iaDown);
        }

        while(!valid){
            bool good = false;

            while(!good){
                pion = getIAPion(round)->id;

                if(getPion(pion) != NULL && ((round % 2 == 0 && pion < width) || (round % 2 == 1 && pion >= width))){
                    if(getPossibility(getPion(pion))->length > 0){
                        direction = getIAMove(getPion(pion));
                        good = true;
                    }
                }
            }
            //cout<<"debug game "<<pion<<" "<<round<<" "<<(round % 2)<<" "<<iaDifficulty<<endl;
            valid = move(getPion(pion), direction);
        }
        //cout<<"move finish"<<getRecentStrike()<<endl;

        setLastStrike(getRecentStrike());
        //cout<<"strike 1"<<endl;
        setRecentStrike(pion, direction);

        //cout<<round<<" "<<getRecentStrike()->data<<" "<<endl;
        if(iaDifficulty == HARD){ addHardPossibility(); }

        render(round);

        round++;
    }

    int winner = verification();
    if(winner == 1){
        cout<<"L'equipe 1 a gagne"<<endl;
    }
    if(winner == 2){
        cout<<"L'equipe 2 a gagne"<<endl;
        iaHardVictory++;
    }
    if(winner == 3){
        cout<<"Egalité"<<endl;
    }

    if(iaDifficulty == HARD){
        setDifficulty(iaDown);
        addHardPossibility();
    }
    
    iaDestroy();
}

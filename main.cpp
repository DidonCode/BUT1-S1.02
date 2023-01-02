#include <iostream>
#include <windows.h>

#include "game/dame.cpp"
#include "ia/ia.cpp"
#include "game/cursor.cpp"

using namespace std;

void inputGrid(){
    int widthTemp = 0, heightTemp = 0;

    while(widthTemp < 3 || widthTemp > 15){
        cout<<"Largeur de la grille ? (min: 3, max: 10)"<<endl;
        cin>>widthTemp;
    }

    while(heightTemp < 3 || heightTemp > 15){
        cout<<"Longueur de la grille ? (min: 3, max: 10)"<<endl;
        cin>>heightTemp;
    }

    width = widthTemp;
    height = heightTemp;
}

void playGame(){
    int round = 0;

    while(verification() == 0){
        int pion;
        direction direction;
        bool valid = false;

        while(!valid){
            render(round);
             
            if(iaDifficulty == NONE || (iaDifficulty != NONE && round % 2 == 0)){
                bool good = false; 
                while(!good){
                    cout<<"Numero du pion: ";

                    int i = 0, max = width;
                    if(round % 2 == 1){ i = width; max = width * 2; }

                    for(i; i < max; i++){
                        if(getPion(i) != NULL){
                            cout<<i<<" ";
                        }
                    }
                    cout<<endl;
                    cin>>pion;

                    if((round % 2 == 0 && pion >= 0 && pion < width) || (round % 2 == 1 && pion >= width && pion < width * 2)){
                        if(getPossibility(getPion(pion))->length > 0){
                            good = true;
                        }else{
                            cout<<"Ce pion ne peut rien faire"<<endl;
                        }
                    }
                }

                good = false; 
                while(!good){
                    possibility* possibilityList = getPossibility(getPion(pion));
                    cout<<"Numero de la direction: ";

                    if(possibilityList->forward){ cout<<"devant = 0 "; }
                    if(possibilityList->diagonal_left){ cout<<"gauche = -1 "; }
                    if(possibilityList->diagonal_right){ cout<<"droite = 1 "; }
                    cout<<endl;
                   
                    int directionBuf;
                    cin>>directionBuf;

                    if(possibilityList->forward && directionBuf == 0){ direction = FORWARD; good = true; }
                    if(possibilityList->diagonal_left && directionBuf == -1){ direction = DIAGONAL_LEFT; good = true; }
                    if(possibilityList->diagonal_right && directionBuf == 1){ direction = DIAGONAL_RIGHT; good = true; }
                }
            }else{
                pion = getIAPion(round)->id;
                if(iaDifficulty != HARD){
                    pion = pion + width;
                }
                direction = getIAMove(getPion(pion));
            }
            
            valid = move(getPion(pion), direction);
        }

        setLastStrike(getRecentStrike());
        setRecentStrike(pion, direction);

        if(iaDifficulty == HARD){ addHardPossibility(); }

        clear();
        render(round);

        round++;
    }

    int winner = verification();
    if(winner == 1){
        cout<<"L'equipe 1 a gagne"<<endl;
    }
    if(winner == 2){
        cout<<"L'equipe 2 a gagne"<<endl;
    }
    if(winner == 3){
        cout<<"Egalité"<<endl;
    }

    if(iaDifficulty == HARD){
        setDifficulty(HARD);
        addHardPossibility();
    }

    iaDestroy();
}

void menu(){
    int choice = 0;

    inputGrid();
    init(width, height);

    while(choice < 1 || choice > 3){
        cout<<"Quel mode de jeux ?"<<endl;
        cout<<"1: Joueur contre Joueur"<<endl;
        cout<<"2: Joueur contre IA"<<endl;
        cout<<"3: Entrainement IA"<<endl;
        cin>>choice;

        if(choice == 2){
            
            int choice_difficulty = -1;
            
            while(choice_difficulty < 0 || choice_difficulty > 3){
                cout<<"Quel difficulte ?"<<endl;
                cout<<"0: Random"<<endl;
                cout<<"1: Facile"<<endl;
                cout<<"2: Normal"<<endl;
                cout<<"3: Difficile"<<endl;
                cin>>choice_difficulty;
            }
            setDifficulty(choice_difficulty);
            iaInit(2);

            clear();
            playGame();
        }
        else if(choice == 1){
            clear();
            playGame();
        }
        else if(choice == 3){
            clear();
            iaTrain();
        }
    }
}

int main(){
    system("color 80");
    srand(time(NULL));

    menu();
    
    // float test = 500.0;
    // int size = 5;
    // init(size, size);

    // for(int i = 0; i < test; i++){
    //     iaTrain();
    //     destroy();
    //     init(size, size);
    // }

    // cout<<"Nombre de victoire: "<<getHardVictory()<<"/"<<test<<endl;
    // cout<<"Un pourcentage de: "<<((getHardVictory() / test) * 100)<<"%"<<endl;

    destroy();

    return 0;
}
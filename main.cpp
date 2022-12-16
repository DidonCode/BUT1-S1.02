#include <iostream>
#include <windows.h>

#include "game/dame.cpp"
#include "ia/ia.cpp"
#include "game/file.cpp"
#include "game/cursor.cpp"

using namespace std;

pion* inputPion(int round){
    int pionId = 0, pionMin = 0, pionMax = 0;
    pion* pionCheck = NULL;

    if(round == 0){
        pionMin = 0; pionMax = width - 1;
    }else{
        pionMin = width; pionMax = width * 2 - 1;
    }
    pionId = pionMin;

    while(!keyPress(VK_BACK) && getPion(pionId) != NULL){
        pionCheck = getPion(pionId);

        if(keyPress(VK_LEFT) && pionId > pionMin){
            pionId -= 1;
        }
        if(keyPress(VK_RIGHT) && pionId < pionMax){
            pionId += 1;
        }
        if(pionCheck != NULL){
            setCursor(pionCheck->position.x, pionCheck->position.y);
        }
    }

    return pionCheck;
}

int inputDirection(pion* pion){
    possibility* possibilityList = getPossibility(pion);
    int directionNumber = 0, directionMin = 0, directionMax = 0;

    if(possibilityList->diagonal_left){ directionMin = -1; }
    if(possibilityList->diagonal_right){ directionMax = 1; }

    while(!keyPress(VK_BACK)){
        if(keyPress(VK_LEFT) && directionNumber > directionMin){
            directionNumber -= 1;
        }
        if(keyPress(VK_RIGHT) && directionNumber < directionMax){
            directionNumber += 1;
        }

        if(pion->id >= width){
            setCursor(pion->position.x + directionNumber, pion->position.y - 1);
        }else{
            setCursor(pion->position.x + directionNumber, pion->position.y + 1);
        }
    }

    return directionNumber; 
}

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

    while(!verification()){
        int pion, direction;
        bool valid = false;

        render(round);

        while(!valid){

            if(iaDifficulty == NONE || (iaDifficulty != NONE && round % 2 == 0)){
                if(console){
                    pion = inputPion(round % 2)->id;
                    Sleep(300);
                    direction = inputDirection(getPion(pion));
                }else{
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
                        if(possibilityList->diagonal_left){ cout<<"gauche = -1"; }
                        if(possibilityList->diagonal_right){ cout<<"droite = 1"; }
                        cout<<endl;

                        cin>>direction;

                        if(possibilityList->forward && direction == 0){ good = true; }
                        if(possibilityList->diagonal_left && direction == -1){ good = true; }
                        if(possibilityList->diagonal_right && direction == 1){ good = true; }
                    }
                }
            }else{
                pion = getIAPion(round)->id;
                pion = pion + width;
                direction = getIAMove(getPion(pion));
            }

            valid = move(getPion(pion), direction);
        }

        clear();
        render(round);

        round++;
    }
}

void menu(){
    int choice = 0;

    inputGrid();
    init(width, height);

    while(choice < 1 || choice > 2){
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
            clear();
            playGame();
        }
        else if(choice == 1){
            clear();
            playGame();
        }
        else if(choice == 3){
            setDifficulty(2);
            clear();
            iaTrain();
        }
    }
}

int main(){
    system("color 80");
    srand(time(NULL));

    HWND consoleWnd = GetConsoleWindow();
    DWORD dwProcessId;
    GetWindowThreadProcessId(consoleWnd, &dwProcessId);
    if (GetCurrentProcessId() == dwProcessId) { 
        console = true; //Powershell console 
    }

    menu();

    destroy();

    // string fileName = createName(10, 10);
    // createFile(database, fileName);

    // const char** datas = new const char*[3];
    // datas[0] = "Je";
    // datas[1] = "Suis";
    // datas[2] = "Richard";

    // writeFile(database, fileName, datas);
    // string* readDatas = NULL;
    // readDatas = readFile(database, fileName);

    // for(int i = 0; i < sizeof(readDatas) - 1; i++){
    //     cout<<i<<" "<<readDatas[i]<<endl;
    // }

    return 0;
}
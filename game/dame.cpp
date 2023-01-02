#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

enum direction{ FORWARD = 0, DIAGONAL_LEFT = -1, DIAGONAL_RIGHT = 1 };

struct coord{ 
    int x = 0, y = 0; };

struct pion{
    coord position;
    string color;
    char sign;
    int id = -1; };

struct possibility{
    bool forward = false;
    bool diagonal_left = false;
    bool diagonal_right = false;
    int length = 0; 
};

struct strike{
    string data = "";
    int pionId = -1;
    int direction = 0;
};

int width = 0, height = 0;
pion*** table = NULL;

strike* recentStrike = NULL;
strike* lastStrike = NULL;

void clear(){
    system("cls");
}

void render(int round){ 
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            string color, color2;
            int temp = x;

            if(y % 2 == 0){ temp = x + 1; }

            if(temp % 2 == 0){ 
                color = ";47m"; 
                color2 = "\033[30;47m"; 
            }else{ 
                color = ";40m"; 
                color2 = "\033[37;40m"; 
            }

            if(table[x][y] != NULL){ //⬤
                if(round % 2 == 0){
                    if(table[x][y]->id < width){
                        cout<<table[x][y]->color + color<<" "<<table[x][y]->id<<" "<<"\033[37;40m"; 
                    }else{
                        cout<<table[x][y]->color + color<<" ⬤ "<<"\033[37;40m";
                    }
                }else{
                    if(table[x][y]->id >= width){
                        cout<<table[x][y]->color + color<<" "<<table[x][y]->id<<" "<<"\033[37;40m"; 
                    }else{
                        cout<<table[x][y]->color + color<<" ⬤ "<<"\033[37;40m";
                    }
                }
            }else{
                cout<<color2<<"   "<<"\033[37;40m";
            }
        }

        cout<<endl;
    }

    cout<<endl;
}

void init(int width1, int height1){
    width = width1;
    height = height1;

    table = new pion**[width];

    for(int x = 0; x < width; x++){
        table[x] = new pion*[height];

        for(int y = 0; y < height; y++){
            
            if(y == 0 || y == height - 1){
                table[x][y] = new pion();
                if(y == 0){
                    table[x][y]->id = x;
                    table[x][y]->color = "\033[31";
                    table[x][y]->sign = 'X';
                }else{
                    table[x][y]->id = x + width;
                    table[x][y]->color = "\033[32";
                    table[x][y]->sign = 'O';
                }
                table[x][y]->position.x = x;
                table[x][y]->position.y = y;
            }else{
                table[x][y] = NULL;
            }
        }
    }
}

pion* getPion(int id){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            if(table[x][y] != NULL && table[x][y]->id == id){
                return table[x][y];
            } 
        }
    }
    return NULL;
}

pion*** getTable(){
    return table;
}

strike* getRecentStrike(){
    return recentStrike;
}

strike* getLastStrike(){
    return lastStrike;
}

void setRecentStrike(int pionId, int direction){
    pion* pion = getPion(pionId);
    recentStrike = new strike();
    recentStrike->data = to_string(pionId) + "/" + to_string(pion->position.x) + "!" + to_string(pion->position.y) +":" + to_string(direction);
    recentStrike->pionId = pionId;
    recentStrike->direction = direction;
}

void setLastStrike(strike* recentStrike){
    if(recentStrike != NULL){
        lastStrike = recentStrike;
    }
}

bool attack(pion* pion, int direction){
    if(pion != NULL){
        int x = pion->position.x;
        int y = pion->position.y;
        int distance = 0;

        if(pion->id >= width){ distance = -1; }else { distance = 1; }

        table[x][y] = NULL;

        if(direction == DIAGONAL_LEFT){
            if(pion->id >= width){
                if(x + distance >= 0 && y + distance >= 0 && table[x + distance][y + distance] != NULL){ //DIAGONAL_LEFT BAS
                    if(pion->sign != table[x + distance][y + distance]->sign){
                        pion->position.x += distance;
                        pion->position.y += distance;
                    }
                }
            }else{
                if(x - distance >= 0 && y + distance < height && table[x - distance][y + distance] != NULL){ //DIAGONAL_LEFT HAUT
                    if(pion->sign != table[x - distance][y + distance]->sign){
                        pion->position.x -= distance;
                        pion->position.y += distance;
                    }
                }
            }
        }
        
        //--------------\\

        if(direction == DIAGONAL_RIGHT){
            if(pion->id >= width){
                if(x - distance < width && y + distance >= 0 && table[x - distance][y + distance] != NULL){ //DIAGONAL_RIGHT BAS
                    if(pion->sign != table[x - distance][y + distance]->sign){
                        pion->position.x -= distance;
                        pion->position.y += distance;
                    }
                }
            }else{
                if(x + distance < width && y + distance < height && table[x + distance][y + distance] != NULL){ //DIAGONAL_RIGHT HAUT
                    if(pion->sign != table[x + distance][y + distance]->sign){
                        pion->position.x += distance;
                        pion->position.y += distance;
                    }
                }
            }
        }

        if(x != pion->position.x || y != pion->position.y){
            table[pion->position.x][pion->position.y] = pion;
            return true;
        }
    }
    return false;
}

bool move(pion* pion, int direction){
    if(pion != NULL){
        int x = pion->position.x;
        int y = pion->position.y;
        int distance = 0;

        if(pion->id >= width){ distance = -1; }else { distance = 1; }

        if(direction == FORWARD){
            if(y + distance < width && y + distance >= 0 && table[x][y + distance] == NULL){ //FORWARD

                table[x][y] = NULL;
                pion->position.y += distance;
                table[pion->position.x][pion->position.y] = pion;

                return true;
            }
        }else{
            return attack(pion, direction);
        }
    }
    return false;
}

possibility* getPossibility(pion* pion){
    int x = pion->position.x;
    int y = pion->position.y;
    int distance = 0;

    if(pion->id >= width){ distance = -1; }else { distance = 1; }

    possibility* possibilityList = new possibility();

    //--------------\\

    if(y + distance < width && y + distance >= 0 && table[x][y + distance] == NULL){ //FORWARD
        possibilityList->forward = true;
        possibilityList->length++;
    }

    //--------------\\

    if(pion->id >= width){
        if(x + distance >= 0 && y + distance >= 0 && table[x + distance][y + distance] != NULL){ //DIAGONAL_LEFT BAS
            if(pion->sign != table[x + distance][y + distance]->sign){
                possibilityList->diagonal_left = true;
                possibilityList->length++; 
            }
        }
    }else{
        if(x - distance >= 0 && y + distance < height && table[x - distance][y + distance] != NULL){ //DIAGONAL_LEFT HAUT
            if(pion->sign != table[x - distance][y + distance]->sign){
                possibilityList->diagonal_left = true;
                possibilityList->length++;
            }
        }
    }
    
    //--------------\\

    if(pion->id >= width){
        if(x - distance < width && y + distance >= 0 && table[x - distance][y + distance] != NULL){ //DIAGONAL_RIGHT BAS
            if(pion->sign != table[x - distance][y + distance]->sign){
                possibilityList->diagonal_right = true;
                possibilityList->length++; 
            }
        }
    }else{
        if(x + distance < width && y + distance < height && table[x + distance][y + distance] != NULL){ //DIAGONAL_RIGHT HAUT
            if(pion->sign != table[x + distance][y + distance]->sign){
                possibilityList->diagonal_right = true;
                possibilityList->length++;
            }
        }
    }

    return possibilityList;
}

int getTeamPossibility(int min, int max){
    int possibility = 0;
    for(int i = min; i < max; i++){
        if(getPion(i) != NULL){
            possibility += getPossibility(getPion(i))->length;
        }
    }

    return possibility;
}

int verification(){
    for(int i = 0; i < width * 2; i++){
        if(i >= width){
            if(getPion(i) != NULL && getPion(i)->position.y == 0){ return 2; }
        }else{
            if(getPion(i) != NULL && getPion(i)->position.y == height - 1){ return 1; }
        }
    }

    int team1 = getTeamPossibility(0, width);
    int team2 = getTeamPossibility(width, width * 2);

    if(team1 == 0 && team2 == 0){ return 3; }
    else{
        if(team1 == 0 && team2 != 0){ return 2; } //return 2;
        if(team1 != 0 && team2 == 0){ return 1; } //return 1;
    }

    return 0;
}

void destroy(){
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            delete table[x][y];
        }
        delete[] table[x];
    }
    delete [] table;

    if(recentStrike != NULL){ delete recentStrike; }
    if(lastStrike != NULL){ delete lastStrike; }
}
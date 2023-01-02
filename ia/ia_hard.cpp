#include <iostream>
#include <string.h>
#include "../game/file.cpp"

bool newGrille = false;
int team = 0;
char** datas;
int temp = 0;
pion* bestPion = NULL;
int bestDirection = -2;
int possibilityRound = 0;

//format pion/x!y:mov,pion/x!y:mov

void initIaHard(int teamId){
    team = teamId;
    if(!existFile(database, createName(width, height))){
        newGrille = true;
        createFile(database, createName(width, height));
    }
    
    datas = new char*[width * height];
    for(int i = 0; i < width * height; i++){ datas[i] = NULL; }
    possibilityRound = 0;
}

void destroyIaHard(){
    int win = 0;
    if(verification() == team){ win = 1;
    }else if(verification() == 3){ win = 0;
    }else{ win = -1; }

    //cout<<"win "<<win<<endl;

    int length = width * height;

    for(int i = 0; i < length; i++){
        if(datas[i] != NULL){
            //cout<<"datas "<<i<<" "<<datas[i]<<endl;
            if(win == 1){
                datas[i][strlen(datas[i]) - 2] = '1';
                datas[i][strlen(datas[i]) - 1] = 0;
            }else if(win == -1){
                datas[i][strlen(datas[i]) - 2] = '-';
                datas[i][strlen(datas[i]) - 1] = '1';
            }else if(win == 0){
                datas[i][strlen(datas[i]) - 2] = '0';
                datas[i][strlen(datas[i]) - 1] = 0;
            }
        }
    }
    
    writeFile(database, createName(width, height), datas, length, false);

    //trier doublon

    int ligneLength = 0;
    string* lignes = readFile(database, createName(width, height), ligneLength);
    datas = new char*[ligneLength];

    //cout<<"passed tg"<<endl;

    for(int i = 0; i < ligneLength; i++){
        string ligne = lignes[i];
        int characterLength = ligne.length();

        datas[i] = new char[characterLength + 1];

        for(int j = 0; j < characterLength; j++){
            datas[i][j] = lignes[i].at(j);
        }

        datas[i][characterLength] = 0;
    }

    char** occurency = new char*[ligneLength];
    clearOccurency(datas, occurency, ligneLength, '*');

    //Poid

    for(int i = 0; i < ligneLength; i++){
        if(datas[i] != NULL){
            string s1 = datas[i];
            for(int j = 0; j < ligneLength; j++){
                if(occurency[j] != NULL){
                    string s2 = occurency[j];

                    string previousLine = splitLigne(s2, '*', false);
                    string newLine = splitLigne(s1, '*', false);

                    if(previousLine == newLine){
                        string previousWeigth = splitLigne(s2, '*', true);
                        string newWeigth = splitLigne(s1, '*', true);

                        int result = stoi(newWeigth) + stoi(previousWeigth);
                        if(result > 999){ result = 999; }
                        if(result < -999){ result = -999; }

                        string resultString = to_string(result);
                        int index = s1.find('*') + 1;

                        for(int k = index; k < s2.length(); k++){
                            datas[i][k] = resultString[k - index]; 
                        }
                        datas[i][s2.length() + 1] = 0;

                        //cout<<"Change "<<result<<endl;
                    }
                }
            }
        }
    }

    //cout<<"passed aie"<<endl;

    writeFile(database, createName(width, height), datas, ligneLength, true);

    for(int i = 0; i < ligneLength; i++){
        delete [] datas[i];
    }
    delete [] datas;

    newGrille = false;
    temp = 0;
}

bool getPositionMove(pion* pion, int newX, int newY, int direction){
    if(pion != NULL){
        int x = pion->position.x;
        int y = pion->position.y;
        int distance = 0;

        if(pion->id >= width){ distance = -1; }else { distance = 1; }

        if(direction == DIAGONAL_LEFT){
            if(pion->id >= width){
                if(x + distance >= 0 && y + distance >= 0 && table[x + distance][y + distance] != NULL){ //DIAGONAL_LEFT BAS
                    if(newX == x + distance && newY == y + distance){ return true; }
                }
            }else{
                if(x - distance >= 0 && y + distance < height && table[x - distance][y + distance] != NULL){ //DIAGONAL_LEFT HAUT
                    if(newX == x - distance && newY == y + distance){ return true; }
                }
            }
        }
        
        //--------------\\

        if(direction == DIAGONAL_RIGHT){
            if(pion->id >= width){
                if(x - distance < width && y + distance >= 0 && table[x - distance][y + distance] != NULL){ //DIAGONAL_RIGHT BAS
                    if(newX == x - distance && newY == y + distance){ return true; }
                }
            }else{
                if(x + distance < width && y + distance < height && table[x + distance][y + distance] != NULL){ //DIAGONAL_RIGHT HAUT
                    if(newX == x + distance && newY == y + distance){ return true; }
                }
            }
        }

        //--------------\\

        if(direction == FORWARD){
            if(y + distance < width && y + distance >= 0 && table[x][y + distance] == NULL){ //FORWARD
                if(newY == pion->position.y + distance){ return true; }
            }
        }
    }

    return false;
}

void addHardPossibility(){
    if(possibilityRound >= 1){
        if(getRecentStrike() != NULL && !getRecentStrike()->data.empty() && getLastStrike() != NULL && !getLastStrike()->data.empty()){
            string pionLastInformation = splitLigne(getLastStrike()->data, ':', false);
            string pionLastId = splitLigne(pionLastInformation, '/', false);

            string pionRecentInformation = splitLigne(getRecentStrike()->data, ':', false);
            string pionRecentId = splitLigne(pionRecentInformation, '/', false);

            if(!pionLastId.empty() && stoi(pionLastId) < width && !pionRecentId.empty() && stoi(pionRecentId) >= width){

                string line = getLastStrike()->data + "," + getRecentStrike()->data;
                
                //cout<<"a "<<line<<endl;

                int lsLen = line.length() + 3;
                datas[temp] = new char[lsLen + 1];

                for(int i = 0; i < lsLen - 3; i++){
                    datas[temp][i] = line.at(i);
                }
                datas[temp][lsLen - 3] = '*';
                datas[temp][lsLen - 2] = '0';
                datas[temp][lsLen - 1] = '0';
                datas[temp][lsLen] = 0;

                temp++;
                possibilityRound = 0;
            }
        }
    }else{
        possibilityRound++;
    }
}

pion* getHardPion(int round){
    //cout<<"chaud "<<temp<<endl;

    // if(round == 0){
    //     int length = 0;
    //     string* readDatas = getLineFile(database, createName(width, height), "start", length);

    //     for(int i = 0; i < length; i++){
    //         string result = plitLigne(readDatas[i], ',', true);

    //         if(result != ""){
    //             string pionInformation = plitLigne(result, ':', false);
    //             string pionId = plitLigne(pionInformation, '/', false);

    //             pion* iaPion = getPion(stoi(pionId));

    //             if(iaPion != NULL){
    //                 return iaPion;
    //             }else{
    //                 return getRandomPion();
    //             }
    //         }
    //     }   
    // }

    //return getRandomPion(round); 

    int length = 0;
    string* readDatas = NULL;

    if(getRecentStrike() != NULL && !getRecentStrike()->data.empty()){
        readDatas = getLineFile(database, createName(width, height), getRecentStrike()->data, length);
    }

    if(readDatas != NULL){

        bestPion = NULL;
        bestDirection = -2;
        
        for(int i = 0; i < length; i++){
            int bestPionLigne = 0;

            string result = splitLigne(readDatas[i], ',', true);

            if(!result.empty()){
                string pionInformation = splitLigne(result, ':', false);
                string pionInformationOther = splitLigne(result, ':', true);

                string pionMouvement = splitLigne(pionInformationOther, '*', false);
                string pionMouvementWeigth = splitLigne(pionInformationOther, '*', true);

                string pionId = splitLigne(pionInformation, '/', false);
                string pionPosition = splitLigne(pionInformation, '/', true);

                string pionPositionX = splitLigne(pionPosition, '!', false);
                string pionPositionY = splitLigne(pionPosition, '!', true);

                //strike* recentStrike = getRecentStrike();

                if(!pionId.empty()){

                    pion* iaPion = getPion(stoi(pionId));

                    if(iaPion != NULL && !pionPositionX.empty() && !pionPositionY.empty()){
                        
                        //cout<<"debug 1 "<<result<<" "<<iaPion->position.x<<" "<<iaPion->position.y<<endl;
                        
                        if(getPositionMove(iaPion, stoi(pionPositionX), stoi(pionPositionY), stoi(pionMouvement))){
                            if(getPossibility(iaPion)->length > 0){

                                //cout<<"debug "<<result<<endl;
                                if(stoi(pionMouvementWeigth) > stoi(splitLigne(readDatas[bestPionLigne], '*', true))){
                                    bestPion = iaPion;
                                    bestDirection = stoi(pionMouvement);
                                    bestPionLigne = i;

                                    //return iaPion;

                                }
                                //return iaPion;
                            }
                        }

                        // if(iaPion->position.x == stoi(pionPositionX) && iaPion->position.y == stoi(pionPositionY)){
                            
                        //     cout<<"debug 2 "<<result<<endl;

                        //     if(getPossibility(iaPion)->length > 0){

                        //         cout<<"debug "<<result<<endl;
                        //         if(bestPion != NULL && stoi(pionMouvementWeigth) > stoi(splitLigne(readDatas[bestPionLigne], '*', true))){
                        //             bestPion = iaPion;
                        //             bestDirection = stoi(pionMouvement);
                        //             bestPionLigne = i;

                        //             //return iaPion;

                        //         }else if(bestPion == NULL && stoi(pionMouvementWeigth) >= 0){
                        //             bestPion = iaPion;
                        //             bestPionLigne = i;
                        //         }
                        //         //return iaPion;
                        //     }
                        // }
                    }
                }
            }
        }
        //cout<<"best "<<bestPion<<endl;
        if(bestPion != NULL){
            cout<<"best is returned "<<bestPion->id<<endl;
            return bestPion;
        }
    }

    cout<<"random "<<endl;
    return getRandomPion(round); 
}

direction getHardMove(pion* pion){
    if(bestDirection != -2){
        if(bestDirection == 0){ return FORWARD; }
        if(bestDirection == -1){ return DIAGONAL_LEFT; }
        if(bestDirection == 1){ return DIAGONAL_RIGHT; }
    }else{
        return getRandomMove(pion);
    }
    //return getRandomMove(pion);
}
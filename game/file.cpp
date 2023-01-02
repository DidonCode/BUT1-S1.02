#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

const string database = "game/database/";

string createName(int width, int height){
    return to_string(width) + "x" + to_string(height) + ".data";
}

bool existFile(string path, string filename){
    ifstream file;
    file.open(path + filename, ios_base::binary);

    return file.is_open();
}

int getLineNumber(string path, string filename){
    fstream file;
    file.open(path + filename);

    string line;
    int number = 0;

    while(getline(file, line)){
        number++;
    }

    file.close();

    return number;
}

void createFile(string path, string filename){
    ofstream file(path + filename);
    file.close();
}

void clearFile(string path, string filename){
    std::ofstream ofs;
    ofs.open(path + filename, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}

void writeFile(string path, string filename, char** datas, int length, bool overwrite){
    ofstream file;

    if(!existFile(path, filename)) return;

    if(overwrite) { file.open(path + filename);
    }else{ file.open(path + filename, ios::app); }

    for(int i = 0; i < length; i++){
        if(datas[i] != NULL){
            file<<datas[i]<<endl;
        }
    }

    file.close();
}

string* readFile(string path, string filename, int& length){
    ifstream file;

    if(!existFile(path, filename)) return 0;

    length = getLineNumber(path, filename);
    string* data = new string[length];
    
    file.open(path + filename);

    string line;
    int n = 0;

    while(getline(file, line)){
        data[n] = line;
        n++;
    }

    return data;
}

string* getLineFile(string path, string filename, string contain, int &ligne){
    if(!existFile(path, filename)) return NULL;

    int length = 0;
    string* content = readFile(path, filename, length);

    ligne = 0;
    for(int i = 0; i < length; i++){
        if(content[i].find(contain) != string::npos){
            ligne++;
        }
    }

    if(ligne == 0) return NULL;
    string* lines = new string[ligne];

    ligne = 0;
    for(int i = 0; i < length; i++){
        if(content[i].find(contain) != string::npos){
            lines[ligne] = content[i];
            //cout<<"debug "<<lines[ligne]<<endl;
            ligne++;
        }
    }
    delete [] content;

    return lines;
}

string splitLigne(string line, char delimiter, bool side){
    string split = "";

    if(!side){
        split = line.substr(0, line.find(delimiter)); 
    }else{
        if(line.find(delimiter) != string::npos){
            split = line.substr(line.find(delimiter) + 1, line.size() - 1);
        }
    }

    return split;
}

void clearOccurency(char** &list, char** &occurency, int length, char delimiter){
    for(int i = 0; i < length; i++){
        if(list[i] != NULL){
            int corespond = 0;

            string s1 = "";
            if(delimiter != ' '){ s1 = splitLigne(list[i], delimiter, false); }
            else{ s1 = list[i]; }

            for(int j = 0; j < length; j++){
                if(list[j] != NULL){

                    string s2 = "";
                    if(delimiter != ' '){ s2 = splitLigne(list[j], delimiter, false); }
                    else{ s2 = list[j]; }

                    if(i != j && s1 == s2){ corespond++; }
                }
            }
            if(corespond > 0){
                occurency[i] = list[i];
                list[i] = NULL;
            }else{
                occurency[i] = NULL;
                //cout<<"occurence oui"<<endl;
            }
        }
    }
}
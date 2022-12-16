#include <iostream>
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

    string temp = "";
    int number = 0;

    while(temp != "end"){
        file>>temp;
        cout<<temp<<endl;

        if(temp == "end"){
            return number;
        }

        number++; 
    }

    file.close();
}

void createFile(string path, string filename){
    ofstream file(path + filename);
    file.close();
}

void writeFile(string path, string filename, const char** datas){
    fstream file;

    if(!existFile(path, filename)) return;

    file.open(path + filename);
    for(int i = 0; i < sizeof(datas) - 1; i++){
        file<<datas[i];
        file<<"\n";
    }

    file<<"end";

    file.close();
}

string* readFile(string path, string filename){
    fstream file;

    if(!existFile(path, filename)) return 0;

    string* data = new string[getLineNumber(path, filename)];
    
    file.open(path + filename);

    string temp = "";
    int n = 0;
    bool good = false;

    while(!good){
        file>>temp;

        if(temp != "end"){
            data[n] = temp;
            n++; 
        }else{
            good = true;
        }
    }

    file.close();

    return data;
}
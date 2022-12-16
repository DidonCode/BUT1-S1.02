#include <windows.h>

bool keyPress(int keyCode){
    if(GetAsyncKeyState(keyCode) & 0x8000){
        Sleep(100);
        return true;
    }
    return false;
}

void setCursor(int x, int y){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;

    coord.X = x * 3 + 1;
    coord.Y = y;

    if(coord.X < 0){
        coord.X = 0;
    }

    SetConsoleCursorPosition(hConsole, coord);
}


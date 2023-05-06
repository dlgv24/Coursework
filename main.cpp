#include <windows.h>
#include "menu.h"

int main() {
    UINT oldin = GetConsoleCP(), oldout = GetConsoleOutputCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");
    Menu* menu = new Menu();
    menu->menu();
    delete menu;
    SetConsoleCP(oldin);
    SetConsoleOutputCP(oldout);
    return 0;
}
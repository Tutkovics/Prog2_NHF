#include <iostream>
#include <cstring>
#include "Documentary.h"
#include "Container.h"
#include "Menu.h"

int main() {
    Container con;
    con.loadToMemory();

    ////// MENU ///////
    Menu men;
    try {
        men.menuShow(con);
    } catch ( const char* msg){
        std::cerr << msg << std::endl;
    }

    con.clean();

    return 0;
}
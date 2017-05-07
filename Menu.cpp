#include "Menu.h"

int Menu::menuDisplay() {
    std::cout << "***************************" << std::endl;
    std::cout << "1 - Mindent listaz" << std::endl;
    std::cout << "2 - Kereses" << std::endl;
    std::cout << "3 - Szerkesztes" << std::endl;
    std::cout << "4 - Uj film letrehozasa" << std::endl;
    std::cout << "5 - Torles" << std::endl;
    std::cout << "6 - Kilépés" << std::endl;
    std::cout << "***************************" << std::endl;

    int c;
    std::cin >> c;
    return c;
}

void Menu::menuShow( Container& con) {
    while( true ) {
        int c = Menu::menuDisplay();
        int id = 0;

        switch (c) {
            case 1:
                con.printAll();
                break;
            case 2:
                con.search();
                break;
            case 3:
                std::cout << "Szerkesztendo film ID-ja: " << std::endl;
                std::cin >> id;
                con.editMovie(id);
                con.clean();
                con.loadToMemory();
                break;
            case 4:
                con.push_back( Container::getNewMovie() );
                con.addToFile( );
                break;
            case 5:
                std::cout << "Torlendo film ID-ja: " << std::endl;
                std::cin >> id;
                con.deleteMovie(id);
                con.clean();
                con.loadToMemory();
                break;
            case 6:
                con.clean();
                exit(0);
            default:
                throw "Hibas user input";
        }
    }
}

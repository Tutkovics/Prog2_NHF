#include "Menu.h"

int Menu::menuDisplay() {
    std::cout << "***************************" << std::endl;
    std::cout << "1 - Mindent listaz" << std::endl;
    std::cout << "2 - Kereses" << std::endl;
    std::cout << "3 - Szerkesztes" << std::endl;
    std::cout << "4 - Torles" << std::endl;
    std::cout << "5 - Kilépés" << std::endl;
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
                std::cout << "Not now" << std::endl;
                break;
            case 3:
                std::cout << "Szerkesztendo film ID-ja: " << std::endl;
                std::cin >> id;
                con.editMovie(id);
                break;
            case 4:
                std::cout << "Torlendo film ID-ja: " << std::endl;
                std::cin >> id;
                con.deleteMovie(id);
                break;
            case 5:
                exit(0);
            default:
                std::cout << "Hibas input" << std::endl;
        }
    }
}

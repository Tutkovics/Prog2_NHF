#include "Movie.h"


void Movie::printDatas() {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Cím: " << title << std::endl;
    std::cout << "Megjelenés éve: " << release << std::endl;
    std::cout << "Rendező: " << director << std::endl;
    std::cout << "Lejátszási idő: " << playingTime << std::endl;
    std::cout << "Értékelés: " << rating << std::endl;

}

void Movie::sep() {
    std::cout << "\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - \n" << std::endl;
}



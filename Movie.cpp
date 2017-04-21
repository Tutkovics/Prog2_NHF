#include "Movie.h"


void Movie::printDatas() {
    std::cout << "Cím: " << title << std::endl;
    std::cout << "Kiadási év: " << release << std::endl;
    std::cout << "Rendező: " << director << std::endl;
    std::cout << "Lejátszási idő: " << playingTime << std::endl;
    std::cout << "Értékelés: " << rating << std::endl;
}

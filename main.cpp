#include <iostream>
#include "Movie.h"
#include "Documentary.h"

int main() {
    Documentary elso("Hullám","Stephen Hawking",120,1997,5.1, documentary, "Nagyon jó kis dokumnetum film");
    /*std::cout << "Rendező: " << elso.getDirector() << std::endl;
    std::cout << "Cím: " << elso.getTitle() << std::endl;
    std::cout << "Értékelés: " << elso.getRating() << std::endl;
    std::cout << "Kiadás: " << elso.getRelease() << std::endl;
    std::cout << "Lejátszási idő: " << elso.getPlayingTime() << std::endl;
    std::cout << "Műfaj: " << elso.getType() << std::endl;
    std::cout << "Leírás: " << elso.getDescription() << std::endl;*/
    elso.printDatas();
    return 0;
}
#include "Family.h"

void Family::printDatas() {
    Movie::printDatas();
    std::cout << "Korhatár: " << ageLimit << std::endl;
}

void Family::printToFile(std::fstream &os) {
    Movie::printToFile(os);
    os << ageLimit ;
}

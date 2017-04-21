#include "Family.h"

void Family::printDatas() {
    Movie::printDatas();
    std::cout << "Korhatár: " << ageLimit << std::endl;
}

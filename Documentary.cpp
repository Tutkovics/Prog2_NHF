#include "Documentary.h"

void Documentary::printDatas() {
    Movie::printDatas();
    std::cout << "Leírás: " << description << std::endl;
}

void Documentary::printToFile(std::fstream &os) {
    Movie::printToFile(os);
    os << description ;
}

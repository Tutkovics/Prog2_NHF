#include "Documentary.h"

void Documentary::printDatas() {
    Movie::printDatas();
    std::cout << "Leírás: " << description << std::endl;
}

#include <iostream>
#include "Movie.h"
#include "Documentary.h"
#include "Family.h"

int main() {
    // test cases
    Documentary doc("Dokumentumfilm","Hawking",120,1999,9.1, documentary, "Nagyon jó kis dokumnetum film");
    Family csalad("Másik család","Fasz Tudja",120,2015,4.1, family, 15);
    Movie altalanos("Hullám","Stephen Hawking",120,1997,5.1, def);

    //print data
    doc.printDatas();
    Movie::sep();
    csalad.printDatas();
    Movie::sep();
    altalanos.printDatas();

    return 0;
}
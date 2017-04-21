#include <iostream>
#include "Movie.h"
#include "Documentary.h"
#include "Family.h"
#include "Container.h"

int main() {
    // test cases
    Container con;
    con.push_back( new Movie("Hullám","Stephen Hawking",120,1997,5.1, def));
    con.push_back( new Family("Másik család","Fasz Tudja",120,2015,4.1, family, 15));
    con.push_back( new Documentary("Dokumentumfilm","Hawking",120,1999,9.1, documentary, "Nagyon jó kis dokumnetum film"));
    con.printAll();
    /*Documentary doc("Dokumentumfilm","Hawking",120,1999,9.1, documentary, "Nagyon jó kis dokumnetum film");
    Family csalad("Másik család","Fasz Tudja",120,2015,4.1, family, 15);
    Movie altalanos("Hullám","Stephen Hawking",120,1997,5.1, def);*/

    //print data
    /*doc.printDatas();
    Movie::sep();
    csalad.printDatas();
    Movie::sep();
    altalanos.printDatas();
    Movie::sep();

    Container::getNewMovie();

    Container::printAll();*/

    return 0;
}
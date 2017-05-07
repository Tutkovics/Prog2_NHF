#include <iostream>
#include "Movie.h"
#include "Documentary.h"
#include "Family.h"
#include "Container.h"
#include "Menu.h"

int main() {
    Container con;
    con.loadToMemory();


    ////// MENU ///////
    Menu men;
    men.menuShow(con);

    // test cases

    /*// General Movie class
    // ID not set
    Movie p =  Movie("Hullám","Stephen Hawking",120,1997,5.1, def);
    if( p.getTitle() == "Hullám")
        std::cout << "Success get title" << std::endl;
    else
        std::cout << "Fail title test" << std::endl;

    //Inherited class
    Family f = Family("Családi film","Rick Adam",120,2015,4.1, family, 1, 15);
    if( f.getAgeLimit() == 15)
        std::cout << "Success get agelimit from family movie" << std::endl;
    else
        std::cout << "Fail agelimit test" << std::endl;

    // Continer's push_back function
    Container con;
    con.push_back( new Documentary("Dokumentumfilm","Somebody",120,1999,9.1, documentary, 0,"Nagyon jó kis dokumnetum film"));
    if( con.getPcs() != 0 )
        std::cout << "Success push_back function" << std::endl;
    else
        std::cout << "Fail push_back function" << std::endl;
    */

    /*Container con;
    con.loadToMemory();
    con.printAll();

    std::cout << "EDIT id = 2" << std::endl;

    con.editMovie(4);*/
    //con.loadToMemory();
    //con.printAll();

    // Container test for JPorta
    /*Container con;
    con.push_back( new Movie("Hullám","Stephen Hawking",120,1997,5.1, def));
    con.push_back( new Family("Másik család","Fasz Tudja",120,2015,4.1, family, 0, 15));
    con.push_back( new Documentary("Dokumentumfilm","Hawking",120,1999,9.1, documentary, 0,"Nagyon jó kis dokumnetum film"));
    //con.push_back( Container::getNewMovie() ); // get new movie from user
    con.printAll();*/

    /* Movie objects test
    Documentary doc("Dokumentumfilm","Hawking",120,1999,9.1, documentary, "Nagyon jó kis dokumnetum film");
    Family csalad("Másik család","Fasz Tudja",120,2015,4.1, family, 15);
    Movie altalanos("Hullám","Stephen Hawking",120,1997,5.1, def);*/


    /* Data print test
    doc.printDatas();
    Movie::sep();
    csalad.printDatas();
    Movie::sep();
    altalanos.printDatas();
    Movie::sep();

    Container::getNewMovie();

    Container::printAll();*/

    return 0;
}
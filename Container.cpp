#include "Container.h"
#include "Family.h"
#include "Documentary.h"
#include <iostream>
#include <fstream>


Movie* Container::getNewMovie() {
    // declare required variables
    std::string newTitle, newDirector;
    int newRelease, newPlayingTime;
    char a; // help with movieType switch
    double newRating;
    movieType newType;
    Movie* pMovie;

    // get variables
    // need exception handling --> now break the function if you give wrong format
    std::cout << "ÚJ FILM" << std::endl;

    std::cout << "Cím: ";
    getline( std::cin, newTitle);

    std::cout << "Rendező: ";
    getline( std::cin, newDirector);

    std::cout << "Megjelenés éve: ";
    std::cin >> newRelease;

    std::cout << "Lejátszási idő (perc): ";
    std::cin >> newPlayingTime;

    std::cout << "Értékelés: ";
    std::cin >> newRating;

    std::cout << "Típus: \n"
            << "\t1 - Általános\n"
            << "\t2 - Családi\n"
            << "\t3 - Dokumentum\n";
    std::cin >> a;

    // depends on movietype what we need
    // then create new object and print all data
    if( a == '2') { // if FAMILY movie
        newType = family;
        int newAgeLimit;
        std::cout << "Korhatár: ";
        std::cin >> newAgeLimit;
        pMovie = new Family(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, 0,newAgeLimit);
    } else if( a == '3') { // if DOCUMENTARY movie
        newType = documentary;
        std::string newDescription;
        std::cin.ignore(1,'\n'); // ignore endline character
        std::cout << "Leírás: ";
        getline(std::cin, newDescription);
        pMovie = new Documentary(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, 0,newDescription);
    } else { // in other cases
        newType = def;
        pMovie = new Movie(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType);
    }

    Movie::sep();

    return  pMovie;
}

void Container::push_back(Movie * moviePointer) {
    // it could be better if wouldn't resize after every new element
    /*if( pcs == 0){// this is the firs pointer
        array[0] = moviePointer;
        pcs++;
    } else { // was not empty
        pcs++;
        Movie* newArray = new  Movie[pcs];
        // exception handling need
        for (int i = 0; i < pcs; ++i) {
            newArray[i] = array[i];
        }
        newArray[pcs] = *moviePointer;
        delete [] array;
        *array = newArray;
    }*/
    array[pcs] = moviePointer;
    pcs++;
}

void Container::printAll() {
    for (int i = 0; i < pcs; ++i) {
        array[i]->printDatas();
        Movie::sep();
    }
}

void Container::loadToMemory() {
    std::fstream dataFile;
    dataFile.open ("data.txt");
    if( !dataFile.is_open() ){ // unsuccessful file opening
        // exception handling
        std::cerr << "SIKERTELEN FÁJLNYITÁS! (data.txt)" << std::endl;

    } else { // successfully opened
        std::string line;
        while ( dataFile.good() ) {
            //read line by line
            getline (dataFile, line);

            std::string delimiter = ";";

            size_t pos = 0;
            std::string token;

            movieType ty = def;
            int i = 0, re, playing, id;
            double ra;
            std::string tit, dir;

            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                //std::cout << token << std::endl;
                if( i == 0 ){ //id
                    id = std::stoi(token);
                } else if( i == 1 ){ //movieType
                    int k = std::stoi(token);
                    if( k == 2 ) ty = family;
                    else if ( k == 3) ty = documentary;
                } else if( i == 2 ){ //title
                    tit = token;
                } else if( i == 3 ){ //director
                    dir = token;
                } else if( i == 4 ){ //release
                    re = std::stoi(token);
                } else if( i == 5 ){ //playingTime
                    playing = std::stoi(token);
                } else if( i == 6 ){ //rating
                    ra = std::stod(token);
                } else {
                    std::cerr << "VALAMI NEM STIMMEL A SOR BEOLVASÁSÁNÁL" << std::endl;
                }
                line.erase(0, pos + delimiter.length());
                i++;
            }
            // line still cuold contain description or ageLimit
            if( ty == family ){
                int ageLim = std::stoi(line);
                this->push_back( new Family(tit, dir, playing, re, ra, ty, id, ageLim) );
            } else if( ty == documentary ){
                std::string des = line;
                this->push_back( new Documentary(tit, dir, playing, re, ra, ty, id, des) );
            } else{ //general movie
                this->push_back( new Movie(tit, dir, playing, re, ra, ty, id) );
            }


        }
        dataFile.close();
    }
}

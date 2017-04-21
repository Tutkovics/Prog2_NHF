#include "Container.h"
#include "Family.h"
#include "Documentary.h"

void Container::getNewMovie() {
    // declare required variables
    std::string newTitle, newDirector;
    int newRelease, newPlayingTime;
    char a; // help with movieType switch
    double newRating;
    movieType newType;

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
        Family csaladi(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, newAgeLimit);
        csaladi.printDatas();
    } else if( a == '3') { // if DOCUMENTARY movie
        newType = documentary;
        std::string newDescription;
        std::cin.ignore(1,'\n'); // ignore endline character
        std::cout << "Leírás: ";
        getline(std::cin, newDescription);
        Documentary doc(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, newDescription);
        doc.printDatas();
    } else { // in other cases
        newType = def;
        Movie bekert(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType);
        bekert.printDatas();
    }

    Movie::sep();
}

void Container::push_back(Movie * moviePointer) {
    // it could be better if wouldn't resize after every new element
    if( pcs == 0){// this is the firs pointer
        array = moviePointer;
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
        array = newArray;
    }
}

void Container::printAll() {
    for (int i = 0; i < pcs; ++i) {
        array[i]->printDatas();
    }
}

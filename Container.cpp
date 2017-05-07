#include "Container.h"
#include "Family.h"
#include "Documentary.h"

int Container::aktId = 0;

Movie* Container::getNewMovie() {
    // declare required variables
    std::string newTitle, newDirector;
    int newRelease, newPlayingTime;

    Container::aktId++;
    int id = Container::aktId;

    char a; // help with movieType switch
    double newRating;
    movieType newType;
    Movie* pMovie;

    // get variables
    // need exception handling --> now break the function if you give wrong format
    std::cin.ignore(1,'\n');
    std::cout << "UJ FILM" << std::endl;

    std::cout << "Cim: ";
    getline( std::cin, newTitle);

    std::cout << "Rendezo: ";
    getline( std::cin, newDirector);

    std::cout << "Megjelenes eve: ";
    std::cin >> newRelease;

    std::cout << "Lejatszasi ido (perc): ";
    std::cin >> newPlayingTime;

    std::cout << "Ertekeles: ";
    std::cin >> newRating;

    std::cout << "Tipus: \n"
            << "\t1 - Altalanos\n"
            << "\t2 - Csaladi\n"
            << "\t3 - Dokumentum\n";
    std::cin >> a;

    // depends on movietype what we need
    // then create new object and print all data
    if( a == '2') { // if FAMILY movie
        newType = family;
        int newAgeLimit;
        std::cout << "Korhatar: ";
        std::cin >> newAgeLimit;

        pMovie = new Family(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, id,newAgeLimit);
    } else if( a == '3') { // if DOCUMENTARY movie
        newType = documentary;
        std::string newDescription;

        std::cin.ignore(2,'\n'); // ignore endline character
        std::cout << "Leiras: ";
        getline(std::cin, newDescription);
        pMovie = new Documentary(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, id,newDescription);
    } else { // in other cases
        newType = def;
        pMovie = new Movie(newTitle, newDirector, newPlayingTime, newRelease, newRating, newType, id);
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
    if( pcs == 0){
        std::cout << "Nincs megjelenitendo adat" << std::endl;
        return;
    }

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
        throw "SIKERTELEN FÁJLNYITÁS! (loadToMemory)" ;

    } else { // successfully opened
        std::string line;
        while ( dataFile.good() ) {
            //read line by line
            getline (dataFile, line);

            std::string delimiter = ";";

            size_t pos = 0;
            std::string token = "";

            movieType ty = def;
            int i = 0, re = 0, playing = 0, id = 0;
            double ra = 0.0;
            std::string tit = "", dir = "";

            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                //std::cout << token << std::endl;
                if( i == 0 ){ //id
                    id = std::stoi(token);
                } else if( i == 1 ){ //movieType
                    int k = std::stoi(token);
                    if( k == 2 ) ty = family;
                    else if ( k == 3) ty = documentary;
                    else if ( k== 1) ty = def;
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
                    throw "VALAMI NEM STIMMEL A SOR BEOLVASÁSÁNÁL";
                }
                line.erase(0, pos + delimiter.length());
                i++;

            }
            // line still cuold contain description or ageLimit
            if( id != 0) {
                if (ty == family) {
                    int ageLim = std::stoi(line);
                    this->push_back(new Family(tit, dir, playing, re, ra, ty, id, ageLim));
                } else if (ty == documentary) {
                    std::string des = line;
                    this->push_back(new Documentary(tit, dir, playing, re, ra, ty, id, des));
                } else if (ty == def) { //general movie
                    this->push_back(new Movie(tit, dir, playing, re, ra, ty, id));
                }
                Container::aktId = id;
            }
        }

        dataFile.close();
    }
}

void Container::deleteMovie(int id) {
    std::fstream dataFile;

    dataFile.open ("data.txt", std::ofstream::out | std::ofstream::trunc);

    if( !dataFile.is_open() ){ // unsuccessful file opening
        // exception handling
        throw "SIKERTELEN FÁJLNYITÁS! (deleteMovie)";

    } else {
        for (int i = 0; i < pcs; i++) {
            if (array[i]->getId() == id) {
                // deleted movie
                std::cout << "Sikeres torles" << std::endl;
            } else {
                if( i != 0) {
                    dataFile << '\n';
                    array[i]->printToFile(dataFile);
                } else {
                    array[i]->printToFile( dataFile );
                }
            }
        }
        dataFile.close();

    }
}

void Container::editMovie(int id) {
    std::fstream dataFile;
    dataFile.open ("data.txt", std::ofstream::out | std::ofstream::trunc);
    if( !dataFile.is_open() ){ // unsuccessful file opening
        // exception handling
        throw "SIKERTELEN FÁJLNYITÁS! (editeMovie)";

    } else {
        for (int i = 0; i < pcs; i++) {
            if (array[i]->getId() == id) {
                std::cout << "Melyik argumentumot akarod megvaltoztatni?" << std::endl;
                int choice;
                std::cout << "\t1 - Cim" << std::endl;
                std::cout << "\t2 - Rendezo" << std::endl;
                std::cout << "\t3 - Megjelenes" << std::endl;
                std::cout << "\t4 - Ertekeles" << std::endl;
                std::cout << "\t5 - Lejatszas ideje" << std::endl;
                std::cin >> choice;
                std::cin.ignore(3,'\n');
                std::string line;
                switch( choice ){
                    case 1:
                        std::cout << "Az uj cim: \t";
                        getline(std::cin, line);
                        array[i]->setTitle( line );
                        break;
                    case 2:
                        std::cout << "Az uj rendezo: \t";
                        getline(std::cin, line);
                        array[i]->setDirector( line );
                        break;
                    case 3:
                        std::cout << "Az uj megjenes: \t";
                        getline(std::cin, line);
                        array[i]->setRelease( std::stoi(line) );
                        break;
                    case 4:
                        std::cout << "Az uj ertekeles: \t";
                        getline(std::cin, line);
                        array[i]->setRating( std::stod(line) );
                        break;
                    case 5:
                        std::cout << "Az uj lejatszasi idő: \t";
                        getline(std::cin, line);
                        array[i]->setPlayingTime( std::stoi(line) );
                        break;
                    default:
                        std::cout << "Helytelen input" << std::endl;
                        if( i != 0) {
                            dataFile << '\n';
                            array[i]->printToFile( dataFile );
                        } else {
                            array[i]->printToFile( dataFile );
                        }
                        break;
                }

                if( i != 0) {
                    dataFile << '\n';
                    array[i]->printToFile( dataFile );
                } else {
                    array[i]->printToFile( dataFile );
                }
            } else {
                if( i != 0) {
                    dataFile << '\n';
                    array[i]->printToFile( dataFile );
                } else {
                    array[i]->printToFile( dataFile );
                }
            }
        }
        dataFile.close();
    }
}

void Container::clean() {
    // delete all pointer from Movie array -> reload datas from file
    for(int i = 0; i < pcs; i++){
        delete array[i];
    }
    pcs = 0;
}

void Container::addToFile() {
    // append the last movie* to data.txt
    std::fstream dataFile;
    dataFile.open ("data.txt", std::ofstream::app);
    if( !dataFile.is_open() ){ // unsuccessful file opening
        // exception handling
        throw "SIKERTELEN FÁJLNYITÁS ÉS MENTÉS! (addToFile)";
    } else {
        dataFile << '\n';
        array[ pcs-1 ]->printToFile( dataFile );

        dataFile.close();
        this->clean();
        this->loadToMemory();
    }
}

void Container::search() {
    int choice;
    std::string str;
    int in;
    double ra;

    std::cout << "Melyik atributumra szeretnél keresni?" << std::endl;
    std::cout << "\t1 - Cim" << std::endl;
    std::cout << "\t2 - Rendezo" << std::endl;
    std::cout << "\t3 - Megjelenes" << std::endl;
    std::cout << "\t4 - Ertekeles" << std::endl;
    std::cout << "\t5 - Lejatszas ideje" << std::endl;
    std::cout << "\t6 - Kategoria" << std::endl;
    std::cin >> choice;
    std::cin.ignore(3,'\n');

    switch ( choice ){
        case 1:
            std::cout << "Cim: " << std::endl;
            std::cin >> str;
            break;
        case 2:
            std::cout << "Rendezo: " << std::endl;
            std::cin >> str;
            break;
        case 3:
            std::cout << "Megjelenes: " << std::endl;
            std::cin >> in;
            break;
        case 4:
            std::cout << "Ertekeles: " << std::endl;
            std::cin >> ra;
            break;
        case 5:
            std::cout << "Lejatszasi ideje: " << std::endl;
            std::cin >> in;
            break;
        case 6:
            std::cout << "Tipus" << std::endl;
            std::cout << "\t1-Altalanos film" << std::endl;
            std::cout << "\t2-Csaladi film" << std::endl;
            std::cout << "\t3-Dokumentum film" << std::endl;
            std::cin >> in;
            break;
        default:
            throw "Hibas user input";
            break;
    }

    for( int i = 0; i < pcs; i++){
        switch ( choice ){
            case 1:
                if (array[i]->getTitle().find(str) != std::string::npos) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            case 2:
                if (array[i]->getDirector().find(str) != std::string::npos) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            case 3:
                if (array[i]->getRelease() == in) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            case 4:
                if (array[i]->getRating() == ra) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            case 5:
                if (array[i]->getPlayingTime() == in) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            case 6:
                if (array[i]->getType() == in) {
                    array[i]->printDatas();
                    array[i]->sep();
                }
                break;
            default:
                break;
        }
    }
}

void Container::tests() {
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
    this->push_back( new Documentary("Dokumentumfilm","Somebody",120,1999,9.1, documentary, 0,"Nagyon jó kis dokumnetum film"));
    if( this->getPcs() != 0 )
        std::cout << "Success push_back function" << std::endl;
    else
        std::cout << "Fail push_back function" << std::endl;

    //Container's clear function
    this->clean();
    if( this->pcs == 0){
        std::cout << "Success clean function" << std::endl;
    } else {
        std::cout << "Fail clean function" << std::endl;
    }
}

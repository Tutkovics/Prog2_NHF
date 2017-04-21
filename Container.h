#ifndef CODE_CONTAINER_H
#define CODE_CONTAINER_H

#include "Movie.h"

class Container {
    Movie* first;
    int pcs;

public:
    // constructor
    Container(Movie* first = NULL, int pcs = 0): first(first), pcs(pcs){}

    int getPcs(){ return pcs; }
    void setPcs( int newPcs ){ pcs = newPcs; }
    void setFirst( Movie* newFirst ){ first = newFirst; }
    Movie* getFirst(){ return first; }

    /*loadToMemory(): void
    addNewMovie(): void
    deleteMovie(): void
    editMovie(): void
    search(): void*/

    //important functions
    void loadToMemory();
    void addNewMovie(string, string, int, int, double, movieType, string, int);
    void deleteMovie();
    void editMovie();
    void search();
    void printAll();

};


#endif //CODE_CONTAINER_H

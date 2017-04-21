#ifndef CODE_CONTAINER_H
#define CODE_CONTAINER_H

#include "Movie.h"

class Container {
    Movie* array;
    int pcs;

public:
    // constructor
    Container(Movie* first = NULL, int pcs = 0): array(first), pcs(pcs){}

    int getPcs(){ return pcs; }
    void setPcs( int newPcs ){ pcs = newPcs; }
    void setFirst( Movie* newFirst ){ array = newFirst; }
    Movie* getFirst(){ return array; }


    //important functions
    void loadToMemory();
    static void getNewMovie();
    void push_back(Movie*);
    void deleteMovie();
    void editMovie();
    void search();

    void printAll();

};


#endif //CODE_CONTAINER_H

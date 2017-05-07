#ifndef CODE_CONTAINER_H
#define CODE_CONTAINER_H

#include "Movie.h"

class Container {
    Movie* array[100];
    int pcs = 0;
    static int aktId;


public:
    // constructor
    Container(){}

    //Container(Movie* first = NULL, int pcs = 0): array(first), pcs(pcs){}

    int getPcs(){ return pcs; }
    void setPcs( int newPcs ){ pcs = newPcs; }
    void setFirst( Movie* newFirst ){ *array = newFirst; }
    Movie* getFirst(){ return array[0]; }


    //important functions
    void loadToMemory();
    static Movie* getNewMovie();
    void push_back(Movie*);
    void deleteMovie(int id);
    void editMovie(int id);
    void search();
    void clean();
    void addToFile();
    void printAll();

};


#endif //CODE_CONTAINER_H

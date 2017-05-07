#ifndef CODE_FAMILY_H
#define CODE_FAMILY_H

#include <iostream>
#include <fstream>
#include <string>
#include "Movie.h"

using std::string;

class Family: public Movie {
    int ageLimit;

public:
    // constuctor
    Family(string t = "No title", string d = "Anonymous", int p = 0, int re = 0, double ra = 0.0, movieType ty = def, int id = 0 ,int a = 0):
        Movie(t, d, p, re, ra, ty, id), ageLimit(a){}

    //set-, get functions
    int getAgeLimit(){ return ageLimit; }
    void setAgeLimit( int a ){ ageLimit = a; }

    void printDatas();
    void printToFile(std::fstream& os);
};


#endif //CODE_FAMILY_H

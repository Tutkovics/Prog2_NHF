#ifndef CODE_DOCUMENTARY_H
#define CODE_DOCUMENTARY_H

#include <iostream>
#include <string>
#include "Movie.h"

using std::string;

class Documentary : public Movie{
    string description;

public:
    //constructor
    Documentary(string t = "No title", string d = "Anonymous", int p = 0, int re = 0, double ra = 0.0, movieType ty = def, string des = ""):
            Movie(t, d, p, re, ra, ty), description(des){}

    //set-, get functions
    string getDescription(){ return description; }
    void setDescription( string des ){ description = des; }
};


#endif //CODE_DOCUMENTARY_H

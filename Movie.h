#ifndef CODE_MOVIE_H
#define CODE_MOVIE_H

#include <iostream>
#include <string>

using std::string;

enum movieType { def, family, documentary };

class Movie {

    // private data
    string title;
    int release;
    int playingTime;
    double rating;
    string director;
    movieType type;

public:

    //constructor
    Movie(string t = "No title", string d = "Anonymous", int p = 0, int re = 0, double ra = 0.0, movieType ty = def):
            title(t), release(re), playingTime(p), rating(ra), director(d), type(ty) {}

    //get functions
    string getTitle(){ return title; }
    int getRelease(){ return release; }
    int getPlayingTime(){ return playingTime; }
    double getRating(){ return rating; }
    string getDirector(){ return director; }
    movieType getType(){ return type; }

    //set functions
    void setTitle( string t ){ title = t; }
    void setRelease( int re ){ release = re; }
    void setPlayingTime( int p ){ playingTime = p; }
    void setRating( double ra ){ rating = ra; }
    void setDirector( string d ){ director = d; }
    void setType( movieType ty ){ type = ty; }

    ~Movie(){}

};


#endif //CODE_MOVIE_H

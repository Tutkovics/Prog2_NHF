#include "Container.h"

void Container::addNewMovie(string t = "No title", string d = "Anonymous", int p = 0, int re = 0, double ra = 0.0, movieType ty = def, string des = "", int a = 0) {
    // if this is the first element have to add
    if( pcs == 0 ){
        pcs++;
        Movie* pointer = new Movie;
        if( ty == def)
            pointer = NULL;
        else if(ty == documentary)
            ;
        else if( ty == family )
            ;
    }
}

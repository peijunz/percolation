#ifndef GRAPH_H
#define GRAPH_H

#include "ndarray.h"
#define BOND true
#define SITE false

class torus{
    bool type;
public:
    ndarray <char> site;
    ndarray <char> *bond;
    torus(int dim, int width, double prob, int tp);
    void print();
    void wrapping();
};

#endif // GRAPH_H

#include <stdio.h>
#include "lib/16807.h"
#include "lib/ndarray.h"
#include "lib/deque.h"
#include "lib/graph.h"
//1458816173 Site Percolation有问题
#define DEBUG
int main(){
    int seed, dim, width;
    double prob;
#ifndef DEBUG
    printf("Input seed, dimension, width, prob: ");
    scanf("%d%d%d%lf",&seed, &dim, &width,&prob);
    printf("%d %d %d %f\n",seed, dim, width, prob);
#else
    seed=1458822974;
    dim=8;
    width=8;
    prob=0.068;
#endif
    myrand(seed);
    torus t(dim,width,prob,BOND);
    t.wrapping();
    return 0;
}

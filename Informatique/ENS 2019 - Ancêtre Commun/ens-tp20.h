#ifndef _ENS_TP20
#define _ENS_TP20

#include <stdlib.h>

struct arbre {
    struct arbre *g;
    struct arbre *d;
    long val;
};

struct liste {
    long val;
    struct liste *suiv;
};

typedef struct arbre arbre;
typedef struct liste liste;
#endif
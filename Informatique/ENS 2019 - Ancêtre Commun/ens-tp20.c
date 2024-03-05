#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ens-tp20.h"

#define M 2147483647
#define u0 42

long max(long a, long b) {
    if (a >= b) return a;
    return b;
}

long *get_ubars(long n) {
    long *lookup = malloc(sizeof(long) * n);
    lookup[0] = u0;
    for (long i = 1; i < n; i++) {
        lookup[i] = (16807 * lookup[i-1] + 17) % M;
    }
    return lookup;
}

long *get_u(long *ubars, long n) {
    long *lookup = malloc(sizeof(long) * n);
    for (long i = 0; i<n; i++) {
        lookup[i] = ubars[i%999983];
    }
    return lookup;
}

long C_list(long *lst, long debut, long fin) {
    long reponse = lst[debut];
    for (long i = debut+1; i<=fin; i++) {
        reponse = (2 * reponse + lst[i]) % 997;
    }
    return reponse;
}

long *D(int n, int m, int i, long *lookup) {
    if (i == 0) {
        long *lst = malloc(sizeof(long) * m);
        lst[0] = 0;
        return lst;
    } else {
        long *lst = D(n, m, i-1, lookup);
        if ((lookup[(n + i - 1) % 999983]) % 2 == 0) {
            lst[i] = lst[i-1] - 1;
        } else {
            lst[i] = lst[i-1] + 1;
        }
        return lst;
    }
}

long index_list(long *L, int m) {
    long somme = 0;
    int p = 1;
    for (int i = 0; i < m-1; i++) {
        somme = somme + (L[i+1] - L[i] + 1) / 2 * p;
        p = p * 2;
    }

    return somme;
}

arbre *T(long n, long m, long i, long *lookup) {
    arbre *racine = malloc(sizeof(arbre));
    racine->val = i;
    racine->g = NULL;
    racine->d = NULL;
    if (m == 1) return racine;
    if ((m == 2) || (lookup[n] % 3 == 0)) {
        racine -> g = T(n+1, m-1, i + 1, lookup);
        return racine;
    } 
    racine -> g = T(n+1, floorl((m-1)/2.), i+1, lookup);
    racine -> d = T(n+1 + floorl((m-1)/2.), ceill((m-1)/2.), i+1+floorl((m-1)/2.), lookup);
    return racine;
}

void free_arbre(arbre *A) {
    if (A == NULL) return;
    free(A->g);
    free(A->d);
    free(A);
}

void E(arbre *T, long *lst, long *i) {
    lst[*i] = T->val; 
    *i = *i + 1;
    if (T->g != NULL) {
        E(T->g, lst, i);
        lst[*i] = T->val;
        *i = *i + 1;
    }
    if (T -> d != NULL){
        E(T->d, lst, i);
        lst[*i] = T->val;
        *i = *i + 1;
    }
}

void E_haut(arbre *T, long *lst, long *i, long h) {
    lst[*i] = h; 
    *i = *i + 1;
    if (T->g != NULL) {
        E_haut(T->g, lst, i, h+1);
        lst[*i] = h;
        *i = *i + 1;
    }
    if (T -> d != NULL){
        E_haut(T->d, lst, i, h+1);
        lst[*i] = h;
        *i = *i + 1;
    }
}

long *H(long n, long m, long *lk) {
    long *hauteurs = malloc(sizeof(long) * 2 * m);
    long *i = malloc(sizeof(long)); *i = 0;
    arbre *A = T(n, m, 0, lk);
    E_haut(A, hauteurs, i, 0);

    free(i); free_arbre(A);
    return hauteurs;
}

long find_R(long *lst, long val, long n) {
    for (long i = 0; i < n; i++) {
        if (lst[i] == val) {
            return i;
        }
    }
    return -1;
}

long *R(long n, long m, long *lk) {
    long *Euler = malloc(sizeof(long) * m * 2);
    long *reponse = malloc(sizeof(long) * m);
    long *i = malloc(sizeof(long)); *i = 0;
    arbre *A = T(n, m, 0, lk);
    E(A, Euler, i);
    for (int j = 0; j<m; j++) {
        reponse[j] = find_R(Euler, j, 2 * m - 1);
    }
    free(Euler); free_arbre(A); free(i);
    return reponse;
}

long U(long n, long *L, long x, long len, long *lk) {
    return lk[n+x] % len;
}

long RMQ(long *L, long i, long j) {
    long min = L[i];
    long ind = i;

    for (int k = i+1; k<=j; k++) {
        if (L[k] < min) {
            min = L[k];
            ind = k;
        }
    }

    return ind;
}

long CRMQ(long *L, long n, long k, long len, long *lk) {
    //52
    long *RMQs = malloc(sizeof(long) * k);
    for (int i = 0; i < 2*k; i++) {
        RMQs[i] = RMQ(L, U(n, L, i*2, 2*k-1, lk), U(n, L, i*2+1, 2*k-1, lk));
    }
    long result = C_list(RMQs, 0, k-1);
    free(RMQs);
    return result;
}

void main(void) {
    long *lookup = get_ubars(1000001);
    long *lookup2 = get_u(lookup, 1000001);
    printf("QUESTION 1\n");
    printf("%ld %ld %ld\n \n", lookup[16] % 997, lookup[1024] % 997, lookup[1000000 % 999983] % 997);

    printf("QUESTION 2\n");
    printf("%ld %ld %ld\n\n", C_list(lookup2, 16, 17), C_list(lookup2, 100, 1000), C_list(lookup2, 200, 1000000));

    printf("QUESTION 3\n");
    long *l3a = D(100, 10, 10, lookup);
    long *l3b = D(200, 32, 32, lookup);
    printf("%ld %ld\n\n", index_list(l3a, 10) % 997, index_list(l3b, 32) % 997);
    free(l3a); free(l3b);

    printf("QUESTION 4\n");
    long *l4a = H(16, 4, lookup2);
    long *l4b = H(100, 1000, lookup2);
    long *l4c = H(200, 10000, lookup2);
    printf("%ld %ld %ld\n\n", C_list(l4a, 0, 6), C_list(l4b, 0, 1998), C_list(l4c, 0, 19998));
    free(l4a); free(l4b); free(l4c);

    printf("QUESTION 5\n");
    long *l5a = R(16, 4, lookup2);
    long *l5b = R(100, 1000, lookup2);
    long *l5c = R(200, 10000, lookup2);
    printf("%ld %ld %ld\n", C_list(l5a, 0, 3), C_list(l5b, 0, 999), C_list(l5c, 0, 9999));
    free(l5a); free(l5b); free(l5c);

    printf("QUESTION 6\n");
    printf("%ld\n", CRMQ(H(16, 4, lookup2), 16, 4, 7, lookup2), CRMQ(H(100, 1000)));
}
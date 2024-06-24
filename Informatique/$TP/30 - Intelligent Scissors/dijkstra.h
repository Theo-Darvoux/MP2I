#ifndef _DIJKSTRA_H
#define _DIJKSTRA_H

#define pi 3.141592

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

struct image {
    int haut;
    int larg;
    int max_couleur;
    int **pix;
};

struct couple {
    int x;
    int y;
};

struct tas {
    int dernier;
    int taille;
    struct couple *tab;
};

typedef struct image image;
typedef struct couple couple;
typedef struct tas tas;

couple vide = {.x = -1, .y = -1};

// Convolutions

int laplace[5][5] = {{0, 0, -1, 0, 0}, {0, -1, -2, -1, 0}, {-1, -2, 17, -2, -1}, {0,-1,-2,-1,0}, {0,0,-1,0,0}};
int dx[5][5] = {{0,0,0,0,0},{0,1, 0, -1,0}, {0,2, 0, -2,0}, {0,1, 0, -1,0},{0,0,0,0,0}};
int dy[5][5] = {{0,0,0,0,0},{0,1, 2, 1,0}, {0,0, 0, 0,0}, {0,-1, -2, -1,0},{0,0,0,0,0}};

 
int max(int a, int b);
int min(int a, int b);

image *charger_image(char *fichier);
void sauvegarder_image(char *fichier, image *img);
void free_image(image *img);

void sauvegarder_convolution(char *fichier, image *img, int mat[5][5]);
void colorier(image *img, couple debut, couple fin, couple **pred);
void contour(char *entree, char *sortie, couple *debuts, couple *fins, int n);

int convolution(image *img, couple q, int mat[5][5]);
image *convolution_totale(image *img, int mat[5][5]);
double scalar(couple A, couple B);
double G(image *img, couple q, image *dx_img, image *dy_img);
couple D(image* img, couple q, image *dx_img, image *dy_img);
couple L(image *img, couple p, couple q, image *dx_img, image *dy_img);
double maxG(image *img, image *dx_img, image *dy_img);
double fz(image *img, couple q, image *laplace_img);
double fg(image *img, couple q, double maxg, image *dx_img, image *dy_img);
double fd(image *img, couple p, couple q, image *dx_img, image *dy_img);
double l(image *img, couple p, couple q, double maxg, image *laplace_img, image *dx_img, image *dy_img);

tas *creer_tas(int taille);
void permute(tas *t, int i , int j, int **indices);
void percoler_vers_bas(tas *t, int i, double **poids, int **indices);
void percoler_vers_haut(tas *t, int i, double **poids, int **indices);
void ajouter(tas *t, couple q, double **poids, int **indices);
couple get_min(tas *t, double **poids, int **indices);
bool est_vide(tas *t);
void free_tas(tas *t);

bool dedans(image *img, couple q);
couple *get_voisins(image *img , couple q);
couple **dijkstra(image *img, couple debut);

#endif
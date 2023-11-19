#include "serpent.h"
#include "fichiers.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct serpent *new_serpent(int taille) {
    if (taille <= 0) {
        return NULL;
    }

    int *ecailles = malloc(sizeof(int)*taille);
    for (int i = 0; i<taille; i++) {
        ecailles[i] = 0;
    }

    struct serpent *nouveau = malloc(sizeof(struct serpent));
    nouveau->taille = taille;
    nouveau->tete = 0;
    nouveau->ecailles = ecailles;

    return nouveau;
}

void free_serpent(struct serpent *serpent) {
    free(serpent->ecailles);
    free(serpent);
}

void avance(struct serpent *serpent) {
    for (int i = serpent->taille-1; i>0; i--) {
        serpent->ecailles[i] = serpent->ecailles[i-1];
    }

    serpent->ecailles[0] = serpent->ecailles[0] + rand() % 3 - 1;
    serpent->tete = serpent->tete + 1;
}

bool **image(struct serpent *serpent, int largeur, int hauteur) {
    bool **img = malloc(sizeof(bool*) * hauteur);
    
    for (int y = 0; y<hauteur; y++) {
        bool *ligne = malloc(sizeof(bool) * largeur);
        for (int x = 0; x < largeur; x++) {
            ligne[x] = false;
        }
        img[y] = ligne;
    }

    for (int i = 0; i < serpent->taille; i++) {
        if (serpent->tete - i >= 0 && serpent->tete - i < largeur) {
            img[serpent->ecailles[i]+50][serpent->tete-i]=true;
        }
    }

    return img;
}

void trajet(int taille, int largeur, int hauteur) {
    struct serpent *serpent = new_serpent(taille);
    for (int i = 0; i<largeur; i++) {
        bool **img = image(serpent, largeur, hauteur);
        ecrire_image(i, img, largeur, hauteur);

        for (int j = 0; j<hauteur; j++) {
            free(img[j]);
        }    
        free(img);

        avance(serpent);
    }
}
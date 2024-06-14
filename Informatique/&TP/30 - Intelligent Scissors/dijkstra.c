// COPYRIGHT LE MEILLEUR GROUPE DE COLLE (14 IBRAHIM, THEO G ET THEO D)

#include "dijkstra.h"

int max(int a, int b) {return a > b ? a : b;}
int min(int a, int b) {return a < b ? a : b;}

// Images

image *charger_image(char *fichier){
    FILE *f = fopen(fichier,"r");
    if (f == NULL){
        return NULL;
    }
    image* img = malloc(sizeof(image));

    fscanf(f,"P2");
    fscanf(f,"%d",&(img -> larg));
    fscanf(f,"%d",&(img -> haut));
    fscanf(f,"%d",&(img -> max_couleur));
    img -> pix = malloc((img->haut)*sizeof(int*));
    for (int i = 0;i<img->haut;i++){
        img->pix[i] = malloc((img->larg)*sizeof(int));
    }

    for (int j=0;j<img -> haut;j++){
        for (int k = 0;k<img->larg;k++){
            fscanf(f,"%d",&(img->pix[j][k]));
        }
    }
    fclose(f);
    return img;
}

void sauvegarder_image(char *fichier, image *img) {
    FILE *f = fopen(fichier, "w");
    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", img->larg, img->haut);
    fprintf(f, "255\n");

    for (int y = 0; y < img -> haut; y++) {
        for (int x = 0; x < img -> larg; x++) {
            fprintf(f, "%d ", img->pix[y][x]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void free_image(image *img) {
    for (int i = 0; i < img->haut; i++) {
        free(img->pix[i]);
    }
    free(img);
}

// coloriage

// applique un noyau à l'image entière pour visualiser son effet
void sauvegarder_convolution(char *fichier, image *img, int mat[5][5]) {
    image *convo = convolution_totale(img, mat);
    sauvegarder_image(fichier, convo);
    free_image(convo);
}

void colorier(image *img, couple debut, couple fin, couple **pred) {
    while (fin.x != debut.x || fin.y != debut.y) {
        img->pix[fin.y][fin.x] = 255;
        fin = pred[fin.x][fin.y];
    }
}

void contour(char *entree, char *sortie, couple *debuts, couple *fins, int n) {
    image *img = charger_image(entree);

    // sauvegarder_convolution("laplacienne.pgm", img, laplace);
    // sauvegarder_convolution("dx.pgm", img, dx);
    // sauvegarder_convolution("dy.pgm", img, dy);

    for (int i = 0; i < n; i++) {
        couple **pred = dijkstra(img, debuts[i]);
        colorier(img, debuts[i], fins[i], pred);
        for (int i = 0; i < img->larg; i++) {
            free(pred[i]);
        }
        free(pred);
    }

    sauvegarder_image(sortie, img);

    free_image(img);
}

// Calcul de poids

// applique le noyau mat à img aux coordonnées q
int convolution(image *img, couple q, int mat[5][5]) { 
    int result = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            result = result + img->pix[max(min(q.y + i - 1, img->haut-1), 0)][max(min(q.x + j - 1, img->larg-1), 0)] * mat[i][j];
        }
    }

    return result;
}

image *convolution_totale(image *img, int mat[5][5]) {
    image *result = malloc(sizeof(image));
    result->haut = img->haut; result->larg = img->larg; result->max_couleur = img->max_couleur;
    result->pix = malloc(sizeof(int*) * img->haut);

    for (int i = 0; i < img->haut; i++) {
        result->pix[i] = malloc(sizeof(int) * img->larg);
        for (int j = 0; j < img->larg; j++) {
            couple c = {.x = j, .y = i};
            result->pix[i][j] = convolution(img, c, mat);
        }
    }

    return result;
}

// produit scalaire entre A et B
double scalar(couple A, couple B) {
    double mag = sqrt(A.x * A.x + A.y * A.y);
    double meg = sqrt(B.x * B.x + B.y * B.y);

    if (mag == 0 || meg == 0) return 0;
    return A.x / mag * B.x / meg + A.y / mag * B.y / meg;
}

double G(image *img, couple q, image *dx_img, image *dy_img) {
    int Ix = dx_img->pix[q.y][q.x];
    int Iy = dy_img->pix[q.y][q.x];

    return sqrt(Ix * Ix + Iy * Iy);
}

couple D(image* img, couple q, image *dx_img, image *dy_img){
    couple c = {.x = dy_img->pix[q.y][q.x], .y = -dx_img->pix[q.y][q.x]};
    return c;
}

couple L(image *img, couple p, couple q, image *dx_img, image *dy_img){
    couple d = D(img, p, dx_img, dy_img);
    if (d.x * (q.x - p.x) + d.y * (q.y - p.y) >= 0){
        couple c = {.x = q.x - p.x, .y = q.y - p.y};
        return c;
    }
    couple c = {.x = p.x - q.x, .y = p.y - q.y};
    return c;
}

double maxG(image *img, image *dx_img, image *dy_img){
    double resultat = 0;

    for (int i = 0; i < img->larg; i++){
        for (int j = 0; j < img->haut; j++){
            couple q = {.x = i, .y = j};
            double g_value = G(img, q, dx_img, dy_img);
            resultat = max(resultat, g_value);
        }
    }

    return resultat;
}

double fz(image *img, couple q, image *laplace_img) {
    if (laplace_img->pix[q.y][q.x] == 0) return 0;
    return 1;
}

double fg(image *img, couple q, double maxg, image *dx_img, image *dy_img){
    return 1. - G(img, q, dx_img, dy_img) / maxg;
}

double fd(image *img, couple p, couple q, image *dx_img, image *dy_img){
    couple l1 = L(img, p, q, dx_img, dy_img);
    return 1.0/pi*(acos(scalar(D(img, p, dx_img, dy_img), l1)) + acos(scalar(D(img, q, dx_img, dy_img), l1)));
}

double l(image *img, couple p, couple q, double maxg, image *laplace_img, image *dx_img, image *dy_img){
    return 0.43 * fz(img, q, laplace_img) + 0.43 * fd(img, p, q, dx_img, dy_img) + 0.14 * fg(img, q, maxg, dx_img, dy_img);
}

// Implémentation tas min

tas *creer_tas(int taille){
    tas *t=malloc(sizeof(tas));t->tab=malloc(sizeof(couple)*taille);
    t->dernier=-1;t->taille=taille;
    for(int i=0;i<t->taille;i++){
        t->tab[i]=vide;
    }
    return t;
}

void permute(tas *t, int i , int j, int **indices){
    couple temp = t->tab[i];
    t->tab[i] = t->tab[j];
    t->tab[j] = temp;
    indices[t->tab[i].x][t->tab[i].y] = i;
    indices[t->tab[j].x][t->tab[j].y] = j;
}

void percoler_vers_haut(tas *t, int i, double **poids, int **indices){
    if(i >= 1 && i <= t->dernier){
        if(poids[t->tab[i].x][t->tab[i].y] <= poids[t->tab[(i-1)/2].x][t->tab[(i-1)/2].y]) {
            permute(t, i, (i-1)/2, indices);
            percoler_vers_haut(t, (i-1)/2, poids, indices);
        }
    }
}

void ajouter(tas *t, couple c, double **poids, int **indices){
    t->dernier += 1;
    indices[c.x][c.y] = t->dernier;
    t->tab[t->dernier] = c;
    percoler_vers_haut(t, t->dernier, poids, indices);
}

void percoler_vers_bas(tas *t, int i, double **poids, int **indices){
    if (2 * i + 2 > t->dernier) return;
    couple gauche = t->tab[2 * i + 1];
    couple droite = t->tab[2 * i + 2];
    int j = i;

    if (gauche.x == -1) { // feuille
        return;
    }
    
    if (droite.x == -1 || poids[gauche.x][gauche.y] <= poids[droite.x][droite.y]) {
        j = 2 * i + 1;
    } else {
        j = 2 * i + 2;
    }

    if (poids[t->tab[i].x][t->tab[i].y] >= poids[t->tab[j].x][t->tab[j].y]) {
        permute(t, i, j, indices);
        percoler_vers_bas(t, j, poids, indices);
    }
}

couple get_min(tas *t, double **poids, int **indices){
    couple c = t->tab[0];
    permute(t, 0, t->dernier, indices);
    t->tab[t->dernier] = vide;
    t->dernier = t->dernier - 1;
    percoler_vers_bas(t, 0, poids, indices);
    return c;
}

void afficher_tas(tas *t, int n){
    if(t->dernier >= 0){
        for(int i = 0; i <= n; i++){
            printf("(%d,%d) ", t->tab[i].x, t->tab[i].y);
        }
    }
    printf("\n");
}

bool est_vide(tas *t) {
    return t->dernier == -1;
}

void free_tas(tas *t) {
    free(t->tab);
    free(t);
}

// Graphes

bool dedans(image *img, couple q){
    return (q.x >= 0 && q.x < img->larg) && (q.y >=0 && q.y < img->haut);
}

couple *get_voisins(image *img , couple q){
    couple *voisins = malloc(sizeof(couple) * 9);
    int ind = 0;

    for (int i = q.x - 1; i <= q.x + 1; i++) {
        for (int j = q.y - 1; j <= q.y + 1; j++) {
            couple voisin = {.x = i, .y = j};
            if (dedans(img, voisin)) {
                if (voisin.x == q.x && voisin.y == q.y) continue;
                voisins[ind] = voisin;
                ind = ind + 1;
            }
        }
    }
    
    voisins[ind] = vide; //sentinelle
    return voisins;
}

// Dijkstra

couple **dijkstra(image *img, couple debut) {
    double **poids = malloc(sizeof(double*) * img->larg);
    couple **pred = malloc(sizeof(couple*) * img->larg);
    int **indices = malloc(sizeof(int*) * img->larg); // indice de x,y dans decouverts->tab
    tas *decouverts = creer_tas(img->larg * img->haut);

    // precalculs (réduit enormement le temps d'exécution)
    image *laplace_img = convolution_totale(img, laplace);
    image *dx_img = convolution_totale(img, dx);
    image *dy_img = convolution_totale(img, dy); 
    int maxg = maxG(img, dx_img, dy_img);

    for (int i = 0; i < img->larg; i++) {
        pred[i] = malloc(sizeof(couple) * img->haut);
        poids[i] = malloc(sizeof(double) * img->haut);
        indices[i] = malloc(sizeof(int) * img->haut);
        for (int j = 0; j < img->haut; j++) {
            couple c = {.x = i, .y = j};
            pred[i][j] = vide;
            indices[i][j] = 0;
            poids[i][j] = (i == debut.x && j == debut.y) ? 0.0 : DBL_MAX;
            ajouter(decouverts, c, poids, indices);
        }
    }    

    while (!est_vide(decouverts)) {
        couple q = get_min(decouverts, poids, indices);
        couple *voisins = get_voisins(img, q);
        for (int i = 0; i < 9; i++) {
            
            couple voisin = voisins[i];
            if (voisin.x == -1) break; // sentinelle

            double poids_arete = l(img, q, voisin, maxg, laplace_img, dx_img, dy_img);

            if (poids[q.x][q.y] + poids_arete < poids[voisin.x][voisin.y]) {
                poids[voisin.x][voisin.y] = poids[q.x][q.y] + poids_arete;
                pred[voisin.x][voisin.y] = q;
                percoler_vers_haut(decouverts, indices[voisin.x][voisin.y], poids, indices); // vers le haut car le poids ne peut que baisser
            }
        }
        
        free(voisins);
    }

    for (int i = 0; i < img->larg; i++) {
        free(poids[i]);
        free(indices[i]);
    }

    free(indices);
    free(poids);
    free_tas(decouverts);
    free_image(laplace_img);
    free_image(dx_img);
    free_image(dy_img);

    return pred;
}

int main(void) {
    // pixels où les tracés commencent puis finissent
    couple debuts[2] = {{.x = 281, .y = 57}}; //  {.x = 223, .y = 51}
    couple fins[2] = {{.x = 269, .y = 235}}; //  {.x = 188, .y = 228}

    contour("paul_valery.pgm", "ca_ne_marchera_pas.pgm", debuts, fins, 2);

    return 1;
}
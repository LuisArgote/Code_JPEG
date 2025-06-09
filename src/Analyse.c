#include"Analyse.h"
#include<math.h>
#include<stdlib.h>

extern imageRGB* cropping_matrice(imageRGB* image, int x, int y) {
    if (x < 0 || y < 0 || x >=8 || y >= 8) {
        return NULL; // Invalid cropping parameters
    }

    imageRGB* cropped = (imageRGB*)malloc(sizeof(imageRGB));
    cropped->height = image->height - x;
    cropped->width = image->width - y;
    cropped->image = (color**)malloc(cropped->height * sizeof(color*));

    for (int i = 0; i < cropped->height; i++) {
        cropped->image[i] = (color*)malloc(cropped->width * sizeof(color));
        for (int j = 0; j < cropped->width; j++) {
            cropped->image[i][j].red = image->image[i + x][j + y].red;
            cropped->image[i][j].green = image->image[i + x][j + y].green;
            cropped->image[i][j].blue = image->image[i + x][j + y].blue;
        }
    }
    return cropped;
}

extern int obtenir_taille_fichier(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    int taille = ftell(file);
    fclose(file);
    return taille;
}

void fusion(taille_cropped* tableau, int debut, int milieu, int fin) {
    int i = debut;
    int j = milieu + 1;
    int k = 0;
    taille_cropped* temp = (taille_cropped*)malloc((fin - debut + 1) * sizeof(taille_cropped));

    while (i <= milieu && j <= fin) {
        if (tableau[i].taille_fichier <= tableau[j].taille_fichier) {
            temp[k].x = tableau[i].x;
            temp[k].y = tableau[i].y;
            temp[k].taille_fichier = tableau[i].taille_fichier;
            i++; 
            k++;
        } else {
            temp[k].x = tableau[j].x;
            temp[k].y = tableau[j].y;
            temp[k].taille_fichier = tableau[j].taille_fichier;
            j++;
            k++;
        }
    }

    while (i <= milieu) {
        temp[k] = tableau[i];
        temp[k].x = tableau[i].x;
        temp[k].y = tableau[i].y;
        temp[k].taille_fichier = tableau[i].taille_fichier;
        i++;
        k++;
    }

    while (j <= fin) {
        temp[k] = tableau[j];
        temp[k].x = tableau[j].x;
        temp[k].y = tableau[j].y;
        temp[k].taille_fichier = tableau[j].taille_fichier;
        j++;
        k++;
    }

    for (i = debut, k = 0; i <= fin; i++, k++) {
        tableau[i].taille_fichier = temp[k].taille_fichier;
        tableau[i].x = temp[k].x;
        tableau[i].y = temp[k].y;
    }

    free(temp);
}

extern void tri_fusion(taille_cropped* tableau, int debut, int fin) {
    if (debut < fin) {
        int milieu = (debut + fin) / 2;
        tri_fusion(tableau, debut, milieu);
        tri_fusion(tableau, milieu + 1, fin);
        fusion(tableau, debut, milieu, fin);
    }
}

extern double calcul_moyenne(taille_cropped* tableau) {
    double somme = 0;
    for (int i = 16; i < 64; i++) {
        somme += tableau[i].taille_fichier;
    }
    return somme / (64 - 16);
}

extern double calcul_ecart_type(taille_cropped* tableau, double moyenne) 
{
    double variance = 0.0;
    for (int i = 16; i < 64; i++)
    {
        double diff = tableau[i].taille_fichier - moyenne;
        variance += diff * diff;
    }
    variance /= (64 - 16);
    return sqrt(variance); // Retourne l'écart type
}

extern void afficher_taille_cropped(taille_cropped* tableau) {
    printf("Taille des fichiers pour chaque cropping:\n");
    for (int i = 0; i < 64; i++) {
        printf("Cropping (%d, %d): %d octets\n", tableau[i].x, tableau[i].y, tableau[i].taille_fichier);
    }
}

extern void enregistrer_taille_fichier_dans_csv(taille_cropped* tailles) {
    FILE* file = fopen("taille_fichiers.csv", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier CSV");
        return;
    }
    fprintf(file, "x,y,taille_fichier\n");
    for (int i = 0; i < 64; i++) {
        fprintf(file, "%d,%d,%d\n", tailles[i].x, tailles[i].y, tailles[i].taille_fichier);
    }
    fclose(file);
}
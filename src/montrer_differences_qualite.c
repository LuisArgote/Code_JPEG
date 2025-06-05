#include<stdio.h>
#include<stdlib.h>
#include"imageRGB.h"
#include"YCbCr.h"
#include"matrice.h"
#include"graphique.h"
#include"AC_DC.h"
#include"huffman.h"
#include"jpeg.h"

int main(int argc, char const *argv[])
{
    imageRGB* image = image_from_file(argv[1]);
    if (image == NULL) {
        printf("Erreur lors de l'importation de l'image.\n");
        return 1;
    }
    int q;
    printf("Entrez la qualité de compression (1-100): ");
    scanf("%d", &q);
    if (q < 1 || q > 100) {
        printf("Qualité de compression invalide. Veuillez entrer un nombre entre 1 et 100.\n");
        free_imageRGB(image);
        return 1;
    }
    afficher_compression(image, q);
    return 0;
}
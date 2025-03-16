#ifndef MATRICE_H
#define MATRICE_H

#include"YCbCr.h"

// Définition de la taille de la matrice
#define MATRIX_SIZE 8

// Déclaration de la structure pour une matrice 8x8
typedef struct {
    YCbCr data[8][8]; // Tableau 2D de YCbCr
} Matrice8x8;

typedef struct
{
    int matrice_colonnes;   // Nombre de colonnes
    int matrice_lignes;     // Nombre de lignes
    Matrice8x8** matrice;
}Image_matrice;

/**
 * @brief Initialiser une matrice a partir d'une image YCbCr
 * @param image Image YCbCr
 */
Image_matrice* init_matrice(YCbCrImage* image);

/**
 * @brief renvoie la largeur de l'image
 * @param image Image former par des blocs de 8x8 qu'on va mesurer la largeur
 */
int mesurer_largeur(Image_matrice* image);

/**
 * @brief renvoie la hauteur de l'image
 * @param image Image former par des bloc 8x8 qu'on va mesurer la largeur
 */
int mesurer_hauteur(Image_matrice* image);

/**
 * @brief Transformer les matrices 8x8 en image
 * @param image L'image en matrice a converitr en YCbCrImage
 */
YCbCrImage* image_YCbCr_from_Image_matrice(Image_matrice* image);
#endif
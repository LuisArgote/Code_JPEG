#ifndef MATRICE_H
#define MATRICE_H

#include"YCbCr.h"

// Définition de la taille de la matrice
#define MATRIX_SIZE 8

// Déclaration de la structure pour une matrice 8x8
typedef struct {
    double data[8][8];
} Matrice8x8;

typedef struct
{
    int lignes, colonnes;
    Matrice8x8** matrice;
}matriceComposant;

typedef enum{
    COMPOSANT_Y,
    COMPOSANT_CB,
    COMPOSANT_CR
}Composant;

typedef struct
{
    int data[8][8];
}Matrice8x8Entier;

typedef  struct
{
    int lignes, colonnes;
    Matrice8x8Entier** matrices;
}matriceQuantifier;

/**
 * @brief Initialiser une matrice a partir d'une image YCbCr
 * @param image Image YCbCr
 * @param composant composant à convertir en matrice
 */
matriceComposant* init_matrice(YCbCrImage* image, Composant composant);

/**
 * @brief Obtenir les coefficients de DCT
 * @param entree Matrice8x8 à transformer en DCT
 * @return Matrice8x8 contenant les coefficients DCT
 */
Matrice8x8 calculer_DCT(const Matrice8x8 *entree);

/**
 * @brief renvoie la largeur de l'image
 * @param image Image former par des blocs de 8x8 qu'on va mesurer la largeur
 */
int mesurer_largeur(matriceComposant* image);

/**
 * @brief renvoie la hauteur de l'image
 * @param image Image former par des bloc 8x8 qu'on va mesurer la largeur
 */
int mesurer_hauteur(matriceComposant* image);

/**
 * @brief Obtenir image YCbCr à partir des composants
 * @param y Matrice avec les composants Y
 * @param Cb Matrice avec les composants Cb
 * @param Cr Matrice avec les composants Cr
 */
YCbCrImage* image_YCbCr_from_Image_matrice(matriceComposant* y, matriceComposant* Cb, matriceComposant* Cr);

/***
 * @brief Générer en la matrice de quantification de composant Y
 * @param q quantification, plus q est grande plus lourd est le fichier final
 * mais meilleur reste la qualité. Cas contraire avec un q petit
 */
Matrice8x8* obtenirMatriceQuantificationLum(int q);

/***
 * @brief Processus de quantification
 * @param composant Le composant à quantifier (Y ou Cb ou Cr)
 * @param matriceQuantification matrice de Quantification
 * @return Renvoie la matrice quantifie
 */
matriceQuantifier* quantification(matriceComposant* composant, Matrice8x8* matriceQuantification);

/***
 * @brief fonction ue renvoie le tableau obtenue après un parcours
 * en zigzag caractéristique de la compression JPEG
 * @param matrice matrice avec les coefficients DCT après avoir apliqué la quantification
 */
int* parcours_zigzag(Matrice8x8Entier matrice);

/**
 * @brief afficher la matrice de composant
 * @param matrice matrice à afficher
 */
void afficher_matrice_composant(matriceComposant* matrice);

/**
 * @brief afficher matrice de 8x8
 * @param matrice matrice à afficher
 */
void afficher_matrice_8x8(Matrice8x8* matrice);

#endif
#ifndef YCBCR_H
#define YCBCR_H 

#include <stdio.h>
#include "image_rgb.h"

// Structure pour représenter un pixel YCbCr
typedef struct {
    unsigned char y, Cb, Cr;
} YCbCr;

// Structure pour représenter une image YCbCr
typedef struct {
    int height, width; 
    YCbCr** pixels;    //[IMAGE_HEIGHT][IMAGE_WIDTH];  // Matrice des pixels de l'image
} YCbCrImage;

/**
 * @brief Transforme une image RGB en YCbCr, coefficients prises selon la norme BT.601
 * @param image Image en format RGB à transformer en YCbCr
 */
YCbCrImage* RGBtoYCbCr(image_rgb* image);

/**
 * @brief transformer une image YCbCr à une image RGB
 * @param image Image YCbCr à transformer en RGB
 * @return image_rgb* 
 */
image_rgb* YCbCrtoRGB(YCbCrImage* image);

// Fonction pour charger une image YCbCr depuis un fichier (par exemple un fichier binaire)
int load_ycbcr_image(const char *filename, YCbCrImage *image);

// Fonction pour sauvegarder une image YCbCr dans un fichier (par exemple un fichier binaire)
int save_ycbcr_image(const char *filename, const YCbCrImage *image);

// Fonction pour afficher quelques pixels d'une image YCbCr (utile pour le débogage)
void print_ycbcr_image(const YCbCrImage *image);

// Fonction pour allouer une image YCbCr de taille IMAGE_WIDTH x IMAGE_HEIGHT
void allocate_ycbcr_image(YCbCrImage *image);

// Fonction pour libérer la mémoire allouée pour une image YCbCr
void free_ycbcr_image(YCbCrImage *image);


#endif // YCBCR_H
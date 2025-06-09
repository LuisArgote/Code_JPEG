#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H
#include "imageRGB.h"
#include "YCbCr.h"

typedef struct {
    imageRGB* image;
    int q;
}CompressionData;

/***
 * @brief Afficher dans une fênetre une image RGB
 * @param image image a afficher
 */
void afficher_rgb(imageRGB* image);

/**
 * @brief Afficher les composants Y, Cb et Cr dans trois fênetres
 * @param image image YCbCr à afficher
 */
void afficher_YCbCr(YCbCrImage* image);

/**
 * @brief Afficher l'image compressée dans une fênetre
 * @param image image à afficher
 */
void afficher_compression(imageRGB *image, int q);

/**
 * @brief Afficher l'image compressée dans des différentes qualités
 * @param image image à afficher
 */
void afficher_differences_qualite(imageRGB *image);
#endif
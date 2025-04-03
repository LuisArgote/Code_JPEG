#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H
#include "imageRGB.h"
#include "YCbCr.h"

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

#endif
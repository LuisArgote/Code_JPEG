#ifndef JPEG_H
#define JPEG_H
#include "matrice.h"

typedef struct
{
    matriceQuantifier* ComposantY; // Composant Y quantifier
    matriceQuantifier* ComposantCb; // Composant Cb quantifier
    matriceQuantifier* ComposantCr; // Composant Cr quantifier
    Matrice8x8* matriceDeQuantificationY; // Matrice de quantification pour Y
    Matrice8x8* matriceDeQuantificationChorminence; // Matrice de quantification pour Cb et Cr
}jpeg;


/**
 * @brief Compresse l'image selon la norme JPEG
 * @param image Image en RGB à compresser
 * @param q facteur de qualité q
 * @return jpeg* 
 */
jpeg* compreser_image(imageRGB* image, int q);

imageRGB* decompression_jpeg(jpeg* image);

#endif
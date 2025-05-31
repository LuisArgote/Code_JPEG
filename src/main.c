#include<stdio.h>
#include<stdlib.h>
#include"imageRGB.h"
#include"YCbCr.h"
#include"matrice.h"
#include"graphique.h"
#include"AC_DC.h"

int main(int argc, char const *argv[])
{
    imageRGB* image = image_from_file(argv[1]);
    YCbCrImage* img = RGBtoYCbCr(image);
    matriceComposant* ComposantY = init_matrice(img, COMPOSANT_Y);
    Matrice8x8* matriceDeQuantification = obtenirMatriceQuantificationLum(50);
    matriceQuantifier* MatriceQuantifier = quantification(ComposantY, matriceDeQuantification);
    ac_dc* tableau = obtenir_ac_de_composant(MatriceQuantifier);
    return 0;
}
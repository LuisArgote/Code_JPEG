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
    YCbCrImage* ycbcr_image = RGBtoYCbCr(image);
    matriceComposant* Y = init_matrice(ycbcr_image, COMPOSANT_Y);
    matriceComposant* dct_y = convertir_composant_en_dct(Y);
    Matrice8x8* quantification_matrix = obtenirMatriceQuantificationLum(50);
    matriceQuantifier* quantifier_y = quantification(dct_y, quantification_matrix);
    ac* ac_y = obtenir_ac_de_composant(quantifier_y);
    enregistrer_fichier_compresser(ac_y, "compressed_fichier.bin");
    return 0;
}
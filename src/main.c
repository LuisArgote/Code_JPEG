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
    if (!image) {
        printf("Erreur lors du chargement de l'image.\n");
        return 1;
    }
    jpeg* image_compresser = compreser_image(image, 50);
    imageRGB* image_final = decompression_jpeg(image_compresser);
    afficher_rgb(image_final);
    return 0;
}
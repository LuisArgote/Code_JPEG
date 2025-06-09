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
    afficher_YCbCr(ycbcr_image);
    return 0;
}

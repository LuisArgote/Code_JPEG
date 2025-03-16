#include<stdio.h>
#include<stdlib.h>
#include"image_rgb.h"
#include"YCbCr.h"
#include"matrice.h"

int main(int argc, char const *argv[])
{
    image_rgb* image = image_from_file(argv[1]);
    YCbCrImage* image_ycbcr = RGBtoYCbCr(image);
    Image_matrice* matrice = init_matrice(image_ycbcr);
    YCbCrImage* image_depuis_matrice = image_YCbCr_from_Image_matrice(matrice);
    //free(image);
    enregistrer_image(YCbCrtoRGB(image_depuis_matrice), "prueba3.ppm");
    //enregistrer_image(image, "prueba1.ppm");
    return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include"imageRGB.h"
#include"YCbCr.h"
#include"matrice.h"
#include"graphique.h"

int main(int argc, char const *argv[])
{
    imageRGB* image = image_from_file(argv[1]);
    afficher_rgb(image);
    return 0;
}

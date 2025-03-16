#include"YCbCr.h"
#include<stdlib.h>

YCbCrImage* RGBtoYCbCr(image_rgb* image)
{
    YCbCrImage* image_YCbCr = (YCbCrImage*)malloc((sizeof(YCbCrImage)));
    image_YCbCr->pixels = (YCbCr**)malloc(sizeof(YCbCr*) * image->image_height);
    for (int i = 0; i < image->image_height; i++)
    {
        image_YCbCr->pixels[i] = (YCbCr*)malloc(sizeof(YCbCr) * image->image_width);
        for (int j = 0; j < image->image_width; j++)
        {
            image_YCbCr->pixels[i][j].y = (unsigned char)(0.299 * image->image[i][j].red +
                                                          0.587 * image->image[i][j].green +
                                                          0.114 * image->image[i][j].blue);

            image_YCbCr->pixels[i][j].Cb = (unsigned char)(128 - 0.168736 * image->image[i][j].red -
                                                           0.331264 * image->image[i][j].green +
                                                           0.5 * image->image[i][j].blue);

            image_YCbCr->pixels[i][j].Cr = (unsigned char)(128 + 0.5 * image->image[i][j].red -
                                                           0.418688 * image->image[i][j].green -
                                                           0.081312 * image->image[i][j].blue);
        }
    }
    image_YCbCr->height = image->image_height;
    image_YCbCr->width = image->image_width;
    return image_YCbCr;
}

image_rgb* YCbCrtoRGB(YCbCrImage* image)
{
    image_rgb* resultat = (image_rgb*)malloc(sizeof(image_rgb));
    resultat->image_height = image->height;
    resultat->image_width = image->width;
    resultat->maxval = 0;
    resultat->image = (color**)malloc(sizeof(color*) * image->height);
    for (int i = 0; i < image->height; i++)
    {
        resultat->image[i] = (color*)malloc(sizeof(color) * image->width);
        for (int j = 0; j < image->width; j++)
        {
            int y = image->pixels[i][j].y;
            int Cb = image->pixels[i][j].Cb;
            int Cr = image->pixels[i][j].Cr;
            // Calcul de couleur rouge
            int red = (int)(y + 1.402*(Cr-128));
            if (red > 255)
            {
                red = 255;
            }
            if (red < 0)
            {
                red = 0;
            }
            if (red > resultat->maxval)
            {
                resultat->maxval = red;
            }
            resultat->image[i][j].red = red;

            // Calcul de couleur vert
            int green = (int)(y - 0.344136 * (Cb - 128) - 0.714136 * (Cr - 128));
            if (green > 255)
            {
                green = 255;
            }
            if (green < 0)
            {
                green = 0;
            }
            if (green > resultat->maxval)
            {
                resultat->maxval = green;
            }
            resultat->image[i][j].green = green;

            // Calcul de couleur bleu
            int blue = (int)(y + 1.772 * (Cb - 128));
            if (blue > 255)
            {
                blue = 255;
            }
            if (blue < 0)
            {
                blue = 0;
            }
            if (blue > resultat->maxval)
            {
                resultat->maxval = blue;
            }
            resultat->image[i][j].blue = blue;
        }
    }
    return resultat;
}
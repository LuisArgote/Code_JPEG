#include"YCbCr.h"
#include<stdlib.h>

YCbCrImage* RGBtoYCbCr(imageRGB* image)
{
    YCbCrImage* image_YCbCr = (YCbCrImage*)malloc((sizeof(YCbCrImage)));
    image_YCbCr->pixels = (YCbCr**)malloc(sizeof(YCbCr*) * image->height);
    for (int i = 0; i < image->height; i++)
    {
        image_YCbCr->pixels[i] = (YCbCr*)malloc(sizeof(YCbCr) * image->width);
        for (int j = 0; j < image->width; j++)
        {
            double r = image->image[i][j].red / 255.0;
            double g = image->image[i][j].green / 255.0;
            double b = image->image[i][j].blue / 255.0;
            image_YCbCr->pixels[i][j].y = 0.299 * r + 0.587 * g + 0.114 * b;
            image_YCbCr->pixels[i][j].Cb = -0.1687 * r - 0.3313 * g + 0.5 * b + 0.5;
            image_YCbCr->pixels[i][j].Cr = 0.5 * r - 0.4187 * g - 0.0813 * b + 0.5;
            // Eviter les dépassement
            image_YCbCr->pixels[i][j].y = (image_YCbCr->pixels[i][j].y < 0) ? 0 : (image_YCbCr->pixels[i][j].y > 1) ? 1 : image_YCbCr->pixels[i][j].y;
            image_YCbCr->pixels[i][j].Cb = (image_YCbCr->pixels[i][j].Cb < 0) ? 0 : (image_YCbCr->pixels[i][j].Cb > 1) ? 1 : image_YCbCr->pixels[i][j].Cb;
            image_YCbCr->pixels[i][j].Cr = (image_YCbCr->pixels[i][j].Cr < 0) ? 0 : (image_YCbCr->pixels[i][j].Cr > 1) ? 1 : image_YCbCr->pixels[i][j].Cr;
        }
    }
    image_YCbCr->height = image->height;
    image_YCbCr->width = image->width;
    return image_YCbCr;
}

imageRGB* YCbCrtoRGB(YCbCrImage* image)
{
    imageRGB* resultat = (imageRGB*)malloc(sizeof(imageRGB));
    resultat->height = image->height;
    resultat->width = image->width;
    resultat->maxval = 0;
    resultat->image = (color**)malloc(sizeof(color*) * image->height);
    for (int i = 0; i < image->height; i++)
    {
        resultat->image[i] = (color*)malloc(sizeof(color) * image->width);
        for (int j = 0; j < image->width; j++)
        {
            int y = image->pixels[i][j].y * 255.0;
            int Cb = image->pixels[i][j].Cb * 255.0;
            int Cr = image->pixels[i][j].Cr * 255.0;
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
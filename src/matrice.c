#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"

// Fonction pour initialiser la matrice avec une valeur
Image_matrice* init_matrice(YCbCrImage* image)
{
    Image_matrice* image_matrice = (Image_matrice*)malloc(sizeof(Image_matrice));
    if (image->width % 8 == 0)
    {
        image_matrice->matrice_colonnes = image->width / 8;
    }
    else
    {
        image_matrice->matrice_colonnes = (image->width / 8) + 1;
    }
    if (image->height % 8 == 0)
    {
        image_matrice->matrice_lignes = image->height / 8;
    }
    else
    {
        image_matrice->matrice_lignes = (image->height / 8) + 1;
    }
    image_matrice->matrice = (Matrice8x8**)malloc(sizeof(Matrice8x8*) * image_matrice->matrice_lignes);
    for (int i = 0; i < image_matrice->matrice_lignes; i++)
    {
        image_matrice->matrice[i] = (Matrice8x8*)malloc(sizeof(Matrice8x8) * image_matrice->matrice_colonnes);
        for (int l = 8*i; l < 8*(i+1); l++)
        {
            for (int j = 0; j < image_matrice->matrice_colonnes; j++)
            {
                for (int k = 8 * j; k < 8 * (j+1); k++)
                {
                    if (k < image->width && l < image->height)
                    {
                        image_matrice->matrice[i][j].data[l - 8*i][k - 8*j].Cb = image->pixels[l][k].Cb;
                        image_matrice->matrice[i][j].data[l - 8*i][k - 8*j].Cr = image->pixels[l][k].Cr;
                        image_matrice->matrice[i][j].data[l - 8*i][k - 8*j].y = image->pixels[l][k].y;
                    }
                    else
                    {
                        image_matrice->matrice[i][j].data[l- 8*i][k - 8*j].Cb = 0;
                        image_matrice->matrice[i][j].data[l - 8*i][k - 8*j].Cr = 0;
                        image_matrice->matrice[i][j].data[l - 8*i][k - 8*j].y = 0;
                    }
                }             
            }
        } 
    }
    return image_matrice;
}

int mesurer_largeur(Image_matrice* image)
{
    int width = (image->matrice_colonnes - 1) * 8;
    for (int i = 0; i < 8; i++)
    {
        if(image->matrice[0][image->matrice_colonnes-1].data[0][i].Cb != 0)
        {
            width++;
        }
        else
        {
            return width;
        }
        
    }
    return width;
}

int mesurer_hauteur(Image_matrice* image)
{
    int height = (image->matrice_lignes - 1) * 8;
    for (int i = 0; i < 8; i++)
    {
        if(image->matrice[image->matrice_lignes - 1][0].data[i][0].Cb != 0)
        {
            height++;
        }
        else
        {
            return height;
        }
        
    }
    return height;
}

YCbCrImage* image_YCbCr_from_Image_matrice(Image_matrice* image)
{
    YCbCrImage* image_resultant = (YCbCrImage*)malloc(sizeof(YCbCrImage));
    image_resultant->width = mesurer_largeur(image);
    image_resultant->height = mesurer_hauteur(image);
    image_resultant->pixels = (YCbCr**)malloc(sizeof(YCbCr*) * image_resultant->height);
    for (int i = 0; i < image_resultant->height; i++)
    {
        int block_hauteur = i/8;
        image_resultant->pixels[i] = (YCbCr*)malloc(sizeof(YCbCr) * image_resultant->width);
        for (int j = 0; j < image_resultant->width; j++)
        {
            int block_largeur = j/8;
            image_resultant->pixels[i][j] = image->matrice[block_hauteur][block_largeur].data[i%8][j%8];
        }
    }
    return image_resultant;
}
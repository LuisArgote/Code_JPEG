#include <stdlib.h>
#include <stdio.h>
#include "imageRGB.h"

extern imageRGB* image_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");// ouverture de fichier
    if(!file)
    {
        perror("Erreur lors de l'ouverture de fichier\n");
        return NULL;
    }
    imageRGB* image = (imageRGB*)malloc(sizeof(imageRGB));
    char ligne[256]; // Ligne de fichier
    fgets(ligne, 256, file);
    fscanf(file, "%d", &image->width); // Lire la largeur
    fscanf(file, "%d", &image->height);// Lire la hauteur
    fscanf(file, "%d", &image->maxval);  // Lire l'intensité maximal
    image->image = (color**)malloc(sizeof(color*) * image->height);
    for (int i = 0; i < image->height; i++)
    {
        image->image[i] = (color*)malloc(sizeof(color) * image->width);
        for (int j = 0; j < image->width; j++)
        {
            fscanf(file, "%d", &image->image[i][j].red);
            fscanf(file, "%d", &image->image[i][j].green);
            fscanf(file, "%d", &image->image[i][j].blue);
            if (image->image[i][j].red > image->maxval ||
                image->image[i][j].green > image->maxval ||
                image->image[i][j].blue > image->maxval)
            {
                printf("Erreur une valeur depasse le maxval\n");
                return NULL;
            }
        }
    }
    fclose(file); // fermeture de fichier
    return image;
}

extern void enregistrer_image(imageRGB* image, const char* nom_fichier)
{
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL)
    {
        perror("Erreur lors de la création de fichier\n");
        return;
    }

    // Ecrire l'en-tête du fichier PPM
    fprintf(fichier, "P3\n");
    fprintf(fichier, "%d %d\n", image->width, image->height);
    fprintf(fichier, "%d\n", image->maxval);

    // Ecrire les données des pixels (R, G, B)
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            fprintf(fichier, "%d %d %d\n", image->image[i][j].red, image->image[i][j].green, image->image[i][j].blue);
        }
    }
    fclose(fichier);
}

void free_imageRGB(imageRGB* image)
{
    for (int i = 0; i < image->height; i++)
    {
        free(image->image[i]);
    }
    free(image->image);
    free(image);
}
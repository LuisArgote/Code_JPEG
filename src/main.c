#include<stdio.h>
#include<stdlib.h>
#include"imageRGB.h"
#include"YCbCr.h"
#include"matrice.h"
#include"graphique.h"
#include"AC_DC.h"
#include"huffman.h"
#include"jpeg.h"
#include"Analyse.h"

int main(int argc, char const *argv[])
{
    imageRGB* image = aligner_taille_image(image_from_file(argv[1]));
    taille_cropped taille_fichier[64];
    int indice = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            imageRGB* cropped_image = cropping_matrice(image, x, y);
            jpeg* jpeg_image = compreser_image(cropped_image, 100);
            char temp_filename[100];
            snprintf(temp_filename, sizeof(temp_filename), "compressed/compressed_image_%d_%d.jpeg", x, y);
            ac* tableau = obtenir_ac_de_composant(jpeg_image->ComposantY);
            enregistrer_fichier_compresser(tableau, temp_filename);
            taille_fichier[indice].taille_fichier = obtenir_taille_fichier(temp_filename);
            taille_fichier[indice].x = x;
            taille_fichier[indice].y = y;
            indice++;
            free_imageRGB(cropped_image);
            free_ac(tableau);
            free_jpeg(jpeg_image);
        }
    }
    enregistrer_taille_fichier_dans_csv(taille_fichier);


    tri_fusion(taille_fichier, 0, indice - 1);
    afficher_taille_cropped(taille_fichier);
    double moyenne = calcul_moyenne(taille_fichier);
    double ecart_type = calcul_ecart_type(taille_fichier, moyenne);;
    if (taille_fichier[0].taille_fichier < moyenne - 1* ecart_type)
    {
        printf("Le debut de la grid est (%d,%d)\n", taille_fichier[0].x, taille_fichier[0].y);
    }
    else
    {
        printf("Aucun cropping n'a une taille de fichier significativement plus petite que la moyenne.\n");
    }
    return 0;
}

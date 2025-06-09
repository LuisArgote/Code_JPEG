#ifndef IMAGE_RGB
#define IMAGE_RGB

#include "color.h"

// Structure pour representer une image en rgb
typedef struct imageRGB
{
    int width;        // Largeur de l'image en pixels
    int height;       // Hauteur de l'image en pixels
    int maxval;             // Valeur maximal qui peut prendre un pixel (généralement 255 pour une image 8 bits)
    color **image;          // Pixels de l'image
}imageRGB;

/**
 * @brief Il créer une image RGB à partir d'un fichier PPM P3
 * @param filename nom de fichier ppm
 */
imageRGB* image_from_file(const char *filename);

/**
 * @brief Il enregistre l'image sur un fichier, utile pour tester l'état d'une image
 * @param image Image à sauvegarder dans un fichier
 * @param nom_fichier Nom de fichier ou sauvegarder une image, attention le nom doit finir par .ppm
 */
void enregistrer_image(imageRGB* image, const char* nom_fichier);

/**
 * @brief afficher l'image avec la librerie opencv
 * @param image image à afficher
 */
void afficher_image(imageRGB* image);

/**
 * @brief Liberer mémoire pour l'image
 * @param image image à libérer
 */
void free_imageRGB(imageRGB* image);

/**
 * @brief Aligner taille de l'image pour qu'il soit multiple de 8
 * 
 */
imageRGB* aligner_taille_image(imageRGB* image);
#endif // imageRGB
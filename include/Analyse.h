#ifndef ANALYSE_H
#define ANALYSE_H

#include "matrice.h"

typedef struct {
    int x;
    int y;
    int taille_fichier;
}taille_cropped;

/**
 * @brief Cropping l'image de x et y
 * @param matrice 
 * @param x 
 * @param y 
 * @return matriceComposant* 
 */
imageRGB* cropping_matrice(imageRGB* image, int x, int y);

/**
 * @brief Obtenir la taille du fichier
 * @param filename Nom du fichier à analyser
 * @return int Taille du fichier en octets
 */
int obtenir_taille_fichier(const char* filename);

/**
 * @brief Tri fusion pour trier les tailles des fichiers
 * @param tableau Tableau de taille_cropped à trier
 * @param debut Indice de début du tableau
 * @param fin Indice de fin du tableau
 */
void tri_fusion(taille_cropped* tableau, int debut, int fin);

/**
 * @brief Fusionne deux sous-tableaux dans le tableau principal
 * @param tableau Tableau de taille_cropped à fusionner
 * @param debut Indice de début du sous-tableau gauche
 * @param milieu Indice de milieu du tableau
 * @param fin Indice de fin du sous-tableau droit
 */
double calcul_moyenne(taille_cropped* tableau);

/**
 * @brief calculer la variance des tailles de fichiers
 * 
 */
double calcul_ecart_type(taille_cropped* tableau, double moyenne);

void afficher_taille_cropped(taille_cropped* tableau);

void enregistrer_taille_fichier_dans_csv(taille_cropped* tailles);
#endif
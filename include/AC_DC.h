#ifndef AC_DC_H
#define AC_DC_H

#include "matrice.h"

typedef struct
{
    int taille_tableau; // Taille de Tableau
    int* valeur; // Valeurs
    int* taille; // Longueur de répétition des valeurs
}ac_dc;

/**
 * @brief Obtenir la matrice ac de la matrice quantifier
 * @param matrice matrice quantifier à obtenir ses coefficient ac
 * @return coefficients ac de la matrice
 */
ac_dc* obtenir_ac_de_composant(matriceQuantifier* matrice);

/***
 * @brief Fonction pour obtenir ac à partir d'une matrice d'entier
 * @param matrice matrice à obtenir son tableau AC
 */
ac_dc* obtenir_ac_de_matrice(Matrice8x8Entier matrice);

/***
 * @brief afficher les valeurs de l'estructure AC_DC
 * @param imprimer structure à afficher ses valeurs
 */
void print_ac_dc(ac_dc* imprimer);
#endif
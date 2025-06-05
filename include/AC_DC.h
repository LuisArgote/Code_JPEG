#ifndef AC_DC_H
#define AC_DC_H

#include "matrice.h"

typedef struct
{
    int taille_tableau; // Taille de Tableau
    int* valeur; // Valeurs
    int* taille; // Longueur de répétition des valeurs
}ac;

typedef struct
{
    int taille_tableau; // taille de tableau
    int* tableau;    // tableau avec les valeurs
}dc;


/**
 * @brief Obtenir la matrice ac de la matrice quantifier
 * @param matrice matrice quantifier à obtenir ses coefficient ac
 * @return coefficients ac de la matrice
 */
ac* obtenir_ac_de_composant(matriceQuantifier* matrice);

/***
 * @brief Fonction pour obtenir ac à partir d'une matrice d'entier
 * @param matrice matrice à obtenir son tableau AC
 */
ac* obtenir_ac_de_matrice(Matrice8x8Entier* matrice);

/***
 * @brief afficher les valeurs de l'estructure AC_DC
 * @param imprimer structure à afficher ses valeurs
 */
void print_ac(ac* imprimer);

/**
 * @brief Obtenir le DC de composant
 * @param matrice matrice à obtenir le DC
 * @return tableau DC
 */
dc* obtenir_dc_de_composant(matriceQuantifier* matrice);

/**
 * @brief Imprimer un DC
 * @param imprimer structure dc à imprimer
 */
void print_dc(dc* imprimer);
#endif
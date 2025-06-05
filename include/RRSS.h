#ifndef RRSS_H
#define RRSS_H

#include"AC_DC.h"
#include<stdbool.h>

typedef struct
{
    int runlenght; // taille%16
    int size;   // Nombre de bits nécessaire pour coder la vallue
    int nombre_zrl; // Nombre de ZRL, avec ZRL 16 nombres 0 
}RRSS;

/**
 * @brief Calculer le RSS d'un couple runlength, value
 * @param taille nombre de 0 avant le nombre value
 * @param value nombre qu'apparait avec un quantité de 0 égal à runlength
 * @return le couple runlength, size correspondant
 */
RRSS* calculer_RRSS(int taille, int value);


/**
 * @brief Fonction que compare deux structure RRSS
 * @param couple1 
 * @param couple2 
 * @return true Si les deux structures sont égaux
 * @return false sinon
 */
bool comparer_RRSS(RRSS* couple1, RRSS* couple2);
#endif
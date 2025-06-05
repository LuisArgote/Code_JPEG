#include "RRSS.h"
#include<stdlib.h>

// Calcul de la valeur absolue pour un nombre
int abs(int n)
{
    if (n>0)
    {
        return n;
    }
    else
    {
        return -n;
    }
}

// Fonction de calcul des bits nécessaires pour coder un nombre
int calcul_des_bits(int nombre)
{
    int nb = 1;
    while (nombre/2)
    {
        nombre /= 2;
        nb++;
    }
    return nb;
}

extern RRSS* calculer_RRSS(int taille, int value)
{
    RRSS* resultat = (RRSS*)malloc(sizeof(RRSS));
    resultat->nombre_zrl = taille / 16;
    resultat->runlenght = taille % 16;
    resultat->size = calcul_des_bits(abs(value));
    return resultat;   
}

extern bool comparer_RRSS(RRSS* couple1, RRSS* couple2)
{
    if (couple1->runlenght == couple2->runlenght  && couple1->size == couple2->size)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}
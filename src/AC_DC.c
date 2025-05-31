#include"AC_DC.h"
#include<stdbool.h>
#include<stdlib.h>

extern ac_dc* obtenir_ac_de_matrice(Matrice8x8Entier  matrice)
{
    int* tableau = parcours_zigzag(matrice);
    ac_dc* resultat = (ac_dc*)malloc(sizeof(ac_dc));
    resultat->valeur = (int*)malloc(sizeof(int) * 63);
    resultat->taille = (int*)malloc(sizeof(int) * 63);
    resultat->taille_tableau = 0;
    int nombre_zero = 0;
    for (int i = 1; i < 64; i++)
    {
        if (tableau[i] != 0)
        {
            resultat->taille[resultat->taille_tableau] = nombre_zero;
            resultat->valeur[resultat->taille_tableau] = tableau[i];
            resultat->taille_tableau++;
            nombre_zero = 0;
            continue;
        }
        nombre_zero++;
    }
    resultat->taille[resultat->taille_tableau] = nombre_zero;
    resultat->valeur[resultat->taille_tableau] = 0;
    resultat->taille_tableau++;
    resultat->valeur = realloc(resultat->valeur, resultat->taille_tableau);
    resultat->taille = realloc(resultat->taille, resultat->taille_tableau);
    return resultat;
}

extern ac_dc* obtenir_ac_de_composant(matriceQuantifier* matrice)
{
    ac_dc* resultat = (ac_dc*)malloc(sizeof(ac_dc));
    resultat->taille = (int*)malloc(sizeof(int) * 63); // Peut importe la taille, il sera modifier après
    resultat->valeur = (int*)malloc(sizeof(int) * 63);
    resultat->taille_tableau = 0;
    for (int i = 0; i < matrice->lignes; i++)
    {
        for (int j = 0; j < matrice->colonnes; j++)
        {
            ac_dc* temp = obtenir_ac_de_matrice(matrice->matrices[i][j]);
            for (int i = resultat->taille_tableau; i < temp->taille_tableau + resultat->taille_tableau; i++)
            {
                resultat->taille[i] = temp->taille[resultat->taille_tableau - i];
                resultat->valeur[i] = temp->valeur[resultat->taille_tableau - i];    
            }
            resultat->taille_tableau+= temp->taille_tableau;
            free(temp);
        }
    }
    return resultat;
}

extern void print_ac_dc(ac_dc* imprimer)
{
    printf("[");
    for (int i = 0; i < imprimer->taille_tableau; i++)
    {
        printf("(%d, %d), ", imprimer->taille[i], imprimer->valeur[i]);
    }
    printf("]\n");
}
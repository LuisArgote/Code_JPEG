#include"AC_DC.h"
#include<stdbool.h>
#include<stdlib.h>

extern ac* obtenir_ac_de_matrice(Matrice8x8Entier*  matrice)
{
    int* tableau = parcours_zigzag(matrice);
    ac* resultat = (ac*)malloc(sizeof(ac));
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
    void* tmp1 = realloc(resultat->valeur, sizeof(int) * resultat->taille_tableau);
    if (tmp1)
    {
        resultat->valeur = tmp1;
    }
    void* tmp2 = realloc(resultat->taille, sizeof(int)*resultat->taille_tableau);
    if (tmp2)
    {
        resultat->taille = tmp2;
    }
    return resultat;
}

extern ac* obtenir_ac_de_composant(matriceQuantifier* matrice)
{
    ac* resultat = (ac*)malloc(sizeof(ac));
    resultat->taille = NULL;
    resultat->valeur = NULL;
    resultat->taille_tableau = 0;
    for (int i = 0; i < matrice->lignes; i++)
    {
        for (int j = 0; j < matrice->colonnes; j++)
        {
            ac* temp = obtenir_ac_de_matrice(&(matrice->matrices[i][j]));
            int nouvelle_taille = resultat->taille_tableau + temp->taille_tableau;
            void* tmp1 = realloc(resultat->taille, sizeof(int) * nouvelle_taille);
            void* tmp2 = realloc(resultat->valeur, sizeof(int) * nouvelle_taille);
            if (tmp1)
            {
                resultat->taille = tmp1;
            }
            else
            {
                printf("Erreur\n");
            }
            if (tmp2)
            {
                resultat->valeur = tmp2;
            }
            else
            {
                printf("Erreur\n");
            }
            for (int i = 0; i < temp->taille_tableau; i++)
            {
                resultat->taille[resultat->taille_tableau + i] = temp->taille[i];
                resultat->valeur[resultat->taille_tableau + i] = temp->valeur[i];    
            }
            resultat->taille_tableau = nouvelle_taille;
            free(temp);
        }
    }
    return resultat;
}

extern void print_ac(ac* imprimer)
{
    printf("[");
    for (int i = 0; i < imprimer->taille_tableau; i++)
    {
        printf("(%d, %d), ", imprimer->taille[i], imprimer->valeur[i]);
    }
    printf("]\n");
}

extern dc* obtenir_dc_de_composant(matriceQuantifier* matrice)
{
    // Pré-conditions: La matrice ne doit pas être vide
    dc* resultat = (dc*)malloc(sizeof(dc));
    resultat->taille_tableau = matrice->colonnes * matrice->lignes;
    resultat->tableau = (int*)malloc(sizeof(int) * resultat->taille_tableau);
    int indice = 1;
    for (int i = 0; i < matrice->lignes; i++)
    {
        for (int j = 0; j < matrice->colonnes; j++)
        {
            if (i==0 && j==0)
            {
                resultat->tableau[0] = matrice->matrices[0][0].data[0][0];
                continue;
            }
            if (j == 0)
            {
                resultat->tableau[indice] = matrice->matrices[i][j].data[0][0] -
                matrice->matrices[i-1][matrice->colonnes - 1].data[0][0];
                indice++;
            }  
            else
            {
                resultat->tableau[indice] = matrice->matrices[i][j].data[0][0] -
                matrice->matrices[i][j-1].data[0][0];
                indice++;
            }
        }
    }
    return resultat;
}

extern void print_dc(dc* imprimer)
{
    printf("[%d", imprimer->tableau[0]);
    for (int i = 1; i < imprimer->taille_tableau; i++)
    {
        printf(", %d", imprimer->tableau[i]);
    }
    printf("]\n");
}
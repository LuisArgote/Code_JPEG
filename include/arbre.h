#ifndef ARBRE_H
#define ARBRE_H
#include<stdbool.h>

// Structure pour representer un arbre
typedef struct arbre
{
    int etiquette;
    bool vide;
    struct arbre* sousArbreGauche;
    struct arbre* sousArbreDroite;
}arbre;

/**
 * @brief Générer Arbre Huffman
 * @param tableau Tableau avec des nombres entier
 * @return Arbre de Huffman de tableau
 */
arbre* genererArbreHuffman(int* tableau);

#endif
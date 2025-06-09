#ifndef HUFFMAN_H
#define HUFFMAN_H
#include<stdbool.h>
#include"AC_DC.h"
#include "RRSS.h"

// Structure pour representer un arbre
typedef struct arbre
{
    RRSS* etiquette;
    bool vide;
    int frequences;
    struct arbre* sousArbreGauche;
    struct arbre* sousArbreDroite;
}arbre;

typedef struct liste_arbres
{
    arbre* a;
    struct liste_arbres* suivant;
}liste_arbres;

typedef struct huffman_liste
{
    RRSS* etiquette;
    char* code; // Code Huffman
    int taille;
    struct huffman_liste* suivant;
}huffman_liste;

/**
 * @brief Générer Arbre Huffman
 * @param tableau Tableau avec les coefficients AC
 * @return Arbre de Huffman de tableau
 */
arbre* genererArbreHuffmanAC(ac* tableau);

/**
 * @brief Ajouter une nouvelle valeur à la liste
 * @param liste 
 * @param rrss Structure RRSS à ajouter à la liste
 */
void ajouter_RRSS(liste_arbres* liste, RRSS* rrss);

/**
 * @brief Affiche la liste des occurrence
 * @param liste 
 */
void afficher_liste_occurrences(liste_arbres* liste);

/**
 * @brief Obtenir la listes des occurrences de tableau ac
 * @param tableau 
 * @return liste avec les occurrences de ac
 */
liste_arbres* obtenir_liste_des_occurences_ac(ac* tableau);


/**
 * @brief Calculer la taille d'une liste
 * 
 * @param liste 
 * @return la taille de la liste
 */
int calculer_taille_liste(liste_arbres* liste);

/**
 * @brief Fusionne les deux abres avec les occurrences plus faibles
 * dans la liste, cette fussion réduit la taille de 1
 * @param liste liste avec d'arbres
 */
liste_arbres* fusionne(liste_arbres* liste);

/**
 * @brief Afficher un arbre
 * @param a arbre a afficher
 */
void afficher_arbre(arbre* a, int nombre);

/**
 * @brief Calculer la profondeur d'un arbre
 * @param a Arbre dont on veut calculer la profondeur
 * @return La profondeur de l'arbre
 */
int profondeur_arbre(arbre* a);

/**
 * @brief Créer une liste de Huffman à partir d'une liste d'occurrences
 * @param arbre_huffman Arbre de Huffman
 * @return Liste de Huffman
 */
huffman_liste* creer_huffman_liste(arbre* arbre_huffman);


/**
 * @brief enregistre les séquences de Huffman dans un fichier
 * @param tableau_ac 
 * @param nom_fichier Nom du fichier dans lequel on va enregistrer les séquences
 */
void enregistrer_fichier_compresser(ac* tableau_ac, const char* nom_fichier);

#endif
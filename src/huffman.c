#include"huffman.h"
#include<stdio.h>
#include<stdlib.h>

extern void ajouter_RRSS(liste_arbres* liste, RRSS* rrss)
{
    liste->a->frequences += rrss->nombre_zrl;
    if (rrss->runlenght == 0 && rrss->size == 0)
    {
        return;
    }
    liste_arbres* temp = liste;
    while (temp->suivant != NULL)
    {
        if (comparer_RRSS(temp->suivant->a->etiquette, rrss))
        {
            temp->suivant->a->frequences++;
            return;
        }
        temp = temp->suivant;
    }
    temp->suivant = (liste_arbres*)malloc(sizeof(liste_arbres));
    temp->suivant->a = (arbre*)malloc(sizeof(arbre));
    temp->suivant->a->etiquette = rrss;
    temp->suivant->a->frequences = 1;
    temp->suivant->a->sousArbreDroite = NULL;
    temp->suivant->a->sousArbreGauche = NULL;
    temp->suivant->suivant = NULL;
}

extern liste_arbres* obtenir_liste_des_occurences_ac(ac* tableau)
{
    liste_arbres* resultat = (liste_arbres*)malloc(sizeof(liste_arbres));
    resultat->a = (arbre*)malloc(sizeof(arbre));
    resultat->a->etiquette = (RRSS*)malloc(sizeof(RRSS));
    resultat->a->etiquette->nombre_zrl = 1;
    resultat->a->etiquette->runlenght = 0;
    resultat->a->etiquette->size = 0;
    resultat->a->frequences = 0;
    resultat->a->sousArbreDroite = NULL;
    resultat->a->sousArbreGauche = NULL;
    resultat->suivant = NULL;
    for (int i = 0; i < tableau->taille_tableau; i++)
    {
        int taille = tableau->taille[i];
        int valeur = tableau->valeur[i];
        ajouter_RRSS(resultat, calculer_RRSS(taille, valeur));
    }
    return resultat;
}

extern int calculer_taille_liste(liste_arbres* liste)
{
    int taille = 0;
    liste_arbres* temp = liste;
    while (temp!=NULL)
    {
        taille++;
        temp = temp->suivant;
    }
    return taille;
}

extern liste_arbres* fusionne(liste_arbres* liste)
{
    liste_arbres* petit1;
    liste_arbres* petit2;
    liste_arbres* prec_petit1; // Précédent de petit 1
    // Pendant tout la fonction nb_occurrences(petit1) <= nb_occurrences(petit2)
    if (liste->a->frequences > liste->suivant->a->frequences)
    {
        petit1 = liste->suivant;
        petit2 = liste;
        prec_petit1 = liste;
    }
    else
    {
        petit1 = liste;
        petit2 = liste->suivant;
        prec_petit1 = NULL;
    }
    liste_arbres* temp = liste->suivant;
    while (temp->suivant!=NULL)
    {
        if (temp->suivant->a->frequences < petit1->a->frequences)
        {
            petit2 = petit1;
            petit1 = temp->suivant;
            prec_petit1 = temp;
        }
        else if (temp->suivant->a->frequences < petit2->a->frequences)
        {
            petit2 = temp->suivant;
        }
        temp = temp->suivant;
    }
    arbre* nouveau = (arbre*)malloc(sizeof(arbre));
    nouveau->sousArbreDroite = petit2->a;
    nouveau->sousArbreGauche = petit1->a;
    nouveau->vide = true;
    nouveau->frequences = petit1->a->frequences + petit2->a->frequences;
    petit2->a = nouveau;
    if (prec_petit1 == NULL)
    {
        *liste = *(liste)->suivant;
    }
    else
    {
        prec_petit1->suivant = petit1->suivant;
    }
    return liste;
}

extern arbre* genererArbreHuffmanAC(ac* tableau)
{
    liste_arbres* listeArbres = obtenir_liste_des_occurences_ac(tableau);
    int taille = calculer_taille_liste(listeArbres);
    while (taille > 1)
    {
        fusionne(listeArbres);
        taille = calculer_taille_liste(listeArbres);
    }
    return listeArbres->a;
}

extern void afficher_liste_occurrences(liste_arbres* liste)
{
    liste_arbres* temp = liste;
    while (temp!=NULL)
    {
        printf("0x%X%d\toccurrences:%d\n\n", temp->a->etiquette->runlenght,
            temp->a->etiquette->size, temp->a->frequences);
        temp = temp->suivant;
    }
}

extern void afficher_arbre(arbre* a, int nombre)
{
    if (a == NULL)
    {
        printf("VIDE");
    }
    else
    {
        printf("Noeud de profondeur %d:\n\n", nombre);
        printf("Etiquette:");
        if (a->vide)
        {
            printf("rien\n");
        }
        else
        {
            printf("0x%X%d\n", a->etiquette->runlenght, a->etiquette->size);
        }
        
        printf("Sous_arbre_gauche:");
        afficher_arbre(a->sousArbreGauche, nombre + 1);
        printf("\n");
        printf("Sous_arbre_droite:");
        afficher_arbre(a->sousArbreDroite, nombre + 1);
        printf("\n\nFin Noeud de profondeur%d", nombre);
    }
}

extern int profondeur_arbre(arbre* a)
{
    if (a == NULL)
    {
        return 0;
    }
    int gauche = profondeur_arbre(a->sousArbreGauche);
    int droite = profondeur_arbre(a->sousArbreDroite);
    return (gauche > droite ? gauche : droite) + 1;
}

void remplir_code_huffman(arbre* a, char* code, int profondeur, huffman_liste* liste)
{
    if (a == NULL)
    {
        return;
    }
    if (!a->vide)
    {
        // Ajouter le code à la liste
        huffman_liste* nouveau = (huffman_liste*)malloc(sizeof(huffman_liste));
        nouveau->etiquette = a->etiquette;
        nouveau->code = (char*)malloc(sizeof(char) * profondeur);
        nouveau->taille = profondeur;
        for (int i = 0; i < profondeur; i++)
        {
            nouveau->code[i] = code[i];
        }
        nouveau->code[profondeur] = '\0';
        if (liste->etiquette == NULL)
        {
            liste->etiquette = nouveau->etiquette;
            liste->code = nouveau->code;
            liste->suivant = NULL;
        }
        else
        {
            huffman_liste* temp = liste;
            while (temp->suivant != NULL)
            {
                temp = temp->suivant;
            }
            temp->suivant = nouveau;
            nouveau->suivant = NULL;
        }
        return;
        
    }
    code[profondeur] = '0';
    remplir_code_huffman(a->sousArbreGauche, code, profondeur + 1, liste);
    code[profondeur] = '1';
    remplir_code_huffman(a->sousArbreDroite, code, profondeur + 1, liste);
}

extern huffman_liste* creer_huffman_liste(arbre* arbre_huffman)
{
    huffman_liste* arbre_liste = (huffman_liste*)malloc(sizeof(huffman_liste));
    arbre_liste->etiquette = NULL;
    int profondeur = profondeur_arbre(arbre_huffman);
    arbre_liste->code = NULL;
    char* code = (char*)malloc(sizeof(char) * (profondeur + 1));
    code[profondeur] = '\0';
    arbre_liste->suivant = NULL;
    remplir_code_huffman(arbre_huffman, code, 0, arbre_liste);
    free(code);
    return arbre_liste;
}

char* trouver_code(huffman_liste* liste, RRSS* encoder, int* taille)
{
    huffman_liste* temp = liste;
    while (temp != NULL)
    {
        if (comparer_RRSS(temp->etiquette, encoder))
        {
            *taille = temp->taille;
            return temp->code; // Trouver le code
        }
        temp = temp->suivant;
    }
    return NULL; // Non trouver
}

void encoder_sequence(char* sequence, int taille, FILE* fichier)
{
    unsigned char buffer = 0;
    int bit_count = 0;

    for (int i = 0; i < taille; i++) {
        buffer = (buffer << 1) | (sequence[i] == '1' ? 1 : 0);
        bit_count++;
        if (bit_count == 8) {
            fputc(buffer, fichier);
            buffer = 0;
            bit_count = 0;
        }
    }
    // Écrire les bits restants (compléter à gauche)
    if (bit_count > 0) {
        buffer <<= (8 - bit_count);
        fputc(buffer, fichier);
    }
}

extern void enregistrer_fichier_compresser(ac* tableau, const char* nom_fichier)
{
    FILE* fichier = fopen(nom_fichier, "wb");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    huffman_liste* liste_huffman = creer_huffman_liste(genererArbreHuffmanAC(tableau));
    if (liste_huffman == NULL)
    {
        printf("Erreur lors de la création de la liste Huffman.\n");
        fclose(fichier);
        return;
    }
    for (int i = 0; i < tableau->taille_tableau; i++)
    {
        int taille;
        char* code = trouver_code(liste_huffman, calculer_RRSS(tableau->taille[i], tableau->valeur[i]), &taille);
        if (code == NULL)
        {            
            printf("Erreur: Code Huffman non trouvé pour la valeur %d avec taille %d.\n", tableau->valeur[i], tableau->taille[i]);
            fclose(fichier);
            return;
        }
        encoder_sequence(code, taille, fichier);
    }
    
    fclose(fichier);
}
#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include <math.h>
#include<assert.h>

#define PI 3.14159265358979323846
// Ordre zigzag standard JPEG pour une matrice 8x8
int zigzag_order[64][2] = {
    {0,0},{0,1},{1,0},{2,0},{1,1},{0,2},{0,3},{1,2},
    {2,1},{3,0},{4,0},{3,1},{2,2},{1,3},{0,4},{0,5},
    {1,4},{2,3},{3,2},{4,1},{5,0},{6,0},{5,1},{4,2},
    {3,3},{2,4},{1,5},{0,6},{0,7},{1,6},{2,5},{3,4},
    {4,3},{5,2},{6,1},{7,0},{7,1},{6,2},{5,3},{4,4},
    {3,5},{2,6},{1,7},{2,7},{3,6},{4,5},{5,4},{6,3},
    {7,2},{7,3},{6,4},{5,5},{4,6},{3,7},{4,7},{5,6},
    {6,5},{7,4},{7,5},{6,6},{5,7},{6,7},{7,6},{7,7}
};

double q_lum[8][8] = {
        {16, 11, 10, 16, 24, 40, 51, 61},
        {12, 12, 14, 19, 26, 58, 60, 55},
        {14, 13, 16, 24, 40, 57, 69, 56},
        {14, 17, 22, 29, 51, 87, 80, 62},
        {18, 22, 37, 56, 68,109,103, 77},
        {24, 35, 55, 64, 81,104,113, 92},
        {49, 64, 78, 87,103,121,120,101},
        {72, 92, 95, 98,112,100,103, 99}
    };

double q_chrom[8][8] = {
        {17, 18, 24, 47, 99, 99, 99, 99},
        {18, 21, 26, 66, 99, 99, 99, 99},
        {24, 26, 56, 99, 99, 99, 99, 99},
        {47, 66, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99},
        {99, 99, 99, 99, 99, 99, 99, 99}
    };
static inline double alpha(int u) {
    return (u == 0) ? (1.0 / sqrt(2)) : 1.0;
}

Matrice8x8 calculer_DCT(const Matrice8x8 *entree) {
    Matrice8x8 sortie;

    for (int u = 0; u < 8; ++u) {
        for (int v = 0; v < 8; ++v) {
            double somme = 0.0;

            for (int x = 0; x < 8; ++x) {
                for (int y = 0; y < 8; ++y) {
                    double cosx = cos((PI / 8.0) * (x + 0.5) * u);
                    double cosy = cos((PI / 8.0) * (y + 0.5) * v);
                    somme += entree->data[x][y] * cosx * cosy;
                }
            }
            sortie.data[u][v] = 0.25 * alpha(u) * alpha(v) * somme;
        }
    }
    return sortie;
}

// Fonction pour initialiser la matrice avec une valeur
matriceComposant* init_matrice(YCbCrImage* image, Composant composant)
{
    matriceComposant* image_matrice = (matriceComposant*)malloc(sizeof(matriceComposant));
    if (image->width % 8 == 0)
    {
        image_matrice->colonnes = image->width / 8;
    }
    else
    {
        image_matrice->colonnes = (image->width / 8) + 1;
    }
    if (image->height % 8 == 0)
    {
        image_matrice->lignes = image->height / 8;
    }
    else
    {
        image_matrice->lignes = (image->height / 8) + 1;
    }
    image_matrice->matrice = (Matrice8x8**)malloc(sizeof(Matrice8x8*) * image_matrice->lignes);
    for (int i = 0; i < image_matrice->lignes; i++)
    {
        image_matrice->matrice[i] = (Matrice8x8*)malloc(sizeof(Matrice8x8) * image_matrice->colonnes);
        for (int l = 8*i; l < 8*(i+1); l++)
        {
            for (int j = 0; j < image_matrice->colonnes; j++)
            {
                for (int k = 8 * j; k < 8 * (j+1); k++)
                {
                    assert(l - 8*i >= 0 && l - 8*i < 8);
                    assert(k - 8*j >= 0 && k - 8*j < 8);
                    if (k < image->width && l < image->height)
                    {
                        switch (composant)
                        {
                        case COMPOSANT_Y:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = image->pixels[l][k].y;
                            break;
                        case COMPOSANT_CB:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = image->pixels[l][k].Cb;
                            break;
                        default:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = image->pixels[l][k].Cr;
                            break;
                        }
                    }
                    else
                    {
                        switch (composant)
                        {
                        case COMPOSANT_Y:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = 0;
                            break;
                        case COMPOSANT_CB:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = 0;
                            break;
                        default:
                            image_matrice->matrice[i][j].data[l - 8*i][k - 8*j] = 0;
                            break;
                        }
                    }
                }             
            }
        } 
    }
    return image_matrice;
}

int mesurer_largeur(matriceComposant* image)
{
    int width = (image->colonnes - 1) * 8;
    for (int i = 0; i < 8; i++)
    {
        if(image->matrice[0][image->colonnes-1].data[0][i] != 0)
        {
            width++;
        }
        else
        {
            return width;
        }
        
    }
    return width;
}

int mesurer_hauteur(matriceComposant* image)
{
    int height = (image->lignes - 1) * 8;
    for (int i = 0; i < 8; i++)
    {
        if(image->matrice[image->lignes - 1][0].data[i][0] != 0)
        {
            height++;
        }
        else
        {
            return height;
        }
    }
    return height;
}

YCbCrImage* image_YCbCr_from_Image_matrice(matriceComposant* y, matriceComposant* Cb, matriceComposant* Cr)
{
    YCbCrImage* image_resultant = (YCbCrImage*)malloc(sizeof(YCbCrImage));
    image_resultant->width = mesurer_largeur(y);
    image_resultant->height = mesurer_hauteur(y);
    image_resultant->pixels = (YCbCr**)malloc(sizeof(YCbCr*) * image_resultant->height);
    for (int i = 0; i < image_resultant->height; i++)
    {
        int block_hauteur = i/8;
        image_resultant->pixels[i] = (YCbCr*)malloc(sizeof(YCbCr) * image_resultant->width);
        for (int j = 0; j < image_resultant->width; j++)
        {
            int block_largeur = j/8;
            image_resultant->pixels[i][j].y = y->matrice[block_hauteur][block_largeur].data[i%8][j%8];
            image_resultant->pixels[i][j].Cb = Cb->matrice[block_hauteur][block_largeur].data[i%8][j%8];
            image_resultant->pixels[i][j].Cr = Cr->matrice[block_hauteur][block_largeur].data[i%8][j%8];
        }
    }
    return image_resultant;
}

extern matriceQuantifier* quantification(matriceComposant* composant, Matrice8x8* matriceQuantification)
{
    // === VÉRIFICATIONS CRITIQUES AVANT TOUTE ALLOCATION ===
    if (!composant) {
        fprintf(stderr, "Erreur : 'composant' est NULL\n");
        return NULL;
    }
    if (!matriceQuantification) {
        fprintf(stderr, "Erreur : 'matriceQuantification' est NULL\n");
        return NULL;
    }
    if (!composant->matrice) {
        fprintf(stderr, "Erreur : 'composant->matrice' est NULL\n");
        return NULL;
    }
    printf("Vérifications des entrées passées\n");
    matriceQuantifier* sortie = (matriceQuantifier*)malloc(sizeof(matriceQuantifier));
    if (!sortie)
    {
        printf("Erreur dans l'allocation de mémoire\n");
        return NULL;
    }
    printf("Réussire la prémiere allocation\n");
    sortie->colonnes = composant->colonnes;
    sortie->lignes = composant->lignes;
    sortie->matrices = (Matrice8x8Entier**)malloc(sizeof(Matrice8x8Entier*)*sortie->lignes);
    for (int i = 0; i < composant->lignes; i++)
    {
        sortie->matrices[i] = (Matrice8x8Entier*)malloc(sizeof(Matrice8x8Entier)*sortie->colonnes);
        for (int j = 0; j < composant->colonnes; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
                    sortie->matrices[i][j].data[k][l]= (int)(composant->matrice[i][j].data[k][l] / matriceQuantification->data[k][l]);
                }
            }
        }
    }
    return sortie;
}

extern int* parcours_zigzag(Matrice8x8Entier matrice)
{
    int* parcours = (int*)malloc(sizeof(int)*64);
    for (int i = 0; i < 64; i++)
    {
        int x = zigzag_order[i][0];
        int y = zigzag_order[i][1];
        parcours[i] = matrice.data[x][y];
    }
    return parcours;
}

extern Matrice8x8* obtenirMatriceQuantificationLum(int q)
{
    Matrice8x8* resultat = (Matrice8x8*)malloc(sizeof(Matrice8x8));
    if (q == 50)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                resultat->data[i][j] = q_lum[i][j];
            }
            
        }
    }
    if (q<50)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                resultat->data[i][j] = ((100-q)/50) * q_lum[i][j];
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                resultat->data[i][j] = (50/q) * q_lum[i][j];
            }
        }
    }
    return resultat;
}

extern void afficher_matrice_composant(matriceComposant* matrice)
{
    for (int i = 0; i < matrice->lignes; i++)
    {
        for (int j = 0; j < matrice->colonnes; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                printf("[");
                for (int l = 0; l < 8; l++)
                {
                    printf("%f,", matrice->matrice[j][k].data[k][l]);
                }
                printf("]\n");
            }
            
        }
        
    }
    
}

extern void afficher_matrice_8x8(Matrice8x8* matrice)
{
    for (int i = 0; i < 8; i++)
    {
        printf("[");
        for (int j = 0; j < 8; j++)
        {
            printf("%f,", matrice->data[i][j]);
        }
        printf("]\n");
    }
    
}
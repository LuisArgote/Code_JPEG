#include "jpeg.h"
#include<stdlib.h>

extern jpeg* compreser_image(imageRGB* image, int q)
{
    if (q < 1 || q > 100)
    {
        printf("Erreur, le facteur de qualite doit être compris entre 0 et 100\n");
    }
    
    jpeg* resultat = (jpeg*)malloc(sizeof(jpeg));
    YCbCrImage* img = RGBtoYCbCr(image);
    matriceComposant* ComposantY = init_matrice(img, COMPOSANT_Y);
    matriceComposant* ComposantCb = init_matrice(img, COMPOSANT_CB);
    matriceComposant* ComposantCr = init_matrice(img, COMPOSANT_CR);
    resultat->matriceDeQuantificationY = obtenirMatriceQuantificationLum(q); 
    resultat->matriceDeQuantificationChorminence = obtenirMatriceQuantificationChrom(q);
    matriceComposant* dct_y = convertir_composant_en_dct(ComposantY);
    matriceComposant* dct_cb = convertir_composant_en_dct(ComposantCb);
    matriceComposant* dct_Cr = convertir_composant_en_dct(ComposantCr);
    resultat->ComposantY = quantification(dct_y, resultat->matriceDeQuantificationY);
    resultat->ComposantCb = quantification(dct_cb, resultat->matriceDeQuantificationChorminence);
    resultat->ComposantCr = quantification(dct_Cr, resultat->matriceDeQuantificationChorminence);
    free_ycbcr_image(img);
    free_matriceComposant(ComposantY);
    free_matriceComposant(ComposantCb);
    free_matriceComposant(ComposantCr);
    free_matriceComposant(dct_y);
    free_matriceComposant(dct_cb);
    free_matriceComposant(dct_Cr);
    return resultat;
}

extern imageRGB* decompression_jpeg(jpeg* image)
{
    if (!image) {
        printf("Erreur : l'image JPEG est NULL.\n");
        return NULL;
    }
    if (!image->ComposantY || !image->ComposantCb || !image->ComposantCr) {
        printf("Erreur : les composants de l'image JPEG sont NULL.\n");
        return NULL;
    }
    if (!image->matriceDeQuantificationY || !image->matriceDeQuantificationChorminence) {
        printf("Erreur : les matrices de quantification sont NULL.\n");
        return NULL;
    }
    matriceComposant* ComposantYdequantifier = dequantification(image->ComposantY, image->matriceDeQuantificationY);
    matriceComposant* ComposantCbdequantifier = dequantification(image->ComposantCb, image->matriceDeQuantificationChorminence);
    matriceComposant* ComposantCrdequantifier = dequantification(image->ComposantCr, image->matriceDeQuantificationChorminence);
    matriceComposant* Ydecompreser = inverser_dct_matrice(ComposantYdequantifier);
    matriceComposant* Cbdecompreser = inverser_dct_matrice(ComposantCbdequantifier);
    matriceComposant* Crdecompreser = inverser_dct_matrice(ComposantCrdequantifier);
    YCbCrImage* img_decompresser = image_from_composant(Ydecompreser, Cbdecompreser, Crdecompreser);
    imageRGB* image_final = YCbCrtoRGB(img_decompresser);
    free_ycbcr_image(img_decompresser);
    free_matriceComposant(Ydecompreser);
    free_matriceComposant(Cbdecompreser);
    free_matriceComposant(Crdecompreser);
    free_matriceComposant(ComposantYdequantifier);
    free_matriceComposant(ComposantCbdequantifier);
    free_matriceComposant(ComposantCrdequantifier);
    return image_final;
}

extern void free_jpeg(jpeg* image)
{
    if (!image) {
        return; // Rien à libérer si l'image est NULL
    }
    free_matriceQuantifier(image->ComposantY);
    free_matriceQuantifier(image->ComposantCb);
    free_matriceQuantifier(image->ComposantCr);
    free_matrice_8x8(image->matriceDeQuantificationY);
    free_matrice_8x8(image->matriceDeQuantificationChorminence);
    free(image);
}
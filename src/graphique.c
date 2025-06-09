#include "graphique.h"
#include <gtk-4.0/gtk/gtk.h>
#include<cairo.h>
#include<graphene-1.0/graphene-gobject.h>
#include"jpeg.h"

// Définition de draw_func
static void draw_func(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_arc(cr, width / 2.0, height / 2.0, MIN(width, height) / 4.0, 0, 2 * G_PI);
    cairo_fill(cr);
}

static void draw_callback(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer user_data)
{
    imageRGB *image = (imageRGB *)user_data;
    
    // Créer une surface Cairo pour l'image
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, image->width, image->height);
    unsigned char *surface_data = cairo_image_surface_get_data(surface);
    int stride = cairo_image_surface_get_stride(surface);
    
    // Remplir la surface avec les données RGB
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            // Convertir les double [0,1] en unsigned char [0,255]
            unsigned char r = (unsigned char)(image->image[y][x].red);
            unsigned char g = (unsigned char)(image->image[y][x].green);
            unsigned char b = (unsigned char)(image->image[y][x].blue);
            
            // Position dans le buffer de la surface
            unsigned char *pixel = surface_data + y * stride + x * 4;
            
            // Format ARGB (Cairo utilise l'ordre pré-multiplié)
            pixel[0] = b; // Blue
            pixel[1] = g; // Green
            pixel[2] = r; // Red
            pixel[3] = 255; // Alpha (opaque)
        }
    }
    
    // Marquer la surface comme modifiée
    cairo_surface_mark_dirty(surface);
    
    // Dessiner la surface sur la zone de dessin
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    
    // Libérer la surface
    cairo_surface_destroy(surface);
}

static void activate_rgb(GtkApplication *app, gpointer user_data)
{
    // Importer l'image
    imageRGB* image = (imageRGB*)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Affichage d'image RGB");
    gtk_window_set_default_size(GTK_WINDOW(window), image->width, image->height);
    
    // Créer une zone de dessin
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);
    
    
    // Connecter le callback de dessin
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), 
                                 draw_callback, 
                                 image, 
                                 NULL);
    
    gtk_window_present(GTK_WINDOW(window));
}


// Fonction pour convertir une valeur double [0,1] en unsigned char [0,255]
static unsigned char double_to_uchar(double value) 
{
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (unsigned char)value;
}


// Fonction utilitaire générique
static void draw_colored_component(cairo_t *cr, unsigned char *component, 
    int width, int height) 
{
    // Créer surface A8
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);
    unsigned char *sdata = cairo_image_surface_get_data(surface);
    int stride = cairo_image_surface_get_stride(surface);

    // Copier les données
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sdata[i * stride + j] = component[i * stride + j];
        }
        
    }
    
    cairo_surface_mark_dirty(surface);

    // Appliquer la couleur
    cairo_mask_surface(cr, surface, 0, 0);

    cairo_surface_destroy(surface);
}

static void draw_y_component(GtkDrawingArea *drawing_area, cairo_t *cr, 
     int width, int height, gpointer data) {
    YCbCrImage *app_data = (YCbCrImage *)data;
    unsigned char *Y = malloc(app_data->width * app_data->height);

    for (int i = 0; i < app_data->height; i++)
    {
        for (int j = 0; j < app_data->width; j++)
        {
            Y[i * app_data->width + j] = 255 - double_to_uchar(app_data->pixels[i][j].y);
        }
        
    }
    draw_colored_component(cr, Y, app_data->width, app_data->height);
    free(Y);
}

static void draw_cb_component(GtkDrawingArea *drawing_area, cairo_t *cr, 
                                int width, int height, gpointer data) 
{
    YCbCrImage *app_data = (YCbCrImage *)data;
    unsigned char *Cb = malloc(app_data->width * app_data->height);

    for (int i = 0; i < app_data->height; i++)
    {
        for (int j = 0; j < app_data->width; j++)
        {
            Cb[i * app_data->width + j] = 255 - double_to_uchar(app_data->pixels[i][j].Cb);
        }    
    }
    

    draw_colored_component(cr, Cb, app_data->width, app_data->height);
    free(Cb);
}

static void draw_cr_component(GtkDrawingArea *drawing_area, cairo_t *cr, 
                                int width, int height, gpointer data)
{
    YCbCrImage *app_data = (YCbCrImage *)data;
    unsigned char *Cr = malloc(app_data->width * app_data->height);

    for (int i = 0; i < app_data->height; i++)
    {
        for (int j = 0; j < app_data->width; j++)
        {
            // Nous avons inverser le noir et le blanc pour que le rouge soit visible
            Cr[i * app_data->width + j] = 255 - double_to_uchar(app_data->pixels[i][j].Cr);
        }   
    }

    draw_colored_component(cr, Cr, app_data->width, app_data->height);
    free(Cr);
}

static void save_drawing_area_to_png(GtkDrawingArea *area, const char *filename) {
    int width = gtk_widget_get_width(GTK_WIDGET(area));
    int height = gtk_widget_get_height(GTK_WIDGET(area));

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    draw_func(area, cr, width, height, NULL);

    cairo_surface_write_to_png(surface, filename);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    g_print("Saved DrawingArea to %s\n", filename);
}


static GtkWidget* create_component_widget(const char *label_text, GtkDrawingAreaDrawFunc draw_func, gpointer user_data) {
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    YCbCrImage *app_data = (YCbCrImage*)user_data;
    GtkWidget *label = gtk_label_new(label_text);
    gtk_box_append(GTK_BOX(vbox), label);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_func, user_data, NULL);

    // Fixer une taille raisonnable
    gtk_widget_set_size_request(drawing_area, app_data->width, app_data->height);

    gtk_box_append(GTK_BOX(vbox), drawing_area);

    return vbox;
}

static void activateYCbCr(GtkApplication *app, gpointer user_data) 
{
    YCbCrImage *app_data = (YCbCrImage *)user_data;

    // Créer la fenêtre principale
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Visualisation YCbCr");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 400);

    // Conteneur principal horizontal
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), hbox);

    // Créer les trois widgets composants
    GtkWidget *y_widget = create_component_widget("Composante Y (Luminance)", draw_y_component, app_data);
    GtkWidget *cb_widget = create_component_widget("Composante Cb (Chrominance Bleue)", draw_cb_component, app_data);
    GtkWidget *cr_widget = create_component_widget("Composante Cr (Chrominance Rouge)", draw_cr_component, app_data);

    // Ajouter au conteneur horizontal
    gtk_box_append(GTK_BOX(hbox), y_widget);
    gtk_box_append(GTK_BOX(hbox), cb_widget);
    gtk_box_append(GTK_BOX(hbox), cr_widget);

    gtk_window_present(GTK_WINDOW(window));
}


extern void afficher_YCbCr(YCbCrImage* image)
{
    GtkApplication *app = gtk_application_new("org.gtk.rgbviewer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activateYCbCr), image);
    g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref(app);   
}

extern void afficher_rgb(imageRGB* image)
{
    GtkApplication *app = gtk_application_new("org.gtk.rgbviewer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate_rgb), image);
    g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref(app);   
}


static void activate_compression(GtkApplication *app, gpointer user_data) {
    CompressionData *data = (CompressionData *)user_data;
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Difference entre l'image originale et l'image compressée");
    gtk_window_set_default_size(GTK_WINDOW(window), data->image->width * 2 + 60, data->image->height + 100);

    // Créer un conteneur vertical
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);


    // Créer l'étiquette initiale
    char label_text[50];
    snprintf(label_text, sizeof(label_text), "Q = %d", data->q);
    GtkWidget *label = gtk_label_new(label_text);

    // Creer un box horizontal pour les images
    GtkWidget *image_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);

    // Créer une zone de dessin
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_box_append(GTK_BOX(image_box), drawing_area);

    
    
    
    // Connecter le callback de dessin
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), 
                                 draw_callback, 
                                 data->image, 
                                 NULL);
    
    imageRGB *compressed_image = decompression_jpeg(compreser_image(data->image, data->q));
    GtkWidget *compressed_drawing_area = gtk_drawing_area_new();
    gtk_box_append(GTK_BOX(image_box), compressed_drawing_area);
    // Connecter le callback de dessin pour l'image compressée
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(compressed_drawing_area), 
                                 draw_callback, 
                                 compressed_image, 
                                 NULL);

    gtk_widget_set_size_request(drawing_area, data->image->width, data->image->height);
    gtk_widget_set_size_request(compressed_drawing_area, compressed_image->width, compressed_image->height);
    // Ajouter le box d'images au box principal
    gtk_box_append(GTK_BOX(box), image_box);
    
    // Ajouter les widgets
    gtk_box_append(GTK_BOX(box), label);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

extern void afficher_compression(imageRGB* image, int q) 
{
    CompressionData* data = (CompressionData*)malloc(sizeof(CompressionData));
    data->image = image;
    data->q = q;
    GtkApplication *app = gtk_application_new("org.example.slider", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate_compression), data);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}

static void differentes_compression(GtkApplication *app, gpointer user_data) 
{
    imageRGB *image = (imageRGB *)user_data;
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Différences de compression");
    gtk_window_set_default_size(GTK_WINDOW(window), image->width * 2 + 60, image->height + 100);

    // Créer un conteneur vertical
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(box, 1);
    gtk_widget_set_margin_bottom(box, 1);
    gtk_widget_set_margin_start(box, 1);
    gtk_widget_set_margin_end(box, 1);

    // Creer un box pour Q = 10 et Q= 30
    GtkWidget *q10et30_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(box), q10et30_box);
    // Creer un conteneur vertical pour l'image q = 10
    GtkWidget *q10_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(q10et30_box), q10_box);
    // Créer l'étiquette pour q = 10
    GtkWidget *label_q10 = gtk_label_new("Q = 10");
    gtk_box_append(GTK_BOX(q10_box), label_q10);

    // Créer l'image pour q = 10
    GtkWidget *drawing_area_q10 = gtk_drawing_area_new();
    imageRGB *image_q10 = decompression_jpeg(compreser_image(image, 10));
    gtk_widget_set_size_request(drawing_area_q10, image_q10->width, image_q10->height);
    gtk_box_append(GTK_BOX(q10_box), drawing_area_q10);
    // Connecter le callback de dessin pour l'image q = 10
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area_q10), 
                                 draw_callback, 
                                 image_q10, 
                                 NULL);
    // Pour Q = 30
    GtkWidget *q30_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(q10et30_box), q30_box);
    GtkWidget *label_q30 = gtk_label_new("Q = 30");
    gtk_box_append(GTK_BOX(q30_box), label_q30);
    GtkWidget *drawing_area_q30 = gtk_drawing_area_new();
    imageRGB *image_q30 = decompression_jpeg(compreser_image(image, 30));
    gtk_widget_set_size_request(drawing_area_q30, image_q30->width, image_q30->height);
    gtk_box_append(GTK_BOX(q30_box), drawing_area_q30);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area_q30), 
                                   draw_callback, 
                                   image_q30, 
                                   NULL);

    // Creer une box pour Q = 50 et Q = 70
    GtkWidget *q50et70_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(box), q50et70_box);
    // Pour Q = 50
    GtkWidget *q50_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(q50et70_box), q50_box);
    GtkWidget *label_q50 = gtk_label_new("Q = 50");
    gtk_box_append(GTK_BOX(q50_box), label_q50);
    GtkWidget *drawing_area_q50 = gtk_drawing_area_new();
    imageRGB *image_q50 = decompression_jpeg(compreser_image(image, 50));
    gtk_widget_set_size_request(drawing_area_q50, image_q50->width, image_q50->height);
    gtk_box_append(GTK_BOX(q50_box), drawing_area_q50);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area_q50), 
                                   draw_callback, 
                                   image_q50, 
                                   NULL);

    // Pour Q = 70
    GtkWidget *q70_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_box_append(GTK_BOX(q50et70_box), q70_box);
    GtkWidget *label_q70 = gtk_label_new("Q = 70");
    gtk_box_append(GTK_BOX(q70_box), label_q70);
    GtkWidget *drawing_area_q70 = gtk_drawing_area_new();
    imageRGB *image_q70 = decompression_jpeg(compreser_image(image, 70));
    gtk_widget_set_size_request(drawing_area_q70, image_q70->width, image_q70->height);
    gtk_box_append(GTK_BOX(q70_box), drawing_area_q70);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area_q70), 
                                   draw_callback, 
                                   image_q70, 
                                   NULL);

    // Ajouter le box principal à la fenêtre
    gtk_window_set_child(GTK_WINDOW(window), box);
    
    // Afficher la fenêtre
    gtk_window_present(GTK_WINDOW(window));
}

extern void afficher_differences_qualite(imageRGB *image) 
{
    GtkApplication *app = gtk_application_new("org.example.slider", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(differentes_compression), image);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}
#include "graphique.h"
#include <gtk-4.0/gtk/gtk.h>
#include <cairo/cairo.h>

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
    value = value * 255.0;
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
            Y[i * app_data->width + j] = (unsigned char)(app_data->pixels[i][j].y);
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
            Cb[i * app_data->width + j] = (unsigned char)(app_data->pixels[i][j].Cb);
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
            Cr[i * app_data->width + j] = (unsigned char)(app_data->pixels[i][j].Cr);
        }   
    }

    draw_colored_component(cr, Cr, app_data->width, app_data->height);
    free(Cr);
}

static void activateYCbCr(GtkApplication *app, gpointer user_data) 
{
    YCbCrImage *app_data = (YCbCrImage *)user_data;

    // Créer la fenêtre principale
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Visualisation YCbCr");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Créer un conteneur notebook (onglets)
    GtkWidget *notebook = gtk_notebook_new();
    gtk_window_set_child(GTK_WINDOW(window), notebook);

    // Onglet pour Y
    GtkWidget *y_label = gtk_label_new("Composante Y (Luminance)");
    GtkWidget *y_drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(y_drawing_area), 
                draw_y_component, app_data, NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), y_drawing_area, y_label);

    // Onglet pour Cb
    GtkWidget *cb_label = gtk_label_new("Composante Cb (Chrominance Bleue)");
    GtkWidget *cb_drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(cb_drawing_area), 
                draw_cb_component, app_data, NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), cb_drawing_area, cb_label);

    // Onglet pour Cr
    GtkWidget *cr_label = gtk_label_new("Composante Cr (Chrominance Rouge)");
    GtkWidget *cr_drawing_area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(cr_drawing_area), 
                draw_cr_component, app_data, NULL);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), cr_drawing_area, cr_label);

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
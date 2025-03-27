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

static void activate(GtkApplication *app, gpointer user_data)
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

extern void afficher_rgb(imageRGB* image)
{
    GtkApplication *app = gtk_application_new("org.gtk.rgbviewer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), image);
    int status = g_application_run (G_APPLICATION (app), 0, NULL);
    g_object_unref(app);
}
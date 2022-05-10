#ifndef F_GRAPH_H
#define F_GRAPH_H

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <string.h>
#include "calculator.h"

#define HEIGHT 600.
#define WIDTH 600.
#define X_START 10.
#define X_END (WIDTH-10.)
#define X_STEP ((X_END-X_START)/100.)
#define Y_START (HEIGHT - 10.)
#define Y_END 10.
#define Y_STEP ((Y_START-Y_END)/100.)
#define LINE_THICKNESS 1.

void create_graph(GtkWidget *p_widget, gpointer data);

#endif // !F_GRAPH_H
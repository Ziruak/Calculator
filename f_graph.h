#ifndef F_GRAPH_H
#define F_GRAPH_H

#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <string.h>
#include "calculator.h"

#define HEIGHT 650.
#define WIDTH 650.
#define STEP_COUNT 20.
#define X_START 20.
#define X_END (WIDTH-20.)
#define X_STEP ((X_END-X_START)/STEP_COUNT)
#define Y_START (HEIGHT - 20.)
#define Y_END 20.
#define Y_STEP ((Y_START-Y_END)/STEP_COUNT)
#define LINE_THICKNESS 1.

void create_graph(GtkWidget *p_widget, gpointer data);
void dimensions_popup(GtkWidget *p_widget, gpointer data);

#endif // !F_GRAPH_H
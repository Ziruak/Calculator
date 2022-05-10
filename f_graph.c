#include "f_graph.h"

static void do_drawing(cairo_t *, GtkWidget*);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{
  do_drawing(cr, (GtkEntry*)user_data);

  return FALSE;
}

void draw_func(cairo_t *cr, GtkWidget *entry) {
    gchar str[256], label_str[256];
   strncpy(label_str,gtk_entry_get_text(GTK_ENTRY(entry)), 255);
   strcat(label_str,"\n");
    // add nan defense;
  for (double i = X_START; i < X_END - X_STEP; i+=X_STEP/10) {
     double xval = (i - X_START)/X_STEP;
     double yval = calculate(label_str, xval);
     double yval_next = calculate(label_str, xval+0.1);

     
      cairo_move_to(cr,i,Y_START - (yval)*Y_STEP);
     cairo_line_to(cr,i+X_STEP/10,Y_START - (yval_next)*Y_STEP);
  }
}

static void do_drawing(cairo_t *cr, GtkWidget* entry)
{
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, LINE_THICKNESS);
   cairo_move_to(cr, X_START, Y_START);
   cairo_line_to(cr, X_END, Y_START);
   cairo_move_to(cr, X_START, Y_START);
   cairo_line_to(cr, X_START, Y_END);

  for (int i = X_START; i < X_END; i+=X_STEP) {
     cairo_move_to(cr,i,Y_START-5);
     cairo_line_to(cr,i,Y_START+5);
  }

  for (int i = Y_END; i < Y_START; i+=Y_STEP) {
     cairo_move_to(cr,X_START-5,i);
     cairo_line_to(cr,X_START+5,i);
  }
  draw_func(cr, entry);

  cairo_stroke(cr);    
}



static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data)
{
    if (event->button == 1) {
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}


void create_graph(GtkWidget *p_widget, gpointer data)
{
  GtkWidget *window;
  GtkWidget *darea;
  GtkWidget *entry = data;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
 
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), entry); 
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_window_close), NULL);  
    
  g_signal_connect(window, "button-press-event", 
      G_CALLBACK(clicked), NULL);
 
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Lines");

  gtk_widget_show_all(window);

}
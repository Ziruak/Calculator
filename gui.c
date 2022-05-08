#include <gtk/gtk.h>
#include<string.h>
#include "calculator.h"

void append_to_label(GtkWidget *widget, GtkLabel* label) {
   gchar str[256];
   strncpy(str,gtk_label_get_label(label),255);
   strncat(str,gtk_button_get_label(GTK_BUTTON(widget)),
   255-strlen(str));
   gtk_label_set_label(label,str);
}

void append_func_to_label(GtkWidget *widget, GtkLabel *label) {
   gchar str[256];
   strncpy(str,gtk_label_get_label(label),255);
   strncat(str,gtk_button_get_label(GTK_BUTTON(widget)),
   255-strlen(str));
   strncat(str,"()", 255-strlen(str));
   gtk_label_set_label(label,str);
}

void delete_sym_from_label(GtkWidget *widget, GtkLabel *label) {
   gchar str[256];
   const gchar *label_str = gtk_label_get_label(label);
   if (strlen(label_str)>0) {
   strncpy(str,label_str, 255);
   str[strlen(label_str)-1] = '\0';
   gtk_label_set_label(label,str);
   }
}

void calculate_expression(GtkWidget *widget, GtkLabel *label) {
   gchar str[256], label_str[256];
   strncpy(label_str,gtk_label_get_label(label), 255);
   strcat(label_str,"\n");
   sprintf(str,"%lf",calculate(label_str,0));
   gtk_label_set_label(label,str);
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkGrid *table;
  gchar *values[30] = {
     "c", "(", ")", "+", "mod", "sqrt",
     "7", "8", "9", "*", "sin", "asin", 
     "4", "5", "6", "-", "cos", "acos",
     "1", "2", "3", "+", "tan", "atan",
     "0", ".", "^", "=", "ln", "log"
  };


  gtk_init(&argc, &argv); // initilizes gtk. Nust be called before other funcs

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // create window
  gtk_window_set_title(GTK_WINDOW(window),"Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window),400,540);
  gtk_window_set_position(GTK_WINDOW(window), GTK_POS_LEFT); // is it even working?
  gtk_container_set_border_width(GTK_CONTAINER(window), 5); // border from insides of the window

  //table = GTK_TABLE(gtk_table_new(6, 6, FALSE));
  table = GTK_GRID(gtk_grid_new());
  gtk_widget_set_hexpand(GTK_WIDGET(table), TRUE);
  gtk_widget_set_vexpand(GTK_WIDGET(table), TRUE);
  //gtk_widget_set_halign(GTK_WIDGET(table),GTK_ALIGN_FILL);
  //gtk_widget_set_valign(GTK_WIDGET(table),GTK_ALIGN_FILL);
  //gtk_table_set_row_spacings(table, 2);
  //gtk_table_set_col_spacings(table, 2);
  GtkWidget *label;
  label = gtk_label_new("");
  gtk_widget_set_hexpand(label, TRUE);
  gtk_widget_set_vexpand(label, TRUE);
  //gtk_table_attach_defaults(table,
   //   label, 0, 6, 0, 1);
   gtk_grid_attach(table, label, 0,0,6,1);
   for (int i = 1, pos = 0; i <6; ++i) {
      for (int j = 0; j < 6; ++j) {
         GtkWidget *button;
         button = gtk_button_new_with_label(values[pos]);
         void (*b_func) (GtkWidget*, GtkLabel*);
         if (pos == 0) b_func = delete_sym_from_label;
         else if (pos == 27) b_func = calculate_expression;
         else if ((pos % 6 == 4 || pos % 6 == 5) && pos != 4) b_func = append_func_to_label;
         else b_func = append_to_label;
         g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(b_func),GTK_LABEL(label));
         gtk_widget_set_hexpand(button, TRUE);
         gtk_widget_set_vexpand(button, TRUE);
         //gtk_table_attach_defaults(table,
          //  button, j, j+1, i, i+1);
          gtk_grid_attach(table, button, j,i,1,1);
         ++pos;
      }
   }

  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(table)); // add table to the window
  
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);  // on x click

  gtk_widget_show_all(window); // show window and all of its children

  gtk_main(); // pass handler to gtk

  return 0;
}
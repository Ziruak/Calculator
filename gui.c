#include <gtk/gtk.h>
#include<string.h>
#include "calculator.h"

void append_str_to_label(GtkLabel *label, const char *str) {
   gchar label_str[256];
    strncpy(label_str,gtk_label_get_label(label),255);
    strncat(label_str,str,255-strlen(label_str));
    gtk_label_set_label(label,label_str);
}

void append_to_label(GtkWidget *widget, GtkLabel* label) {
   append_str_to_label(label, gtk_button_get_label(GTK_BUTTON(widget)));
}

void append_func_to_label(GtkWidget *widget, GtkLabel *label) {
   gchar str[64];
   strncpy(str,gtk_button_get_label(GTK_BUTTON(widget)),63);
   strncat(str,"()", 63-strlen(str));
   append_str_to_label(label, str);
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

void delete_label_str(GtkWidget *widget, GtkLabel *label) {
   gtk_label_set_label(label,"");
}

void calculate_expression(GtkWidget *widget, GtkLabel *label) {
   gchar str[256], label_str[256];
   strncpy(label_str,gtk_label_get_label(label), 255);
   strcat(label_str,"\n");
   sprintf(str,"%lf",calculate(label_str,0));
   gtk_label_set_label(label,str);
}

void keypress_handler (GtkWidget *widget, GdkEventKey *event, GtkLabel *label) {
    if (event->keyval == GDK_KEY_BackSpace){
        delete_sym_from_label(widget, label);
    } else if (event->keyval == GDK_KEY_Delete) {
       delete_label_str(widget, label);
    } else if (strchr("1234567890()*-+^/.",event->keyval)) {
       char str[2] = {event->keyval,'\0'};
       append_str_to_label(label,str);
    } else if (event->keyval == GDK_KEY_equal) {
       calculate_expression(widget,label);
    } else if (event->keyval == GDK_KEY_percent) {
       append_str_to_label(label,"mod");
    }
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkGrid *table;
  gchar *values[31] = { // 6 11 12 17 18
     "c", "Del", "(", ")", "/", "mod", "sqrt",
     "7",  "8",  "9", "*",      "sin", "asin", 
     "4",  "5",  "6", "-",      "cos", "acos",
     "1",  "2",  "3", "+",      "tan", "atan",
     "0",  ".",  "^", "=",       "ln", "log"
  };


  gtk_init(&argc, &argv); // initilizes gtk. Nust be called before other funcs

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // create window
  gtk_window_set_title(GTK_WINDOW(window),"Calculator");
  gtk_window_set_default_size(GTK_WINDOW(window),400,540);
  gtk_window_set_position(GTK_WINDOW(window), GTK_POS_LEFT); // is it even working?
  gtk_container_set_border_width(GTK_CONTAINER(window), 5); // border from insides of the window

   

  table = GTK_GRID(gtk_grid_new());
  gtk_widget_set_hexpand(GTK_WIDGET(table), TRUE);
  gtk_widget_set_vexpand(GTK_WIDGET(table), TRUE);
  GtkWidget *label;
  label = gtk_label_new("");
  gtk_widget_set_hexpand(label, TRUE);
  gtk_widget_set_vexpand(label, TRUE);
   gtk_grid_attach(table, label, 0,0,6,1);
   for (int i = 1, pos = 0; i <6; ++i) {
      for (int j = 0; j < 7; ++j) {
         if (i == 1 || j != 4) {
         GtkWidget *button;
         button = gtk_button_new_with_label(values[pos]);
         void (*b_func) (GtkWidget*, GtkLabel*);
         if (pos == 0) b_func = delete_sym_from_label;
         else if (pos == 1) b_func = delete_label_str;
         else if (pos == 28) b_func = calculate_expression;
         else if ((pos % 6 == 0 || pos % 6 == 5) && pos != 5) b_func = append_func_to_label;
         else b_func = append_to_label;
         g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(b_func),GTK_LABEL(label));
         gtk_widget_set_hexpand(button, TRUE);
         gtk_widget_set_vexpand(button, TRUE);
          gtk_grid_attach(table, button, j,i,1,1);
         ++pos;
         }
      }
   }

  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK); // enable key press handler
  g_signal_connect (G_OBJECT (window), "key_press_event",
        G_CALLBACK (keypress_handler), label);

  gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(table)); // add table to the window
  
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);  // on x click

  gtk_widget_show_all(window); // show window and all of its children

  gtk_main(); // pass handler to gtk

  return 0;
}
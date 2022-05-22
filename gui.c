#include <gtk/gtk.h>
#include<string.h>
#include "calculator.h"
#include "f_graph.h"

struct ent_data {
   GtkEntry *x_entry, *calc_entry;
};

void append_str_to_entry(GtkEntry *entry, const char *str) {
   gchar label_str[256];
    strncpy(label_str,gtk_entry_get_text(entry),255);
    strncat(label_str,str,255-strlen(label_str));
    gtk_entry_set_text(entry,label_str);
}

void append_to_entry(GtkWidget *widget, struct ent_data* entry) {
   append_str_to_entry(entry->calc_entry, gtk_button_get_label(GTK_BUTTON(widget)));
}

void append_func_to_entry(GtkWidget *widget, struct ent_data  *entry) {
   gchar str[64];
   strncpy(str,gtk_button_get_label(GTK_BUTTON(widget)),63);
   strncat(str,"()", 63-strlen(str));
   append_str_to_entry(entry->calc_entry, str);
}

void delete_sym_from_entry(GtkWidget *widget, struct ent_data *entry) {
   gchar str[256];
   const gchar *label_str = gtk_entry_get_text(entry->calc_entry);
   if (strlen(label_str)>0) {
   strncpy(str,label_str, 255);
   str[strlen(label_str)-1] = '\0';
   gtk_entry_set_text(entry->calc_entry,str);
   }
}

void delete_entry_str(GtkWidget *widget, struct ent_data *entry) {
   gtk_entry_set_text(entry->calc_entry,"");
}

void calculate_expression(GtkWidget *widget, struct ent_data *entry) {
   gchar str[256], label_str[256];
   double x = 0;
   if (sscanf(gtk_entry_get_text(entry->x_entry),"%lf",&x) == 1) {
   strncpy(label_str,gtk_entry_get_text(entry->calc_entry), 255);
   strcat(label_str,"\n");
   sprintf(str,"%lf",calculate(label_str,x));
   gtk_entry_set_text(entry->calc_entry,str);
   } else {
      gtk_entry_set_text(entry->x_entry,"Invalid number");
   }
}

void calc_entry_text_handler (GtkEntry    *entry,
                          const gchar *text,
                          gint         length,
                          gint        *position,
                          gpointer     data)
{
  GtkEditable *editable = GTK_EDITABLE(entry);
  int count=0, need_calc = 0;
  gchar *result = g_new (gchar, length);

  for (int i=0; i < length && !need_calc; i++) {
    if (text[i]== '=') need_calc = 1;
    else result[count++] = text[i];
  }
  
  if (count > 0) {
    g_signal_handlers_block_by_func (G_OBJECT (editable),
                                     G_CALLBACK (calc_entry_text_handler),
                                     data);
    gtk_editable_insert_text (editable, result, count, position);
    g_signal_handlers_unblock_by_func (G_OBJECT (editable),
                                       G_CALLBACK (calc_entry_text_handler),
                                       data);
  }
  if (need_calc) {
     calculate_expression(NULL, data);
  }
  g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");

  g_free (result);
}

void main_window_quit(GtkWidget* win, struct ent_data *data) {
   free(data);
   gtk_main_quit();
}

int main(int argc, char *argv[]) {

  GtkWidget *window, *graphbtn, *hbox, *vbox, *xalbel;
  GtkGrid *table;
  GtkWidget *entry, *xentry;
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
  gtk_window_set_position(GTK_WINDOW(window), GTK_POS_LEFT);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

   hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
   
   vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
   gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);

   entry = gtk_entry_new();

   graphbtn = gtk_button_new_with_label("Graph");
   g_signal_connect(G_OBJECT(graphbtn), "clicked",
        G_CALLBACK(dimensions_popup), entry);

   xalbel = gtk_label_new(" X = ");
   xentry = gtk_entry_new();


   //gtk_container_add(GTK_CONTAINER(window),graphbtn);
   gtk_box_pack_start(GTK_BOX(hbox),graphbtn,FALSE,FALSE,0);
   gtk_box_pack_start(GTK_BOX(hbox),xalbel,FALSE,FALSE,0);
   gtk_box_pack_start(GTK_BOX(hbox),xentry,FALSE,FALSE,0);

   gtk_entry_set_max_length(GTK_ENTRY(entry),64);
   gtk_entry_set_text(GTK_ENTRY(xentry),"0");

  table = GTK_GRID(gtk_grid_new());

  
  gtk_box_pack_start(GTK_BOX(vbox),entry,FALSE,TRUE,0);

   struct ent_data *data = (struct ent_data*) malloc(sizeof(struct ent_data));
   data->calc_entry = GTK_ENTRY(entry);
   data->x_entry = GTK_ENTRY(xentry);

   gtk_entry_set_max_length(GTK_ENTRY(entry),255);
  g_signal_connect(entry, "insert_text",
		     G_CALLBACK(calc_entry_text_handler),
		     data);

   for (int i = 0, pos = 0; i <5; ++i) {
      for (int j = 0; j < 7; ++j) {
         if (i == 0 || j != 4) {
         GtkWidget *button;
         button = gtk_button_new_with_label(values[pos]);
         void (*b_func) (GtkWidget*, struct ent_data*);
         if (pos == 0) b_func = delete_sym_from_entry;
         else if (pos == 1) b_func = delete_entry_str;
         else if (pos == 28) b_func = calculate_expression;
         else if ((pos % 6 == 0 || pos % 6 == 5) && pos != 5) b_func = append_func_to_entry;
         else b_func = append_to_entry;
         g_signal_connect(G_OBJECT(button),"clicked",
         G_CALLBACK(b_func),data);
         gtk_widget_set_hexpand(button, TRUE);
         gtk_widget_set_vexpand(button, TRUE);
          gtk_grid_attach(table, button, j,i,1,1);
         ++pos;
         }
      }
   }
   //gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(table)); // add table to the window
   gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(table),TRUE,TRUE,0);

   

  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK); // enable key press handler
      
  g_signal_connect(window, "destroy",
      G_CALLBACK(main_window_quit), data);  // on x click

  gtk_widget_show_all(window); // show window and all of its children

  gtk_main(); // pass handler to gtk

  return 0;
}
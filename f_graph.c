#include "f_graph.h"

struct draw_data {
  const gchar* entry_str;
  double xstart,xend,ystart,yend;
  gboolean is_valid;
};

struct entries {
  GtkWidget *parent_w;
  const gchar* entry_f;
  GtkWidget *entry_xs, *entry_xe, *entry_ys, *entry_ye;
};

void draw_func(cairo_t *cr, struct draw_data *ddata) {
    gchar label_str[256];
   strncpy(label_str,ddata->entry_str, 255);
   strcat(label_str,"\n");
   double xstep = (ddata->xend-ddata->xstart)/STEP_COUNT;
   double ystep = (ddata->yend-ddata->ystart)/STEP_COUNT;
   double step_coef = 0.1;
    // add nan defense;
  for (double i = X_START; i < X_END - X_STEP; i+=X_STEP*step_coef) {
     double xval = ddata->xstart + xstep*(i - X_START)/X_STEP;
     double xval_next = xval + xstep*step_coef;
     double yval = calculate(label_str, xval);
     double yval_next = calculate(label_str, xval_next);

     if (yval <= ddata->yend && yval >= ddata->ystart 
     && yval_next <= ddata->yend && yval_next >= ddata->ystart) {
      cairo_move_to(cr,i,Y_END + ((ddata->yend - yval)/ystep)*Y_STEP);
      cairo_line_to(cr,X_START+(xval_next-ddata->xstart)/xstep*X_STEP,Y_END + ((ddata->yend - yval_next)/ystep)*Y_STEP);}
  }
}

static void do_drawing(cairo_t *cr, struct draw_data *ddata)
{
  char str[64];
  double xstep = (ddata->xend-ddata->xstart)/STEP_COUNT;
   double ystep = (ddata->yend-ddata->ystart)/STEP_COUNT;

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, LINE_THICKNESS);
  cairo_select_font_face(cr,"Times New Roman", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, 8);
   cairo_move_to(cr, X_START, Y_START);
   cairo_line_to(cr, X_END, Y_START);
   cairo_move_to(cr,X_START, Y_START + 15);
   sprintf(str, "%g", ddata->xstart);
   cairo_show_text(cr, str);
   
   cairo_move_to(cr, X_START, Y_START);
   cairo_line_to(cr, X_START, Y_END);

  for (int i = X_START; i < X_END; i+=X_STEP) {
     cairo_move_to(cr,i,Y_START+15);
     sprintf(str, "%g", ddata->xstart + xstep*(i - X_START)/X_STEP);
     cairo_show_text(cr, str);
     cairo_move_to(cr,i,Y_START-5);
     cairo_line_to(cr,i,Y_START+5);
  }

  for (int i = Y_START; i > Y_END; i-=Y_STEP) {
     cairo_move_to(cr,X_START-10,i);
     sprintf(str, "%g", ddata->ystart + ystep*(Y_START - i)/Y_STEP);
     cairo_show_text(cr, str);
     cairo_move_to(cr,X_START-5,i);
     cairo_line_to(cr,X_START+5,i);
  }
  draw_func(cr, ddata);

  cairo_stroke(cr);    
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{
  do_drawing(cr, user_data);

  return FALSE;
}

static gboolean clicked(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data)
{
    if (event->button == 1) {
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
}

struct draw_data getdata(struct entries *ent) {
   struct draw_data res = {.entry_str = ent->entry_f, .is_valid = TRUE};
   if (sscanf(gtk_entry_get_text(GTK_ENTRY(ent->entry_xs)), "%lf", &(res.xstart)) != 1)
   res.is_valid = FALSE;
   if (sscanf(gtk_entry_get_text(GTK_ENTRY(ent->entry_xe)), "%lf", &(res.xend)) != 1)
   res.is_valid = FALSE;
   if (sscanf(gtk_entry_get_text(GTK_ENTRY(ent->entry_ys)), "%lf", &(res.ystart)) != 1)
   res.is_valid = FALSE;
   if (sscanf(gtk_entry_get_text(GTK_ENTRY(ent->entry_ye)), "%lf", &(res.yend)) != 1)
   res.is_valid = FALSE;
   return res;
}

void ok_clicked(GtkWidget *p_widget, gpointer data) {
   struct entries *ent = data;
   
   struct draw_data *ddata = (struct draw_data*)malloc(sizeof(struct draw_data));
   *ddata = getdata(ent);
   if (ddata->is_valid) {
     gtk_window_close(GTK_WINDOW(ent->parent_w));
     create_graph(ent->parent_w,(gpointer)ddata);
   } else {
     gtk_window_close(GTK_WINDOW(ent->parent_w));
   }

}
void close_clicked(GtkWidget *window, gpointer data) {
  free(data);
  gtk_window_close(GTK_WINDOW(window));
}

void dimensions_popup(GtkWidget *p_widget, gpointer data) {
   GtkWidget *window;
   const gchar* entry_str = gtk_entry_get_text(GTK_ENTRY(data));
   GtkWidget *vbox, *hboxx, *hboxy;
   GtkWidget *labelx, *labely;
   GtkWidget *xstart, *xend, *ystart, *yend;
   GtkWidget *btnok;

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   hboxx = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
   hboxy = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
   vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
   gtk_container_add(GTK_CONTAINER(window),vbox);

  labelx = gtk_label_new("Input x start and end.");
  gtk_box_pack_start(GTK_BOX(vbox), labelx, FALSE, FALSE, 0);
  xstart = gtk_entry_new();
  xend = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hboxx), xstart, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hboxx), xend, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox),hboxx, FALSE,FALSE,0);

  labely = gtk_label_new("Input f(x) start and end.");
  gtk_box_pack_start(GTK_BOX(vbox),labely, FALSE,FALSE,0);
  ystart = gtk_entry_new();
  yend = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hboxy), ystart, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hboxy), yend, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox),hboxy, FALSE,FALSE,0);

  btnok = gtk_button_new_with_label("Ok");
  gtk_box_pack_start(GTK_BOX(vbox),btnok, FALSE,FALSE,0);


  struct entries *ent = (struct entries*)malloc(sizeof(struct entries));
  ent->parent_w = window;
  ent->entry_f = entry_str;
   ent->entry_xs=xstart;
   ent->entry_xe=xend;
   ent->entry_ys = ystart;
   ent->entry_ye=yend;

  g_signal_connect(btnok, "clicked", 
      G_CALLBACK(ok_clicked), ent);

  g_signal_connect(window, "destroy",
      G_CALLBACK(close_clicked), ent); 

      gtk_widget_show_all(window);

}

void create_graph(GtkWidget *p_widget, gpointer data)
{
  GtkWidget *window;
  GtkWidget *darea;
  struct draw_data *ddata = data;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);
 
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), ddata); 
  g_signal_connect(window, "destroy",
      G_CALLBACK(close_clicked), ddata);  
    
  g_signal_connect(window, "button-press-event", 
      G_CALLBACK(clicked), NULL);
 
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Lines");

  gtk_widget_show_all(window);

}
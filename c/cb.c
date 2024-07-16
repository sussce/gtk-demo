#include <gtk/gtk.h>

typedef struct {
  GtkWidget *root, *fixed, *target;
  gboolean move;
  
  gdouble x, y;
  guint w, h;
  guint w_max, h_max;
} dnd_data;

void
pressed_cb(GtkGestureClick* self, gint n_press,
           gdouble x, gdouble y, gpointer data) {
  dnd_data* dnd = (dnd_data*)data;
  if(n_press != 1) return;

  dnd->target = gtk_widget_pick(dnd->fixed, x, y, GTK_PICK_DEFAULT);
  dnd->move = TRUE;

  dnd->w = gtk_widget_get_width(dnd->target);
  dnd->h = gtk_widget_get_height(dnd->target);
  dnd->w_max = gtk_widget_get_width(dnd->fixed);
  dnd->h_max = gtk_widget_get_height(dnd->fixed);
  
  gdouble x_pos, y_pos;
  gtk_fixed_get_child_position(GTK_FIXED(dnd->fixed),
                               dnd->target, &x_pos, &y_pos);

  dnd->x = x - x_pos;
  dnd->y = y - y_pos;
}

void
released_cb(GtkGestureClick* self, gint n_press,
            gdouble x, gdouble y, gpointer data) {
  dnd_data* dnd = (dnd_data*)data;
  if(n_press != 1) return;

  dnd->target = NULL;
  dnd->move = FALSE;
  
  dnd->x = dnd->y =
    dnd->w = dnd->h =
    dnd->w_max = dnd->h_max = 0;
}
  
void
motion_cb(GtkEventControllerMotion* self,
          gdouble x, gdouble y, gpointer data) {
  dnd_data* dnd = (dnd_data*)data;
  if(!dnd->move) return;

  gdouble x_pos, y_pos, x_min, x_max, y_min, y_max;  
  x_pos = x_min = x - dnd->x;
  y_pos = y_min = y - dnd->y;
  x_max = x + dnd->w - dnd->x;
  y_max = y + dnd->h - dnd->y;

  if(x_min < 0) x_pos = 0;
  if(y_min < 0) y_pos = 0;
  if(x_max > dnd->w_max) x_pos = dnd->w_max - dnd->w;
  if(y_max > dnd->h_max) y_pos = dnd->h_max - dnd->h;
  
  gtk_fixed_move(GTK_FIXED(dnd->fixed), dnd->target, x_pos, y_pos);
}

void
draw_func(GtkDrawingArea* area, cairo_t* cr,
          int width, int height, gpointer data) {
  GdkRGBA bg;
  
  gdk_rgba_parse(&bg, "white");
  gdk_cairo_set_source_rgba(cr, &bg);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);
}

GtkWidget* set_info() {
  GtkWidget *box, *label1, *label2;

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

  label1 = gtk_label_new("LABEL1               name");
  label2 = gtk_label_new("LABEL2               name");

  gtk_box_append(GTK_BOX(box), label1);
  gtk_box_append(GTK_BOX(box), label2);

  return box;
}
GtkWidget* set_area() {
  GtkWidget* area = gtk_drawing_area_new();
  gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(area), 350);
  gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(area), 350);  
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_func, NULL, NULL);

  return area;
}
GtkWidget* set_comment() {
  GtkWidget *scrolled, *view;
  
  scrolled= gtk_scrolled_window_new();
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled), 150);
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled), 200);     
  
  view = gtk_text_view_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), view);

  return scrolled;
}
GtkWidget* set_op1() {
  GtkWidget *scrolled, *view;
  
  scrolled= gtk_scrolled_window_new();
  gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolled), 150);
  gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled), 200);     
  
  view = gtk_text_view_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), view);

  return scrolled;
}
void add_event_to_fixed(GtkWidget* fixed, GtkWidget* root, dnd_data* dnd) {
  GtkEventController *click, *motion;
  
  click = GTK_EVENT_CONTROLLER(gtk_gesture_click_new());
  motion = gtk_event_controller_motion_new();

  g_signal_connect(click, "pressed", G_CALLBACK(pressed_cb), dnd);
  g_signal_connect(click, "released", G_CALLBACK(released_cb), dnd);  
  g_signal_connect(motion, "motion", G_CALLBACK(motion_cb), dnd);
  gtk_widget_add_controller(fixed, click);
  gtk_widget_add_controller(fixed, motion);
}

#include <gtk/gtk.h>
#include "cb.c"

/* LAYOUT
  widget overlay
  calculate child position
  auto-calculate child position in resize
  auto-scale in resize
  layout signal
  layout shortcut
  close in layout
  scale in layout
  gesture event in layout
  implements widget
*/

/*
GApplication*
GtkApplication*

main
  return g_application_run(my_app_new(), argc, argv)

MyApp* myapp
return g_object_new(MY_TYPE_APP,
                    "application-id", "my.app.id",
                    "flags", G_FLAGS, NULL)

activate
  MyWidget* mywidget = my_widget_new()
  gtk_window_present(GTK_WINDOW(window))
  
startup
  action

MyWindow* mywindow
*/
  
GtkWidget* w;
void realize_cb(GtkWidget* self, gpointer data) {
  g_print("%p, %p\n", w, self);
}

void notify_cb(GObject* self, GParamSpec* pspec,
               gpointer user_data) {
  g_print("notify\n");
}
  
int main(int argc, char** argv) {
  GtkWidget *root, *header, *_fixed, *fixed;
  GtkWidget *info, *area, *op, *comment, *op1;

  gtk_init();
  root = gtk_window_new(); w = root;
  
  g_signal_connect(fixed, "notify::default-width",
                   G_CALLBACK(notify_cb), NULL);
  
  header = gtk_header_bar_new();
  gtk_window_set_titlebar(GTK_WINDOW(root), header);

  fixed = gtk_fixed_new();
  //fixed = gtk_fixed_layout_new();
  gtk_window_set_child(GTK_WINDOW(root), fixed);
  

  info = set_info();
  gtk_fixed_put(GTK_FIXED(fixed), info, 0, 0);

  area = set_area();
  gtk_fixed_put(GTK_FIXED(fixed), area, 0, 80);
  
  comment = set_comment();
  gtk_fixed_put(GTK_FIXED(fixed), comment, 500, 0);

  op1 = set_op1();
  gtk_fixed_put(GTK_FIXED(fixed), op1, 500, 220);
  
  dnd_data dnd = {
    .root = root,
    .fixed=fixed
  };
  add_event_to_fixed(fixed, root, &dnd);
  
  gtk_window_present(GTK_WINDOW(root));

  while (g_list_model_get_n_items (gtk_window_get_toplevels ()) > 0)
    g_main_context_iteration (NULL, TRUE);
}

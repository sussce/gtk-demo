#include <gtk/gtk.h>
#include "mywidget.h"
  
int main(int argc, char** argv) {
  GtkWidget* widget;
  
  gtk_init();
  
  widget = my_widget_new();

  /* while(!done) */
  g_main_context_iteration(NULL, TRUE);
  
  g_object_ref_sink(widget);
  return 0;
}

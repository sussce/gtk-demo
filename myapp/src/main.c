#include <gtk/gtk.h>
#include "myapp/myapp.h"

int main(int argc, char** argv) {
  return g_application_run(G_APPLICATION(my_app_new()), argc, argv);
}

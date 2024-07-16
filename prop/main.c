#include <glib-object.h>
#include "alabel.h"

int main(int argc, char** argv) {
  ALabel *label = a_label_new();
  
  g_object_unref(label);
  return 0;
}

#include <glib-object.h>
#include "afile.h"
  
int main(int argc, char** argv) {
  AFile* file = a_file_new();

  a_file_open(file, NULL);
  a_file_save(file, NULL);
  
  g_object_unref(file);
  return 0;
}

#ifndef _A_FILE_H_
#define _A_FILE_H_

#include <glib-object.h>

#define A_TYPE_FILE (a_file_get_type())
G_DECLARE_DERIVABLE_TYPE(AFile, a_file, A, FILE, GObject);

struct _AFileClass {
  GObjectClass parent_class;
  
  void (*open)(AFile* file, GError** error);
  gboolean (*can_mem_map)(AFile* file);
  
  gpointer padding[8];
};

AFile* a_file_new();
void a_file_open(AFile* file, GError** error);
void a_file_save(AFile* file, GError** error);

#endif /* _A_FILE_H_ */

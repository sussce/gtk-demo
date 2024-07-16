#include "afile.h"

typedef struct {
  gchar* data;
} AFilePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(AFile, a_file, G_TYPE_OBJECT);

/* native implementations */
static gboolean
a_file__can_mem_map(AFile* file) {
  return TRUE;
}

static void
a_file__close(AFile* file, GError** error) {
  g_print("%p, a_file__close\n", file);
}

static gboolean
a_file_can_mem_map(AFile* file) {
  return A_FILE_GET_CLASS(file)->can_mem_map(file);
}

static void
a_file_class_init(AFileClass *klass) {
  klass->open = NULL;
  klass->close = a_file__close;
  klass->can_mem_map = a_file__can_mem_map;
}

static void
a_file_init(AFile *self) {
  AFilePrivate* priv =
    a_file_get_instance_private(self);

  priv->data = NULL;
}

AFile*
a_file_new() {
  return g_object_new(A_TYPE_FILE, NULL);
}

void
a_file_open(AFile* file, GError** error) {
  AFileClass* klass;

  g_return_if_fail(A_IS_FILE(file));
  g_return_if_fail(error == NULL || *error == NULL);

  klass = A_FILE_GET_CLASS(file);
  g_return_if_fail(klass->open != NULL);
  
  if(a_file_can_mem_map(file)) {
    g_print("memory mapped I/O\n");
    klass->open(file, error);
  }
  else {
    g_print("fall back to stream I/O\n");
  }
}

void
a_file_close(AFile* file, GError** error) {
  AFileClass* klass;
  
  g_return_if_fail(A_IS_FILE(file));
  g_return_if_fail(error == NULL || *error == NULL);
  
  klass = A_FILE_GET_CLASS(file);
  klass->close(file, error);
}


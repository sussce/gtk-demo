#include "alabel.h"

typedef struct {
  gchar* data;
} ALabelPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ALabel, a_label, G_TYPE_OBJECT);

static void
a_label_class_init(ALabelClass* klass) {}

static void
a_label_init(ALabel* self) {}

ALabel*
a_label_new() {
  return g_object_new(A_TYPE_LABEL, NULL);
}

#include "alabel.h"

typedef struct {
  gchar* data;
  GObject* object;
} ALabelPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(ALabel, a_label, G_TYPE_OBJECT);

static void
a_label_constructed(GObject* object) {
  G_OBJECT_CLASS(a_label_parent_class)->constructed(object);
}

static void
a_label_dispose(GObject* object) {
  ALabelPrivate* priv =
    a_label_get_instance_private(A_LABEL(object));

  g_clear_object(&priv->object);
  G_OBJECT_CLASS(a_label_parent_class)->dispose(object);
}

static void
a_label_finalize(GObject* object) {
  ALabelPrivate* priv =
    a_label_get_instance_private(A_LABEL(object));

  g_free(priv->data);
  G_OBJECT_CLASS(a_label_parent_class)->finalize(object);
}

static void
a_label_class_init(ALabelClass* klass) {
  GObjectClass* gklass = G_OBJECT_CLASS(klass);

  gklass->constructed = a_label_constructed;
  gklass->dispose = a_label_dispose;
  gklass->finalize = a_label_finalize;
}

static void
a_label_init(ALabel* self) {
  ALabelPrivate* priv =
    a_label_get_instance_private(self);

  priv->data = NULL;
  priv->object = NULL;
}

ALabel*
a_label_new() {
  return g_object_new(A_TYPE_LABEL, NULL);
}

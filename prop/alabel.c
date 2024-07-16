#include "alabel.h"

enum {
  PROP_0,
  FILE_NAME,
  ZOOM_LEVEL,
  N_PROPS
};
struct _ALabel {
  GObject parent_instance;

  gchar* file_name;
  gint zoom_level;
};

static GParamSpec* props[N_PROPS] = {NULL};

G_DEFINE_TYPE(ALabel, a_label, G_TYPE_OBJECT);

static void
a_label_set_prop(GObject* object, guint prop_id,
                 const GValue* value, GParamSpec* pspec) {
  ALabel* self = A_LABEL(object);
  
  switch(prop_id) {
  case FILE_NAME:
    g_free(self->file_name);
    self->file_name = g_value_dup_string(value);
    break;
  case ZOOM_LEVEL:
    self->zoom_level = g_value_get_int(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
a_label_get_prop(GObject* object, guint prop_id,
                 GValue* value, GParamSpec* pspec) {
  ALabel* self = A_LABEL(object);
  
  switch(prop_id) {
  case FILE_NAME:
    g_value_set_string(value, self->file_name);
    break;
  case ZOOM_LEVEL:
    g_value_set_int(value, self->zoom_level);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
a_label_class_init(ALabelClass *klass) {
  GObjectClass* gklass = G_OBJECT_CLASS(klass);

  gklass->set_property = a_label_set_prop;
  gklass->get_property = a_label_get_prop;
  
  props[FILE_NAME] = g_param_spec_string("file_name", "file name", NULL, NULL,
                                         G_PARAM_READWRITE);
  props[ZOOM_LEVEL] = g_param_spec_int("zoom_level", "zoom level", NULL,
                                       0, 10, 1,
                                       G_PARAM_READWRITE);
  g_object_class_install_properties(gklass, N_PROPS, props);
}

static void
a_label_init(ALabel* self) {
  self->file_name = NULL;
  self->zoom_level = 0;
}

ALabel*
a_label_new() {
  return g_object_new(A_TYPE_LABEL, NULL);
}

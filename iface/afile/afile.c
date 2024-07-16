#include "afile.h"
#include "aeditable.h"

enum {
  PROP_0,
  PATH,
  N_PROPS,
  AUTOSAVE,
};
typedef struct {
  gchar* path;
  guint autosave;
} AFilePrivate;

static GParamSpec* props[N_PROPS] = {NULL};

static void a_file_editable_iface_init(AEditableInterface* iface);

/* implements iface */
G_DEFINE_TYPE_WITH_CODE(AFile, a_file, G_TYPE_OBJECT,
                        G_ADD_PRIVATE(AFile)
                        G_IMPLEMENT_INTERFACE(A_TYPE_EDITABLE,
                                              a_file_editable_iface_init));

static void
a_file__changed(AEditable* editable, gpointer data) {
  AFilePrivate* priv =
    a_file_get_instance_private(A_FILE(editable));

  g_print("%p, a_file__changed, %s, %d\n",
          priv, priv->path, priv->autosave);
}

static void
a_file__save(AEditable* editable, GError** error) {
  AFilePrivate *priv =
    a_file_get_instance_private(A_FILE(editable));
  
  g_print("%p, a_file__save, %s, %d\n",
          priv, priv->path, priv->autosave);
}

static gboolean
a_file__can_mem_map(AFile* file) {
  return TRUE;
}

static gboolean
a_file_can_mem_map(AFile* file) {
  return A_FILE_GET_CLASS(file)->can_mem_map(file);
}

static void
a_file_set_prop(GObject* object, guint prop_id,
                const GValue* value, GParamSpec* pspec) {
  AFilePrivate* priv =
    a_file_get_instance_private(A_FILE(object));
    
  switch(prop_id) {
  case PATH:
    g_free(priv->path);
    priv->path = g_value_dup_string(value);
    break;
  case AUTOSAVE:
    priv->autosave = g_value_get_uint(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
a_file_get_prop(GObject* object, guint prop_id,
                GValue* value, GParamSpec* pspec) {
  AFilePrivate *priv =
    a_file_get_instance_private(A_FILE(object));
    
  switch(prop_id) {
  case PATH:
    g_value_set_string(value, priv->path);
    break;
  case AUTOSAVE:
    g_value_set_uint(value, priv->autosave);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
a_file_editable_iface_init(AEditableInterface* iface) {
  iface->save = a_file__save;
  iface->changed = a_file__changed;
}

static void
a_file_class_init(AFileClass* klass) {
  GObjectClass* gklass;
  
  gklass = G_OBJECT_CLASS(klass);
  
  gklass->set_property = a_file_set_prop;
  gklass->get_property = a_file_get_prop;
  klass->can_mem_map = a_file__can_mem_map;
  klass->open = NULL;  

  props[PATH] = g_param_spec_string("path", "path", NULL, NULL,
                                    G_PARAM_READWRITE);
  g_object_class_install_properties(gklass, N_PROPS, props);
  g_object_class_override_property(gklass, AUTOSAVE, "autosave");
}

static void
a_file_init(AFile* self) {
  AFilePrivate* priv =
    a_file_get_instance_private(self);

  priv->path = NULL;
}

AFile*
a_file_new() {
  return g_object_new (A_TYPE_FILE, NULL);
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
a_file_save(AFile* file, GError** error) {
  AEditableInterface* iface;
  
  g_return_if_fail(A_IS_FILE(file));
  g_return_if_fail(error == NULL || *error == NULL);
  
  iface = A_EDITABLE_GET_IFACE(A_EDITABLE(file));
  g_return_if_fail(iface->save != NULL);
  
  iface->save(A_EDITABLE(file), error);
  g_signal_emit_by_name(file, "changed");
}

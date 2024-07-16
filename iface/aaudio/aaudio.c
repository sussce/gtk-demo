#include "aaudio.h"
#include "aeditable.h"

/* inherits props */
enum {
  PROP_0,
  HZ,
  N_PROPS
};
struct _AAudio {
  AFile parent_instance;
  
  gfloat hz;
};

static GParamSpec* props[N_PROPS] = {NULL};
static gpointer a_audio_parent_editable = NULL;

static void a_audio_editable_iface_init(AEditableInterface* iface);

G_DEFINE_TYPE_WITH_CODE(AAudio, a_audio, A_TYPE_FILE,
                        G_IMPLEMENT_INTERFACE(A_TYPE_EDITABLE,
                                              a_audio_editable_iface_init));

static void
a_audio__changed(AEditable* editable, gpointer data) {
  AAudio* audio = A_AUDIO(editable);

  g_print("%p, a_audio__changed, %f\n", audio, audio->hz);
}

static void
a_audio__save(AEditable* editable, GError** error) {
  AAudio* audio = A_AUDIO(editable);
  
  g_print("%p, a_audio__save, %f\n", audio, audio->hz);
}

static void
a_audio__open(AFile* file, GError** error) {
  AAudio* audio = A_AUDIO(file);
  
  g_print("%p, a_audio__open, %f\n", audio, audio->hz);
}

static void
a_audio_set_prop(GObject* object, guint prop_id,
                 const GValue* value, GParamSpec* pspec) {
  AAudio* self = A_AUDIO(object);
  
  switch(prop_id) {
  case HZ:
    self->hz = g_value_get_float(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
a_audio_get_prop(GObject* object, guint prop_id,
                 GValue* value, GParamSpec* pspec) {
  AAudio* self = A_AUDIO(object);
  switch(prop_id) {
  case HZ:
    g_value_set_float(value, self->hz);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}  

static void
a_audio_editable_iface_init(AEditableInterface* iface) {
  a_audio_parent_editable = g_type_interface_peek_parent(iface);
  
  iface->changed = a_audio__changed;
  iface->save = a_audio__save;
}

static void
a_audio_class_init(AAudioClass* klass) {
  GObjectClass* gklass = G_OBJECT_CLASS(klass);
  AFileClass* fklass = A_FILE_CLASS(klass);
    
  gklass->set_property = a_audio_set_prop;
  gklass->get_property = a_audio_get_prop;
  fklass->open = a_audio__open;
  
  props[HZ] = g_param_spec_float("hz", "hz", NULL,
                                 -FLT_MAX, FLT_MAX, 0.0,
                                 G_PARAM_READWRITE);  
  g_object_class_install_properties(gklass, N_PROPS, props);
}

static void
a_audio_init(AAudio* self) {}

AAudio*
a_audio_new() {
  return g_object_new(A_TYPE_AUDIO, NULL);
}

void
a_audio_open(AAudio* audio, GError** error) {
  AFileClass* fklass;
  
  g_return_if_fail(A_IS_AUDIO(audio));
  g_return_if_fail(error == NULL || *error == NULL);

  fklass = A_FILE_CLASS(A_AUDIO_GET_CLASS(audio));
  g_return_if_fail(fklass->open != NULL);
  
  fklass->open(A_FILE(audio), NULL);
}

void
a_audio_save(AAudio* audio, GError** error) {
  AEditableInterface* iface;
  
  g_return_if_fail(A_IS_AUDIO(audio));
  g_return_if_fail(error == NULL || *error == NULL);

  iface = A_EDITABLE_GET_IFACE(A_EDITABLE(audio));
  g_return_if_fail(iface->save != NULL);

  iface->save(A_EDITABLE(audio), error);
  g_signal_emit_by_name(audio, "changed", 10, 20);
}

void
a_audio_changed(AEditable* editable, gpointer data) {
  AAudio* audio = A_AUDIO(editable);
  
  g_print("%p, a_audio_changed, %f\n", audio, audio->hz);
}

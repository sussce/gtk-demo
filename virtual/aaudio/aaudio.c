#include "aaudio.h"

enum {
  PROP_0,
  PATH,
  N_PROPS
};
struct _AAudio {
  AFile parent_instance;
  
  gchar* path;
};

static GParamSpec* props[N_PROPS] = {NULL};

G_DEFINE_TYPE(AAudio, a_audio, A_TYPE_FILE);

static void
a_audio__open(AFile* file, GError** error) {
  AAudio* audio = A_AUDIO(file);

  g_print ("%p, a_audio__open, %s\n",
           audio, audio->path);

  if (NULL != A_FILE_CLASS(a_audio_parent_class)->open)
    A_FILE_CLASS(a_audio_parent_class)->open(file, error);
  
}

static void
a_audio_set_prop(GObject* object, guint prop_id,
                 const GValue* value, GParamSpec* pspec) {
  AAudio* self = A_AUDIO(object);
  
  switch(prop_id) {
  case PATH:
    g_free(self->path);
    self->path = g_value_dup_string(value);
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
  case PATH:
    g_value_set_string(value, self->path);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
a_audio_class_init(AAudioClass *klass) {
  GObjectClass* gklass;
  AFileClass* fklass;

  gklass = G_OBJECT_CLASS(klass);
  fklass = A_FILE_CLASS(klass);
  
  gklass->set_property = a_audio_set_prop;
  gklass->get_property = a_audio_get_prop;
  fklass->open = a_audio__open;

  props[PATH] = g_param_spec_string("path", "audio path", NULL, NULL,
                                    G_PARAM_READWRITE);
  g_object_class_install_properties(gklass, N_PROPS, props);
}

static void
a_audio_init(AAudio *self) {
  self->path = NULL;
}

AAudio*
a_audio_new() {
  return g_object_new(A_TYPE_AUDIO, NULL);
}

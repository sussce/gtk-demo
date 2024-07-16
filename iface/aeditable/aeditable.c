#include "aeditable.h"

enum {
  CHANGED,
  N_SIGS
};
enum {
  PROP_0,
  AUTOSAVE,
  N_PROPS
};

static guint sigs[N_SIGS] = {0};

G_DEFINE_INTERFACE(AEditable, a_editable, G_TYPE_OBJECT);

static void
a_editable__changed(AEditable* editable, gpointer data) {
  g_print("%p, a_editable__changed\n", editable);
}

static void
a_editable__save(AEditable* editable, GError** error) {
  g_print("%p, a_editable__save\n", editable);
}

static void
a_editable_default_init(AEditableInterface* iface) {
  iface->changed = a_editable__changed;
  iface->save = a_editable__save;

  g_object_interface_install_property(iface,
                                      g_param_spec_uint("autosave", "autosave", NULL,
                                                        0, 100, 0,
                                                        G_PARAM_READWRITE));
  
  sigs[CHANGED] = g_signal_new("changed",
                               A_TYPE_EDITABLE,
                               G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                               G_STRUCT_OFFSET(AEditableInterface, changed) /* class offset */,
                               NULL /* accumulator */,
                               NULL /* accumulator data */,
                               NULL /* C marshaller */,
                               G_TYPE_NONE /* return type */,
                               0     /* n_params, */
                               /* param-types */);
}

void
a_editable_save(AEditable* editable, GError** error) {
  AEditableInterface* iface;
  
  g_return_if_fail(A_IS_EDITABLE(editable));
  g_return_if_fail(error == NULL || *error == NULL);

  iface = A_EDITABLE_GET_IFACE(editable);
  g_return_if_fail(iface->save != NULL);
  
  iface->save(editable, error);
}

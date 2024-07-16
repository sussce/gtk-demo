#ifndef _A_EDITABLE_H_
#define _A_EDITABLE_H_

#include <glib-object.h>

#define A_TYPE_EDITABLE (a_editable_get_type())
G_DECLARE_INTERFACE(AEditable, a_editable, A, EDITABLE, GObject);

struct _AEditableInterface {
  GTypeInterface parent_iface;

  /* signals */
  void (*changed)(AEditable* editable, /* args.., */ gpointer data);

  /* public */
  void (*save)(AEditable* editable, GError** error);
};

void a_editable_save(AEditable* editable, GError** error);

#endif /* _A_EDITABLE_H_ */

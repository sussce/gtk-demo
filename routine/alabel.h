#ifndef _A_LABEL_H_
#define _A_LABEL_H_

#include <glib-object.h>

#define A_TYPE_LABEL (a_label_get_type())
G_DECLARE_DERIVABLE_TYPE(ALabel, a_label, A, LABEL, GObject);

struct _ALabelClass {
  GObjectClass parent_class;

  gpointer padding[8];
};

ALabel* a_label_new();

#endif /* _A_LABEL_H_ */

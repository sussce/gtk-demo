#ifndef _A_LABEL_H_
#define _A_LABEL_H_

#include <glib-object.h>

#define A_TYPE_LABEL (a_label_get_type())
G_DECLARE_FINAL_TYPE(ALabel, a_label, A, LABEL, GObject);
  
ALabel* a_label_new();

#endif /* _A_LABEL_H_ */

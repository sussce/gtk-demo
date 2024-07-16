#ifndef _MY_WIDGET_PRIVATE_H_
#define _MY_WIDGET_PRIVATE_H_

#include "mywidget/mywidget.h"

#define MY_STATE_FLAGS_BITS 16

struct _MyWidgetPrivate {
  guint state :MY_STATE_FLAGS_BITS;

  GtkWidget* info;
  GtkWidget* area;
  GtkWidget* op;
  GtkWidget* comment;
  GtkWidget* stats;  
  gchar* name;
};
struct _MyWidgetClassPrivate {
  guint activate_sig;
};

#endif /* _MY_WIDGET_PRIVATE_H_ */

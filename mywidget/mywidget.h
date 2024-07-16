#ifndef _MY_WIDGET_H_
#define _MY_WIDGET_H_

#include <gtk/gtk.h>

#define MY_TYPE_WIDGET (my_widget_get_type())
#define MY_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), MY_TYPE_WIDGET, MyWidget))
#define MY_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass), MY_TYPE_WIDGET, MyWidgetClass))
#define MY_IS_WIDGET(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), MY_TYPE_WIDGET))
#define MY_IS__WIDGET(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), MY_TYPE_WIDGET))
#define MY_WIDGET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS((obj), MY_TYPE_WIDGET, MyWidgetClass))

typedef struct _MyWidget MyWidget;
typedef struct _MyWidgetClass MyWidgetClass;
typedef struct _MyWidgetPrivate MyWidgetPrivate;
typedef struct _MyWidgetClassPrivate MyWidgetClassPrivate;

struct _MyWidget {
  GtkWidget parent_instance;

  /* private */
  MyWidgetPrivate* priv;
};
struct _MyWidgetClass {
  GtkWidgetClass parent_class;

  /* private */
  MyWidgetClassPrivate* priv;
  gpointer padding[8];
};

GType my_widget_get_type();
GtkWidget* my_widget_new();

#endif /* _MY_WIDGET_H_ */

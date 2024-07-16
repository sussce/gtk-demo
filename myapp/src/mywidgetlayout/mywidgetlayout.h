#ifndef _MY_WIDGET_LAYOUT_H_
#define _MY_WIDGET_LAYOUT_H_

#include <gtk/gtk.h>

#define MY_TYPE_WIDGET_LAYOUT (my_widget_layout_get_type())
G_DECLARE_FINAL_TYPE(MyWidgetLayout, my_widget_layout, MY, WIDGET_LAYOUT, GtkLayoutManager);

GtkLayoutManager* my_widget_layout_new();

#endif /* _MY_WIDGET_LAYOUT_H_ */


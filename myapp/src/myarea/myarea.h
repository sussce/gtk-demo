#ifndef _MY_AREA_H_
#define _MY_AREA_H_

#include <gtk/gtk.h>

#define MY_TYPE_AREA (my_area_get_type())
G_DECLARE_FINAL_TYPE(MyArea, my_area, MY, AREA, GtkFrame);

GtkWidget* my_area_new();

#endif /* _MY_AREA_H_ */

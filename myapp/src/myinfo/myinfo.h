#ifndef _MY_INFO_H_
#define _MY_INFO_H_

#include <gtk/gtk.h>

#define MY_TYPE_INFO (my_info_get_type())
G_DECLARE_FINAL_TYPE(MyInfo, my_info, MY, INFO, GtkGrid);

GtkWidget* my_info_new();

#endif /* _MY_INFO_H_ */

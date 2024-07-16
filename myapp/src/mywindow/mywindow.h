#ifndef _MY_WINDOW_H_
#define _MY_WINDOW_H_

#include <gtk/gtk.h>
#include "myapp/myapp.h"

#define MY_TYPE_WINDOW (my_window_get_type ())
G_DECLARE_FINAL_TYPE(MyWindow, my_window, MY, WINDOW, GtkApplicationWindow);

GtkWidget* my_window_new (MyApp* app);
void my_window_open (MyWindow* window, GFile* file);

#endif /* _MY_WINDOW_H_ */

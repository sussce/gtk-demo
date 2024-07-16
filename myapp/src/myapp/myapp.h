#ifndef _MY_APP_H_
#define _MY_APP_H_

#include <gtk/gtk.h>

#define MY_TYPE_APP (my_app_get_type())
G_DECLARE_FINAL_TYPE(MyApp, my_app, MY, APP, GtkApplication);

MyApp* my_app_new();

#endif /* _MY_APP_H_ */

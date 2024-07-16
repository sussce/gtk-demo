#include "myinfo.h"

struct _MyInfo {
  GtkGrid parent_instance;
};

G_DEFINE_TYPE(MyInfo, my_info, GTK_TYPE_GRID);

static void
my_info_dispose(GObject* object) {
  G_OBJECT_CLASS(my_info_parent_class)->dispose(object);
}

static void
my_info_class_init(MyInfoClass* klass) {
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);
  
  G_OBJECT_CLASS(klass)->dispose = my_info_dispose;
  
  gtk_widget_class_set_template_from_resource(wklass, "/my/app/info.ui"); 
}

static void
my_info_init(MyInfo* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget*
my_info_new() {
  return g_object_new(MY_TYPE_INFO, NULL);
}

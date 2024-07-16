#include "myapp/myapp.h"
#include "mywindow/mywindow.h"
#include "mywidget/mywidget.h"

struct _MyWindow {
  GtkApplicationWindow parent_instance;

  GtkWidget* menu;
};

G_DEFINE_TYPE(MyWindow, my_window, GTK_TYPE_APPLICATION_WINDOW);

static void
my_window_dispose (GObject* object) {
  G_OBJECT_CLASS(my_window_parent_class)->dispose (object);
}

static void
my_window_class_init (MyWindowClass* klass) {
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);
  
  G_OBJECT_CLASS(klass)->dispose = my_window_dispose;
    
  gtk_widget_class_set_template_from_resource (wklass, "/my/app/window.ui");
  gtk_widget_class_bind_template_child (wklass, MyWindow, menu);  
}

static void
my_window_init (MyWindow* self) {
  GtkBuilder* menu_builder;
  GMenuModel* menu;
  
  g_type_ensure (MY_TYPE_WIDGET);
  gtk_widget_init_template (GTK_WIDGET(self));

  menu_builder = gtk_builder_new_from_resource ("/my/app/menu.ui");
  menu = G_MENU_MODEL(gtk_builder_get_object (menu_builder, "menu"));
  gtk_menu_button_set_menu_model (GTK_MENU_BUTTON(self->menu), menu);
  
  g_object_unref (menu_builder);
}

GtkWidget*
my_window_new (MyApp* app) {
  return g_object_new (MY_TYPE_WINDOW,
                       "application", app, NULL);
}

void
my_window_open (MyWindow* window, GFile* file) {
  gchar* filename, content;

  filename = g_file_get_basename (file);

  //my_widget_open (window->widget, file);
  
  g_free (filename);
}

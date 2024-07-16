#include "myarea.h"

struct _MyArea {
  GtkFrame parent_instance;
};
 
G_DEFINE_TYPE(MyArea, my_area, GTK_TYPE_FRAME);

static void
my_area_snapshot(GtkWidget* widget, GtkSnapshot* snapshot) {
  GdkRGBA white;
  float w, h;
  
  w = gtk_widget_get_width(widget);
  h = gtk_widget_get_height(widget);

  gdk_rgba_parse(&white, "white");
  gtk_snapshot_append_color(snapshot, &white,
                            &GRAPHENE_RECT_INIT(0, 0, w, h));

  GTK_WIDGET_CLASS(my_area_parent_class)->snapshot(widget, snapshot);
}

static void
my_area_dispose(GObject* object) {
  G_OBJECT_CLASS(my_area_parent_class)->dispose(object);
}

static void
my_area_class_init(MyAreaClass* klass) {
  GObjectClass* oklass = G_OBJECT_CLASS(klass);
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);

  oklass->dispose = my_area_dispose;
  wklass->snapshot = my_area_snapshot;

  gtk_widget_class_set_template_from_resource(wklass, "/my/app/area.ui");
}

static void
my_area_init(MyArea* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget*
my_area_new() {
  return g_object_new(MY_TYPE_AREA, NULL);
}

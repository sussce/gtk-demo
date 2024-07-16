#include "mystats.h"

struct _MyStats {
  GtkBox parent_instance;
};

G_DEFINE_TYPE(MyStats, my_stats, GTK_TYPE_BOX);

static void
my_stats_dispose(GObject* object) {
  G_OBJECT_CLASS(my_stats_parent_class)->dispose(object);
}

static void
my_stats_class_init(MyStatsClass* klass) {
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);
  
  G_OBJECT_CLASS(klass)->dispose = my_stats_dispose;
  
  gtk_widget_class_set_template_from_resource(wklass, "/my/app/stats.ui"); 
}

static void
my_stats_init(MyStats* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget*
my_stats_new() {
  return g_object_new(MY_TYPE_STATS, NULL);
}

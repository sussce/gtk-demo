#include "myop.h"

struct _MyOp {
  GtkBox parent_instance;
};

G_DEFINE_TYPE(MyOp, my_op, GTK_TYPE_BOX);

static void
my_op_dispose(GObject* object) {
  G_OBJECT_CLASS(my_op_parent_class)->dispose(object);
}

static void
my_op_class_init(MyOpClass* klass) {
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);
  
  G_OBJECT_CLASS(klass)->dispose = my_op_dispose;
  
  gtk_widget_class_set_template_from_resource(wklass, "/my/app/op.ui"); 
}

static void
my_op_init(MyOp* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget*
my_op_new() {
  return g_object_new(MY_TYPE_OP, NULL);
}

#include "mycomment.h"

struct _MyComment {
  GtkFrame parent_instance;
};

G_DEFINE_TYPE(MyComment, my_comment, GTK_TYPE_FRAME);

static void
my_comment_dispose(GObject* object) {
  G_OBJECT_CLASS(my_comment_parent_class)->dispose(object);
}

static void
my_comment_class_init(MyCommentClass* klass) {
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(klass);
  
  G_OBJECT_CLASS(klass)->dispose = my_comment_dispose;
  
  gtk_widget_class_set_template_from_resource(wklass, "/my/app/comment.ui"); 
}

static void
my_comment_init(MyComment* self) {
  gtk_widget_init_template(GTK_WIDGET(self));
}

GtkWidget*
my_comment_new() {
  return g_object_new(MY_TYPE_COMMENT, NULL);
}



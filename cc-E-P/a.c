#include "gtype.h"

G_DECLARE_DERIVABLE_TYPE (MyWidget, my_widget, MY, WIDGET, MyWidget);

G_DEFINE_TYPE_WITH_CODE (MyWidget, my_widget, GTK_TYPE_WIDGET,
                         G_ADD_PRIVATE (MyWidget)
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_ORIENTABLE, NULL));

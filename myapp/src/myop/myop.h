#ifndef _MY_OP_H_
#define _MY_OP_H_

#include <gtk/gtk.h>

#define MY_TYPE_OP (my_op_get_type())
G_DECLARE_FINAL_TYPE(MyOp, my_op, MY, OP, GtkBox);

GtkWidget* my_op_new();

#endif /* _MY_OP_H_ */

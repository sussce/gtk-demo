#ifndef _MY_COMMENT_H_
#define _MY_COMMENT_H_

#include <gtk/gtk.h>

#define MY_TYPE_COMMENT (my_comment_get_type())
G_DECLARE_FINAL_TYPE(MyComment, my_comment, MY, COMMENT, GtkFrame);

GtkWidget* my_comment_new();

#endif /* _MY_COMMENT_H_ */

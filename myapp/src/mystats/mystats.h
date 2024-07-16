#ifndef _MY_STATS_H_
#define _MY_STATS_H_

#include <gtk/gtk.h>

#define MY_TYPE_STATS (my_stats_get_type())
G_DECLARE_FINAL_TYPE(MyStats, my_stats, MY, STATS, GtkBox);

GtkWidget* my_stats_new();

#endif /* _MY_STATS_H_ */

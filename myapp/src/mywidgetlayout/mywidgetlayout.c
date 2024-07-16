#include "mywidgetlayout.h"

enum {
  PROP_0,
  N_PROPS,
  ORIENTATION
};
typedef struct {
  int x;
  int y;
} alloc_point_t;
struct _MyWidgetLayout {
  GtkLayoutManager parent_instance;
  
  GtkOrientation orientation;
};

static GParamSpec* props[N_PROPS] = {NULL};

G_DEFINE_TYPE_WITH_CODE(MyWidgetLayout, my_widget_layout, GTK_TYPE_LAYOUT_MANAGER,
                        G_IMPLEMENT_INTERFACE(GTK_TYPE_ORIENTABLE, NULL));

static inline gboolean
child_is_visible (GtkWidget* child) {
  return gtk_widget_get_visible (child) &&
    gtk_widget_get_child_visible (child);
}

static void
count_visible_children (GtkWidget* widget, int* n_visible) {
  GtkWidget* child;

  for (child = gtk_widget_get_first_child (widget);
       child != NULL;
       child = gtk_widget_get_next_sibling (child)) {
    if (child_is_visible (child))
      *n_visible += 1;
  }
}

static void
measure_size_aligned (GtkWidget* widget, GtkOrientation orientation,
                      int* minimum, int* natural, int lines) {
  GtkWidget* child;
  GtkRequestedSize* sizes;
  int min_size = 0, nat_size = 0;
  int i = 0;
  
  sizes = g_newa (GtkRequestedSize, lines);
  
  for (child = gtk_widget_get_first_child (widget);
       child != NULL;
       child = gtk_widget_get_next_sibling (child)) {
    int child_min = 0, child_nat = 0;
    
    if (!child_is_visible (child))
      continue;
    
    gtk_widget_measure (child, orientation, -1,
                        &child_min, &child_nat, NULL, NULL);

    sizes[i%lines].minimum_size =
      MAX(sizes[i%lines].minimum_size, child_min);
    sizes[i%lines].natural_size =
      MAX(sizes[i%lines].natural_size, child_nat);
    i++;
  }

  for (i = 0; i < lines; i++) {
    min_size += sizes[i].minimum_size;
    nat_size += sizes[i].natural_size;
  }

  // g_free (sizes);
  
  if (minimum)
    *minimum = min_size;
  if (natural)
    *natural = nat_size;
}

static void
measure_size_aligned_opp (GtkWidget* widget, GtkOrientation orientation, int for_size,
                          int* minimum, int* natural, int lines) {
  GtkWidget* child;
  int min_size = 0, nat_size = 0, line_min = 0, line_nat = 0;
  int i = 0;
  
  for (child = gtk_widget_get_first_child(widget);
       child != NULL;
       child = gtk_widget_get_next_sibling(child)) {
    int child_min = 0, child_nat = 0;

    if (!child_is_visible (child))
      continue;
    
    if (i % lines == 0) {
      min_size += line_min;
      nat_size += line_nat;
      line_min = 0;
      line_nat = 0;
    }
    
    gtk_widget_measure (child, 1 - orientation, for_size,
                        &child_min, &child_nat, NULL, NULL);
    
    line_min = MAX(line_min, child_min);
    line_nat = MAX(line_nat, child_nat);
    i++;
  }

  min_size += line_min;
  nat_size += line_nat;
  
  if (minimum)
    *minimum = min_size;
  if (natural)
    *natural = nat_size;
}

static void
measure_size (GtkWidget* widget, GtkOrientation orientation,
              int* minimum, int* natural) {
  GtkWidget* child;
  int min_size = 0, nat_size = 0;
  
  for (child = gtk_widget_get_first_child (widget);
      child != NULL;
      child = gtk_widget_get_next_sibling (child)) {
    int child_min = 0, child_nat = 0;

    if (!child_is_visible (child))
      continue;
    
    gtk_widget_measure (child, orientation, -1,
                        &child_min, &child_nat, NULL, NULL);
    
    min_size += child_min;
    nat_size += child_nat;
  }
  
  if (minimum)
    *minimum = min_size;
  if (natural)
    *natural = nat_size;
}

static void
measure_size_max (GtkWidget* widget, GtkOrientation orientation,
                  int* minimum, int* natural) {
  GtkWidget* child;
  int min_size = 0, nat_size = 0;

  for (child = gtk_widget_get_first_child (widget);
      child != NULL;
      child = gtk_widget_get_next_sibling (child)) {
    int child_min = 0, child_nat = 0;

    if (!child_is_visible (child))
      continue;
    
    gtk_widget_measure (child, orientation, -1,
                        &child_min, &child_nat, NULL, NULL);
    
    min_size = MAX(min_size, child_min);
    nat_size = MAX(nat_size, child_nat);    
  }

  if (minimum)
    *minimum = min_size;
  if (natural)
    *natural = nat_size;  
}

static void
my_widget_layout_measure (GtkLayoutManager* layout_manager, GtkWidget* widget,
                          GtkOrientation orientation, int for_size,
                          int *minimum, int* natural,
                          int* minimum_baseline, int* natural_baseline) {
  MyWidgetLayout* self = MY_WIDGET_LAYOUT(layout_manager);
  GtkWidget* child;
  int min_size = 0, nat_size = 0;

  if (self->orientation != orientation) {
    if (for_size < 0) {
      int given_size = 0, dummy;
      
      my_widget_layout_measure (layout_manager, widget, self->orientation, -1,
                                &given_size, &dummy, NULL, NULL);
      my_widget_layout_measure (layout_manager, widget, orientation, given_size,
                                &min_size, &nat_size, NULL, NULL);
    }
    else {/* *-mode */
      int avail_size, given_size, lines, min_size_max, nat_size_max;
      int n_children, dummy;

      avail_size = given_size = lines = min_size_max = nat_size_max = 0;
      n_children = 0;
      
      count_visible_children (widget, &n_children);
      if (!n_children)
        return;
      
      my_widget_layout_measure (layout_manager, widget, self->orientation, -1,
                                &given_size, &dummy, NULL, NULL);

      avail_size = MAX(given_size, for_size);

      if (avail_size <= 0) {
        *minimum = 0;
        *natural = 0;
        return;
      }
      
      measure_size_max (widget, self->orientation,
                        &min_size_max, &nat_size_max);

      if (nat_size_max <= 0) {
        *minimum = 0;
        *natural = 0;
        return;
      }
      
      lines = avail_size / nat_size_max;
      
      measure_size_aligned_opp (widget, self->orientation, avail_size,
                                &min_size, &nat_size, lines);
    }
  }
  else {
    if(for_size < 0) {
      int avail_size, lines;
      int min_size_max, nat_size_max, dummy;

      avail_size = lines = min_size_max = nat_size_max = 0;
      
      measure_size (widget, orientation,
                    &avail_size, &dummy);

      measure_size_max (widget, orientation,
                        &min_size_max, &nat_size_max);
      
      lines = avail_size / nat_size_max;
      
      measure_size_aligned (widget, orientation,
                            &min_size, &nat_size, lines);
    }
    else {
      my_widget_layout_measure (layout_manager, widget, orientation, -1,
                                &min_size, &nat_size, NULL, NULL);
    }
  }

  if (min_size > nat_size)
    min_size = nat_size;
  
  if (minimum)
    *minimum = min_size;
  if (natural)
    *natural = nat_size;
}

static void
allocate_pos (GtkOrientation orientation, int size,
              GtkRequestedSize* sizes, alloc_point_t* pos,
              int n_children) {
  int avail_size, line_size, line_size_max;
  int i, start;

  line_size = line_size_max = 0;
  avail_size = size;
  start = 0;
  
  for(i = 1; i < n_children; i++) {
    avail_size -= sizes[i].minimum_size;
    
    if (avail_size < 0) {
      avail_size = size - sizes[i].minimum_size;
        
      for (int _start = start; _start < i; _start++) {
        int child_min_opp = 0, child_nat_opp = 0;
 
        gtk_widget_measure(sizes[_start].data, 1 - orientation,
                           sizes[_start].minimum_size,
                           &child_min_opp, &child_nat_opp, NULL, NULL);

        line_size_max = MAX(line_size_max, child_min_opp);
      }
      
      start = i;
      line_size += line_size_max;

      if (orientation == GTK_ORIENTATION_HORIZONTAL) {
        pos[i].x = 0;
        pos[i].y = line_size;
      }
      else {
        pos[i].x = line_size;
        pos[i].y = 0;
      }
    }
    else {
      if (orientation == GTK_ORIENTATION_HORIZONTAL) {
        pos[i].x = pos[i-1].x + sizes[i-1].minimum_size;
        pos[i].y = line_size;
      }
      else {
        pos[i].x = line_size;
        pos[i].y = pos[i-1].y + sizes[i-1].minimum_size;        
      }
    }    
  }
    
}

static void
allocate_size (GtkWidget* widget, GtkOrientation orientation,
               int for_size, GtkRequestedSize* sizes) {
  GtkWidget* child;
  int i = 0;
  
  for(child = gtk_widget_get_first_child (widget);
      child != NULL;
      child = gtk_widget_get_next_sibling (child)) {
    int child_min = 0, child_nat = 0;

    if(!child_is_visible (child))
      continue;
    
    gtk_widget_measure (child, orientation, for_size,
                        &child_min, &child_nat, NULL, NULL);
    
    sizes[i].minimum_size = child_min;
    sizes[i].natural_size = child_nat;
    sizes[i].data = child;
    i++;
  }
}

static void
allocate (GtkAllocation* alloc,
          GtkRequestedSize* sizes, alloc_point_t* pos,
          int n_children) {
  int i;
  
  for(i = 0; i < n_children; i++) {
    alloc->width = sizes[i].minimum_size;
    alloc->height = sizes[i].natural_size;
    alloc->x = pos[i].x;
    alloc->y = pos[i].y;
 
    gtk_widget_size_allocate(sizes[i].data, alloc, -1);
  }
}

static void
my_widget_layout_allocate (GtkLayoutManager* layout_manager, GtkWidget* widget,
                           int width, int height, int baseline) {
  MyWidgetLayout* self = MY_WIDGET_LAYOUT(layout_manager);
  GtkWidget* child;
  GtkAllocation alloc;
  GtkRequestedSize* sizes = NULL;
  alloc_point_t* pos = NULL;
  int avail_size = 0, n_children = 0;
  
  count_visible_children(widget, &n_children);

  if(n_children <= 0)
    return;
  
  sizes = g_new0 (GtkRequestedSize, n_children);
  pos = g_new0 (alloc_point_t, n_children);
  
  /* available size */
  if(self->orientation == GTK_ORIENTATION_HORIZONTAL) {
    avail_size = width; /* -spacing */
  }
  else {
    avail_size = height;
  }
 
  allocate_size (widget, self->orientation, -1, sizes);

  /* measure the preferred line,
     it's no smaller than the size with max-size per child */

  allocate_pos (self->orientation, avail_size,
                sizes, pos, n_children);
  
  allocate (&alloc, sizes, pos, n_children);  
  
  g_free (sizes);
  g_free (pos);
}

static void
_gtk_widget_update_orientation (GtkWidget *widget,
                                GtkOrientation orientation) {
  g_return_if_fail(GTK_IS_WIDGET(widget));
  
  if (orientation == GTK_ORIENTATION_HORIZONTAL) {
    gtk_widget_add_css_class(widget, "horizontal");
    gtk_widget_remove_css_class(widget, "vertical");
  }
  else {
    gtk_widget_add_css_class(widget, "vertical");
    gtk_widget_remove_css_class (widget, "horizontal");
  }
  
  gtk_accessible_update_property(GTK_ACCESSIBLE(widget),
                                 GTK_ACCESSIBLE_PROPERTY_ORIENTATION, orientation,
                                 -1);
}

static void
my_widget_layout_set_orientation (MyWidgetLayout* self,
                                  GtkOrientation orientation) {
  GtkLayoutManager* layout_manager = GTK_LAYOUT_MANAGER(self);
  GtkWidget *widget;

  if(self->orientation == orientation) return;

  self->orientation = orientation;
  
  widget = gtk_layout_manager_get_widget(layout_manager);
  
  if(widget != NULL && GTK_IS_ORIENTABLE(widget))
    _gtk_widget_update_orientation(widget, self->orientation);

  gtk_layout_manager_layout_changed(layout_manager);
  g_object_notify(G_OBJECT(self), "orientation");  
}

static void
my_widget_layout_set_prop (GObject* object, guint prop_id,
                           const GValue* value, GParamSpec* pspec) {
  MyWidgetLayout* self = MY_WIDGET_LAYOUT(object);

  switch(prop_id) {
  case ORIENTATION:
    my_widget_layout_set_orientation(self, g_value_get_enum(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
my_widget_layout_get_prop (GObject* object, guint prop_id,
                           GValue* value, GParamSpec* pspec) {
  MyWidgetLayout* self = MY_WIDGET_LAYOUT(object);

  switch(prop_id) {
  case ORIENTATION:
    g_value_set_enum(value, self->orientation);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break; 
  }  
}

static void
my_widget_layout_class_init (MyWidgetLayoutClass* klass) {
  GObjectClass* oklass = G_OBJECT_CLASS(klass);
  GtkLayoutManagerClass* lmklass = GTK_LAYOUT_MANAGER_CLASS(klass);

  oklass->set_property = my_widget_layout_set_prop;
  oklass->get_property = my_widget_layout_get_prop;
  lmklass->measure = my_widget_layout_measure;
  lmklass->allocate = my_widget_layout_allocate;
  
  g_object_class_override_property(oklass, ORIENTATION, "orientation");
}

static void
my_widget_layout_init (MyWidgetLayout* self) {
  self->orientation = GTK_ORIENTATION_HORIZONTAL;
}

GtkLayoutManager*
my_widget_layout_new (GtkOrientation orientation) {
  return g_object_new(MY_TYPE_WIDGET_LAYOUT,
                      "orientation", orientation, NULL);
}


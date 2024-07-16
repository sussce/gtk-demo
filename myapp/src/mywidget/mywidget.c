#include "mywidgetprivate.h"
#include "mywidgetlayout/mywidgetlayout.h"
#include "myinfo/myinfo.h"
#include "myarea/myarea.h"
#include "myop/myop.h"
#include "mycomment/mycomment.h"
#include "mystats/mystats.h"

#define GTK_PARAM_READWRITE \
  G_PARAM_READWRITE|G_PARAM_STATIC_NAME| \
  G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB

enum {
  ACTIVATE,
  N_SIGS
};
enum {
  PROP_0,
  NAME,
  N_PROPS,
  ORIENTATION
};
typedef enum {
  MY_STATE_FLAGS_UNSET = 0,
  MY_STATE_FLAGS_ACTIVE = 1<<0,
  MY_STATE_FLAGS_FOCUSED = 1<<1,
  MY_STATE_FLAGS_SELECTED = 1<<2
} my_state_flags;
typedef struct {
  guint flags_to_set;
  guint flags_to_unset;
} my_state_data;

static gint MyWidget_private_offset = 0;
static gpointer my_widget_parent_class = NULL;
static GtkBuildableIface* my_widget_parent_buildable_iface = NULL;
static guint sigs[N_SIGS] = {0};
static GParamSpec* props[N_PROPS] = {NULL};

static void my_widget_buildable_iface_init (GtkBuildableIface* iface);
static void my_widget_base_init (gpointer g_class);
static void my_widget_base_finalize (gpointer g_class);
static void my_widget_class_init (gpointer g_class, gpointer class_data);
static void my_widget_init (GTypeInstance* instance, gpointer g_class);
static void my_widget_dispose (GObject* object);
static void my_widget_finalize (GObject *object);
static void my_widget_set_prop (GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec);
static void my_widget_get_prop (GObject* object, guint prop_id, GValue* value, GParamSpec* pspec);

GType
my_widget_get_type () {
  static GType type = 0;
  if (G_UNLIKELY(type == 0)) {
    const GTypeInfo type_info = {
      sizeof(MyWidgetClass) /* class_size */,
      my_widget_base_init /* base_init */,
      my_widget_base_finalize /* base_finalize */,
      my_widget_class_init /* class_init */,
      NULL /* class_finalize */,
      NULL /* class_data */,
      sizeof(MyWidget) /* instance_size */,
      0 /* n_preallocs */,
      my_widget_init /* instance_init */,
      NULL /* value_table */
    };
    const GInterfaceInfo buildable_info = {
      (GInterfaceInitFunc)my_widget_buildable_iface_init,
      (GInterfaceFinalizeFunc)NULL,
      NULL /* iface data */,
    };
    const GInterfaceInfo orientable_info = {
      (GInterfaceInitFunc)NULL,
      (GInterfaceFinalizeFunc)NULL,
      NULL
    };
    
    type = g_type_register_static (GTK_TYPE_WIDGET,
                                  g_intern_static_string ("MyWidget"),
                                  &type_info,
                                  (GTypeFlags)0);
 
    g_type_add_class_private (type, sizeof(MyWidgetClassPrivate));
    MyWidget_private_offset = g_type_add_instance_private (type, sizeof(MyWidgetPrivate));
    g_type_add_interface_static (type, GTK_TYPE_BUILDABLE, &buildable_info);
    g_type_add_interface_static (type, GTK_TYPE_ORIENTABLE, &orientable_info);    
  }
  return type;
}

static inline gpointer
my_widget_get_instance_private (MyWidget* widget) {
  return (G_STRUCT_MEMBER_P(widget, MyWidget_private_offset));  
}

static void
_gtk_widget_update_orientation (GtkWidget *widget,
                                GtkOrientation orientation) {
  g_return_if_fail (GTK_IS_WIDGET(widget));
  
  if (orientation == GTK_ORIENTATION_HORIZONTAL) {
    gtk_widget_add_css_class (widget, "horizontal");
    gtk_widget_remove_css_class (widget, "vertical");
  }
  else {
    gtk_widget_add_css_class (widget, "vertical");
    gtk_widget_remove_css_class (widget, "horizontal");
  }
  
  gtk_accessible_update_property (GTK_ACCESSIBLE(widget),
                                  GTK_ACCESSIBLE_PROPERTY_ORIENTATION,
                                  orientation, -1);
}

static void
_my_widget_set_orientation (MyWidget* widget,
                            GtkOrientation orientation, GParamSpec* pspec) {
  GtkLayoutManager* layout_manager =
    gtk_widget_get_layout_manager (GTK_WIDGET(widget));

  if (orientation !=
      gtk_orientable_get_orientation (GTK_ORIENTABLE(layout_manager))) {
    gtk_orientable_set_orientation (GTK_ORIENTABLE(layout_manager),
                                    orientation);
    
    _gtk_widget_update_orientation (GTK_WIDGET(widget),
                                    orientation);
     
    g_object_notify_by_pspec (G_OBJECT(widget), pspec);
  }
}

static void
my_widget_set_prop (GObject* object, guint prop_id,
                    const GValue* value, GParamSpec* pspec) {
  MyWidget* self = MY_WIDGET(object);
  
  switch (prop_id) {
  case NAME:
    my_widget_set_name (self, g_value_get_string (value));
    break;
  case ORIENTATION:
    _my_widget_set_orientation (self, g_value_get_enum (value), pspec);
    break;    
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
my_widget_get_prop (GObject* object, guint prop_id,
                    GValue* value, GParamSpec* pspec) {
  MyWidget* self = MY_WIDGET(object);
  MyWidgetPrivate* priv = my_widget_get_instance_private (self);
  MyWidgetLayout* layout =
    MY_WIDGET_LAYOUT(gtk_widget_get_layout_manager (GTK_WIDGET(self)));
  
  switch (prop_id) {
  case NAME:
    g_value_set_string (value, priv->name);
    break;
  case ORIENTATION:
    g_value_set_enum(value,
                     gtk_orientable_get_orientation (GTK_ORIENTABLE(layout)));
    break;    
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }  
}

static void
my_widget_dispose (GObject* object) {
  GtkWidget* child;
  
  while((child =
         gtk_widget_get_first_child (GTK_WIDGET(object))))
    gtk_widget_unparent (child);
  
  G_OBJECT_CLASS(my_widget_parent_class)->dispose (object);
}

static void
my_widget_finalize (GObject* object) {
  MyWidget* widget = MY_WIDGET(object);
  MyWidgetPrivate* priv = my_widget_get_instance_private (widget);

  g_free (priv->name);
  
  G_OBJECT_CLASS(my_widget_parent_class)->finalize (object);  
}

static void
my_widget_buildable_add_child (GtkBuildable* buildable, GtkBuilder* builder,
                               GObject* child, const gchar* type) {
  if (GTK_IS_WIDGET(child))
    my_widget_add (MY_WIDGET(buildable), GTK_WIDGET(child));
  else
    my_widget_parent_buildable_iface->
      add_child (buildable, builder, child, type);
}

static void
my_widget_buildable_iface_init (GtkBuildableIface* iface) {
  my_widget_parent_buildable_iface =
    g_type_interface_peek_parent (iface);

  iface->add_child = my_widget_buildable_add_child;
}

static void
my_widget_base_init(gpointer g_class) {
  MyWidgetClass* klass = g_class;
  MyWidgetClassPrivate* priv;
  
  priv = klass->priv =
    G_TYPE_CLASS_GET_PRIVATE(g_class, MY_TYPE_WIDGET, MyWidgetClassPrivate);

  priv->activate_sig = 0;
}

static void
my_widget_base_finalize (gpointer g_class) {}

static void
my_widget_class_init (gpointer g_class, gpointer class_data) {
  GObjectClass* oklass = G_OBJECT_CLASS(g_class);
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(g_class);
  MyWidgetClass* klass = MY_WIDGET_CLASS(g_class);
  
  g_type_class_adjust_private_offset (klass, &MyWidget_private_offset);  
  my_widget_parent_class = g_type_class_peek_parent (klass);
  
  oklass->dispose = my_widget_dispose;
  oklass->finalize = my_widget_finalize;
  oklass->set_property = my_widget_set_prop;
  oklass->get_property = my_widget_get_prop;
  
  props[NAME] = g_param_spec_string ("name", "name", NULL, NULL,
                                     G_PARAM_READWRITE|G_PARAM_EXPLICIT_NOTIFY);
  
  g_object_class_install_properties (oklass, N_PROPS, props);
  g_object_class_override_property (oklass, ORIENTATION, "orientation");
  
  gtk_widget_class_set_layout_manager_type(wklass, MY_TYPE_WIDGET_LAYOUT);

  /* gtk_css_provider_load_from_resource(provider, "widget.css"); */
  /* gtk_widget_class_set_css_name(); */
  
  gtk_widget_class_set_template_from_resource (wklass, "/my/app/widget.ui");
  gtk_widget_class_bind_template_child_private (wklass, MyWidget, info);
  gtk_widget_class_bind_template_child_private (wklass, MyWidget, area);
  gtk_widget_class_bind_template_child_private (wklass, MyWidget, op);
  gtk_widget_class_bind_template_child_private (wklass, MyWidget, comment);
  gtk_widget_class_bind_template_child_private (wklass, MyWidget, stats);
}

static void
my_widget_init (GTypeInstance* instance, gpointer g_class) {
  MyWidget* self = MY_WIDGET(instance);
  MyWidgetPrivate* priv;

  priv = self->priv = my_widget_get_instance_private (self);
  priv->name = NULL;
  
  g_type_ensure (MY_TYPE_INFO);
  g_type_ensure (MY_TYPE_AREA);
  g_type_ensure (MY_TYPE_OP);
  g_type_ensure (MY_TYPE_COMMENT);
  g_type_ensure (MY_TYPE_STATS);
  
  gtk_widget_init_template (GTK_WIDGET(self));
}

GtkWidget*
my_widget_new (GtkOrientation orientation, gint spacing) {
  return g_object_new (MY_TYPE_WIDGET,
                       "orientation", orientation, NULL);
}

void
my_widget_add (MyWidget* widget, GtkWidget* child) {
  g_return_if_fail (MY_IS_WIDGET(widget));
  g_return_if_fail (GTK_IS_WIDGET(child));
  g_return_if_fail (gtk_widget_get_parent (child) == NULL);
  
  gtk_widget_insert_before (child, GTK_WIDGET(widget), NULL);  
}

void
my_widget_remove (MyWidget* widget, GtkWidget* child) {
  g_return_if_fail (MY_IS_WIDGET(widget));
  g_return_if_fail (GTK_IS_WIDGET(child));
  g_return_if_fail (gtk_widget_get_parent (child) == GTK_WIDGET(widget));
  
  gtk_widget_unparent (child);
}

void
my_widget_set_name (MyWidget* widget, const gchar* name) {
  MyWidgetPrivate* priv = my_widget_get_instance_private (widget);

  g_return_if_fail (MY_IS_WIDGET(widget));

  g_free (priv->name);
  priv->name = g_strdup (name);

  g_object_notify_by_pspec (G_OBJECT(widget), props[NAME]);
}

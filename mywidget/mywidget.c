#include "mywidgetprivate.h"

enum {
  ACTIVATE,
  N_SIGS
};
enum {
  PROP_0,
  NAME,
  LAYOUT,
  N_PROPS
  /* overridden props */
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

static gint my_widget_private_offset = 0;
static gpointer my_widget_parent_class = NULL;
static guint my_sigs[N_SIGS] = {0};
static GParamSpec* my_props[N_PROPS] = {NULL};

static void my_widget_base_init(gpointer g_class);
static void my_widget_base_finalize(gpointer g_class);
static void my_widget_class_init(gpointer g_class, gpointer class_data);
static void my_widget_init(GTypeInstance* instance, gpointer g_class);
static void my_widget_dispose(GObject* object);
static void my_widget_finalize(GObject *object);
static void my_widget_set_prop(GObject* object, guint prop_id, const GValue* value, GParamSpec* pspec);
static void my_widget_get_prop(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec);

GType
my_widget_get_type() {
  static GType type = 0;
  if(G_UNLIKELY(type == 0)) {
    const GTypeInfo type_info = {
      sizeof(MyWidgetClass)      /* class_size */,
      my_widget_base_init        /* base_init */,
      my_widget_base_finalize    /* base_finalize */,
      my_widget_class_init       /* class_init */,
      NULL                       /* class_finalize */,
      NULL                       /* class_data */,
      sizeof(MyWidget)           /* instance_size */,
      0                          /* n_preallocs */,
      my_widget_init             /* instance_init */,
      NULL                       /* value_table */
    };
    const GInterfaceInfo iface_info = {
      /* GInterfaceInitFunc */
      /* GInterfaceFinalizeFunc */
      /* iface data */
    };
    
    type = g_type_register_static(GTK_TYPE_WIDGET,
                                  g_intern_static_string("MyWidget"),
                                  &type_info,
                                  (GTypeFlags)0);
 
    g_type_add_class_private(type, sizeof(MyWidgetClassPrivate));
    my_widget_private_offset = g_type_add_instance_private(type, sizeof(MyWidgetPrivate));
    /* g_type_add_interface_static(type, iface_gtype, &iface_info) */
  }
  return type;
}

static inline gpointer
my_widget_get_instance_private(MyWidget* widget) {
  return (G_STRUCT_MEMBER_P(widget, my_widget_private_offset));  
}

static void
my_widget_set_prop(GObject* object, guint prop_id,
                   const GValue* value, GParamSpec* pspec) {
  MyWidget* self = MY_WIDGET(object);
  MyWidgetPrivate* priv = my_widget_get_instance_private(self);

  switch(prop_id) {
  case NAME:
    g_free(priv->name);
    priv->name = g_value_dup_string(value);
    break;
  case LAYOUT:
    priv->layout = g_value_get_boolean(value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }
}

static void
my_widget_get_prop(GObject* object, guint prop_id,
                   GValue* value, GParamSpec* pspec) {
  MyWidget* self = MY_WIDGET(object);
  MyWidgetPrivate* priv = my_widget_get_instance_private(self);

  switch(prop_id) {
  case NAME:
    g_value_set_string(value, priv->name);
    break;
  case LAYOUT:
    g_value_set_boolean(value, priv->layout);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    break;
  }  
}

static void
my_widget_dispose(GObject* object) {
  MyWidget* widget = MY_WIDGET(object);
  MyWidgetClass* klass = MY_WIDGET_GET_CLASS(widget);
  MyWidgetPrivate* priv = my_widget_get_instance_private(widget);
   
  G_OBJECT_CLASS(my_widget_parent_class)->dispose(object);
}

static void
my_widget_finalize(GObject* object) {
  MyWidget* widget = MY_WIDGET(object);
  MyWidgetClass* klass = MY_WIDGET_GET_CLASS(widget);
  MyWidgetPrivate* priv = my_widget_get_instance_private(widget);

  g_free(priv->name);
  G_OBJECT_CLASS(my_widget_parent_class)->finalize(object);  
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
my_widget_base_finalize(gpointer g_class) {}

static void
my_widget_class_init(gpointer g_class, gpointer class_data) {
  GObjectClass* gklass = G_OBJECT_CLASS(g_class);
  GtkWidgetClass* wklass = GTK_WIDGET_CLASS(g_class);
  MyWidgetClass* klass = MY_WIDGET_CLASS(g_class);
  
  g_type_class_adjust_private_offset(klass, &my_widget_private_offset);  
  my_widget_parent_class = g_type_class_peek_parent(klass);

  gklass->dispose = my_widget_dispose;
  gklass->finalize = my_widget_finalize;
  gklass->set_property = my_widget_set_prop;
  gklass->get_property = my_widget_get_prop;

  my_props[NAME] = g_param_spec_string("name", "name", NULL, NULL,
                                       G_PARAM_READWRITE);
  my_props[LAYOUT] = g_param_spec_boolean("layout", "layout", NULL, FALSE,
                                          G_PARAM_READWRITE);
  g_object_class_install_properties(gklass, N_PROPS, my_props);
}

static void
my_widget_init(GTypeInstance* instance, gpointer g_class) {
  MyWidget* self = MY_WIDGET(instance);
  MyWidgetPrivate* priv;

  priv = self->priv = my_widget_get_instance_private(self);

  priv->layout = FALSE;
  priv->name = NULL;
}

GtkWidget*
my_widget_new() {
  return g_object_new(MY_TYPE_WIDGET, NULL);
}

void
get_data(MyWidget* widget) {
  g_print("%s, %d\n", widget->priv->name, widget->priv->layout);
}

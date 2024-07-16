G_BEGIN_DECLS
typedef gsize GType;
typedef struct _GValue GValue;
typedef union _GTypeCValue GTypeCValue;
typedef struct _GTypePlugin GTypePlugin;
typedef struct _GTypeClass GTypeClass;
typedef struct _GTypeInterface GTypeInterface;
typedef struct _GTypeInstance GTypeInstance;
typedef struct _GTypeInfo GTypeInfo;
typedef struct _GTypeFundamentalInfo GTypeFundamentalInfo;
typedef struct _GInterfaceInfo GInterfaceInfo;
typedef struct _GTypeValueTable GTypeValueTable;
typedef struct _GTypeQuery GTypeQuery;

struct _GTypeClass
{
  GType g_type;
};
struct _GTypeInstance
{
  GTypeClass *g_class;
};
struct _GTypeInterface
{
  GType g_type;
  GType g_instance_type;
};
struct _GTypeQuery
{
  GType type;
  const gchar *type_name;
  guint class_size;
  guint instance_size;
};
typedef enum
{
  G_TYPE_DEBUG_NONE = 0,
  G_TYPE_DEBUG_OBJECTS = 1 << 0,
  G_TYPE_DEBUG_SIGNALS = 1 << 1,
  G_TYPE_DEBUG_INSTANCE_COUNT = 1 << 2,
  G_TYPE_DEBUG_MASK = 0x07
} GTypeDebugFlags GLIB_DEPRECATED_TYPE_IN_2_36;


G_GNUC_BEGIN_IGNORE_DEPRECATIONS
GLIB_DEPRECATED_IN_2_36
void g_type_init (void);
GLIB_DEPRECATED_IN_2_36
void g_type_init_with_debug_flags (GTypeDebugFlags debug_flags);
G_GNUC_END_IGNORE_DEPRECATIONS
GLIB_AVAILABLE_IN_ALL
const gchar * g_type_name (GType type);
GLIB_AVAILABLE_IN_ALL
GQuark g_type_qname (GType type);
GLIB_AVAILABLE_IN_ALL
GType g_type_from_name (const gchar *name);
GLIB_AVAILABLE_IN_ALL
GType g_type_parent (GType type);
GLIB_AVAILABLE_IN_ALL
guint g_type_depth (GType type);
GLIB_AVAILABLE_IN_ALL
GType g_type_next_base (GType leaf_type,
            GType root_type);
GLIB_AVAILABLE_IN_ALL
gboolean g_type_is_a (GType type,
            GType is_a_type);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_class_ref (GType type);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_class_peek (GType type);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_class_peek_static (GType type);
GLIB_AVAILABLE_IN_ALL
void g_type_class_unref (gpointer g_class);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_class_peek_parent (gpointer g_class);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_interface_peek (gpointer instance_class,
            GType iface_type);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_interface_peek_parent (gpointer g_iface);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_default_interface_ref (GType g_type);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_default_interface_peek (GType g_type);
GLIB_AVAILABLE_IN_ALL
void g_type_default_interface_unref (gpointer g_iface);
GLIB_AVAILABLE_IN_ALL
GType* g_type_children (GType type,
            guint *n_children);
GLIB_AVAILABLE_IN_ALL
GType* g_type_interfaces (GType type,
            guint *n_interfaces);
GLIB_AVAILABLE_IN_ALL
void g_type_set_qdata (GType type,
            GQuark quark,
            gpointer data);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_get_qdata (GType type,
            GQuark quark);
GLIB_AVAILABLE_IN_ALL
void g_type_query (GType type,
            GTypeQuery *query);
GLIB_AVAILABLE_IN_2_44
int g_type_get_instance_count (GType type);

typedef void (*GBaseInitFunc) (gpointer g_class);
typedef void (*GBaseFinalizeFunc) (gpointer g_class);
typedef void (*GClassInitFunc) (gpointer g_class,
           gpointer class_data);
typedef void (*GClassFinalizeFunc) (gpointer g_class,
           gpointer class_data);
typedef void (*GInstanceInitFunc) (GTypeInstance *instance,
           gpointer g_class);
typedef void (*GInterfaceInitFunc) (gpointer g_iface,
           gpointer iface_data);
typedef void (*GInterfaceFinalizeFunc) (gpointer g_iface,
           gpointer iface_data);
typedef gboolean (*GTypeClassCacheFunc) (gpointer cache_data,
           GTypeClass *g_class);
typedef void (*GTypeInterfaceCheckFunc) (gpointer check_data,
           gpointer g_iface);

typedef enum
{
  G_TYPE_FLAG_CLASSED = (1 << 0),
  G_TYPE_FLAG_INSTANTIATABLE = (1 << 1),
  G_TYPE_FLAG_DERIVABLE = (1 << 2),
  G_TYPE_FLAG_DEEP_DERIVABLE = (1 << 3)
} GTypeFundamentalFlags;
typedef enum
{
  G_TYPE_FLAG_ABSTRACT = (1 << 4),
  G_TYPE_FLAG_VALUE_ABSTRACT = (1 << 5)
} GTypeFlags;
struct _GTypeInfo
{
  guint16 class_size;
  GBaseInitFunc base_init;
  GBaseFinalizeFunc base_finalize;
  GClassInitFunc class_init;
  GClassFinalizeFunc class_finalize;
  gconstpointer class_data;
  guint16 instance_size;
  guint16 n_preallocs;
  GInstanceInitFunc instance_init;
  const GTypeValueTable *value_table;
};
struct _GTypeFundamentalInfo
{
  GTypeFundamentalFlags type_flags;
};
struct _GInterfaceInfo
{
  GInterfaceInitFunc interface_init;
  GInterfaceFinalizeFunc interface_finalize;
  gpointer interface_data;
};
struct _GTypeValueTable
{
  void (*value_init) (GValue *value);
  void (*value_free) (GValue *value);
  void (*value_copy) (const GValue *src_value,
      GValue *dest_value);
  gpointer (*value_peek_pointer) (const GValue *value);
  const gchar *collect_format;
  gchar* (*collect_value) (GValue *value,
      guint n_collect_values,
      GTypeCValue *collect_values,
      guint collect_flags);
  const gchar *lcopy_format;
  gchar* (*lcopy_value) (const GValue *value,
      guint n_collect_values,
      GTypeCValue *collect_values,
      guint collect_flags);
};

GLIB_AVAILABLE_IN_ALL
GType g_type_register_static (GType parent_type,
      const gchar *type_name,
      const GTypeInfo *info,
      GTypeFlags flags);
GLIB_AVAILABLE_IN_ALL
GType g_type_register_static_simple (GType parent_type,
      const gchar *type_name,
      guint class_size,
      GClassInitFunc class_init,
      guint instance_size,
      GInstanceInitFunc instance_init,
      GTypeFlags flags);
GLIB_AVAILABLE_IN_ALL
GType g_type_register_dynamic (GType parent_type,
      const gchar *type_name,
      GTypePlugin *plugin,
      GTypeFlags flags);
GLIB_AVAILABLE_IN_ALL
GType g_type_register_fundamental (GType type_id,
      const gchar *type_name,
      const GTypeInfo *info,
      const GTypeFundamentalInfo *finfo,
      GTypeFlags flags);
GLIB_AVAILABLE_IN_ALL
void g_type_add_interface_static (GType instance_type,
      GType interface_type,
      const GInterfaceInfo *info);
GLIB_AVAILABLE_IN_ALL
void g_type_add_interface_dynamic (GType instance_type,
      GType interface_type,
      GTypePlugin *plugin);
GLIB_AVAILABLE_IN_ALL
void g_type_interface_add_prerequisite (GType interface_type,
      GType prerequisite_type);
GLIB_AVAILABLE_IN_ALL
GType*g_type_interface_prerequisites (GType interface_type,
      guint *n_prerequisites);
GLIB_DEPRECATED_IN_2_58
void g_type_class_add_private (gpointer g_class,
                                         gsize private_size);
GLIB_AVAILABLE_IN_2_38
gint g_type_add_instance_private (GType class_type,
                                         gsize private_size);
GLIB_AVAILABLE_IN_ALL
                                           gpointer g_type_instance_get_private (GTypeInstance *instance,
                                         GType private_type);
GLIB_AVAILABLE_IN_2_38
void g_type_class_adjust_private_offset (gpointer g_class,
                                             gint *private_size_or_offset);
GLIB_AVAILABLE_IN_ALL
void g_type_add_class_private (GType class_type,
      gsize private_size);
GLIB_AVAILABLE_IN_ALL
gpointer g_type_class_get_private (GTypeClass *klass,
      GType private_type);
GLIB_AVAILABLE_IN_2_38
gint g_type_class_get_instance_private_offset (gpointer g_class);
GLIB_AVAILABLE_IN_2_34
void g_type_ensure (GType type);
GLIB_AVAILABLE_IN_2_36
guint g_type_get_type_registration_serial (void);
GLIB_AVAILABLE_IN_ALL
GTypePlugin* g_type_get_plugin (GType type);
GLIB_AVAILABLE_IN_ALL
GTypePlugin* g_type_interface_get_plugin (GType instance_type,
       GType interface_type);
GLIB_AVAILABLE_IN_ALL
GType g_type_fundamental_next (void);
GLIB_AVAILABLE_IN_ALL
GType g_type_fundamental (GType type_id);
GLIB_AVAILABLE_IN_ALL
GTypeInstance* g_type_create_instance (GType type);
GLIB_AVAILABLE_IN_ALL
void g_type_free_instance (GTypeInstance *instance);
GLIB_AVAILABLE_IN_ALL
void g_type_add_class_cache_func (gpointer cache_data,
       GTypeClassCacheFunc cache_func);
GLIB_AVAILABLE_IN_ALL
void g_type_remove_class_cache_func (gpointer cache_data,
       GTypeClassCacheFunc cache_func);
GLIB_AVAILABLE_IN_ALL
void g_type_class_unref_uncached (gpointer g_class);
GLIB_AVAILABLE_IN_ALL
void g_type_add_interface_check (gpointer check_data,
       GTypeInterfaceCheckFunc check_func);
GLIB_AVAILABLE_IN_ALL
void g_type_remove_interface_check (gpointer check_data,
       GTypeInterfaceCheckFunc check_func);
GLIB_AVAILABLE_IN_ALL
GTypeValueTable* g_type_value_table_peek (GType type);
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_instance (GTypeInstance *instance) G_GNUC_PURE;
GLIB_AVAILABLE_IN_ALL
GTypeInstance* g_type_check_instance_cast (GTypeInstance *instance,
       GType iface_type);
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_instance_is_a (GTypeInstance *instance,
       GType iface_type) G_GNUC_PURE;
GLIB_AVAILABLE_IN_2_42
gboolean g_type_check_instance_is_fundamentally_a (GTypeInstance *instance,
                                                           GType fundamental_type) G_GNUC_PURE;
GLIB_AVAILABLE_IN_ALL
GTypeClass* g_type_check_class_cast (GTypeClass *g_class,
       GType is_a_type);
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_class_is_a (GTypeClass *g_class,
       GType is_a_type) G_GNUC_PURE;
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_is_value_type (GType type) G_GNUC_CONST;
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_value (const GValue *value) G_GNUC_PURE;
GLIB_AVAILABLE_IN_ALL
gboolean g_type_check_value_holds (const GValue *value,
       GType type) G_GNUC_PURE;
GLIB_AVAILABLE_IN_ALL
gboolean g_type_test_flags (GType type,
       guint flags) G_GNUC_CONST;
GLIB_AVAILABLE_IN_ALL
const gchar * g_type_name_from_instance (GTypeInstance *instance);
GLIB_AVAILABLE_IN_ALL
const gchar * g_type_name_from_class (GTypeClass *g_class);
G_END_DECLS

static void my_widget_init(MyWidget *self);
static void my_widget_class_init(MyWidgetClass *klass);
static GType my_widget_get_type_once(void);
static gpointer my_widget_parent_class = NULL;
static gint MyWidget_private_offset;

static void my_widget_class_intern_init(gpointer klass) {
  my_widget_parent_class = g_type_class_peek_parent(klass);
  if (MyWidget_private_offset != 0)
    g_type_class_adjust_private_offset(klass, &MyWidget_private_offset);
  my_widget_class_init((MyWidgetClass *)klass);
}

static inline gpointer my_widget_get_instance_private(
    MyWidget *self) {
  return (G_STRUCT_MEMBER_P(self, MyWidget_private_offset));
}

GType my_widget_get_type(void) {
  static volatile gsize g_define_type_id__volatile = 0;
  if (g_once_init_enter(&g_define_type_id__volatile)) {
    GType g_define_type_id = my_widget_get_type_once();
    g_once_init_leave(&g_define_type_id__volatile, g_define_type_id);
  }
  return g_define_type_id__volatile;
}
G_GNUC_NO_INLINE static GType my_widget_get_type_once(void) {
  GType g_define_type_id = g_type_register_static_simple(
      GTK_TYPE_WIDGET, g_intern_static_string("MyWidget"),
      sizeof(MyWidgetClass),
      (GClassInitFunc)(void (*)(void))my_widget_class_intern_init,
      sizeof(MyWidget), (GInstanceInitFunc)(void (*)(void))my_widget_init,
      (GTypeFlags)0);
  {
    {
      {
        MyWidget_private_offset = g_type_add_instance_private(
            g_define_type_id, sizeof(MyWidgetPrivate));
      }
      {
        const GInterfaceInfo g_implement_interface_info = {
            (GInterfaceInitFunc)(void (*)(void))my_widget_buildable_iface_init,
            NULL, NULL};
        g_type_add_interface_static(g_define_type_id, GTK_TYPE_BUILDABLE,
                                    &g_implement_interface_info);
      };
    }
  }
  return g_define_type_id;
};

G_DECLARE_DERIVABLE_TYPE (GtkFrob, gtk_frob, GTK, FROB, GtkWidget);

GType gtk_frob_get_type(void);

G_GNUC_BEGIN_IGNORE_DEPRECATIONS
typedef struct _GtkFrob GtkFrob;
typedef struct _GtkFrobClass GtkFrobClass;

struct _GtkFrob {
  GtkWidget parent_instance;
};

_GLIB_DEFINE_AUTOPTR_CHAINUP(GtkFrob, GtkWidget);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(GtkFrobClass, g_type_class_unref);

G_GNUC_UNUSED static inline GtkFrob * GTK_FROB(gpointer ptr) {
  return (((GtkFrob * ) g_type_check_instance_cast((GTypeInstance * )(ptr), (gtk_frob_get_type()))));
}
G_GNUC_UNUSED static inline GtkFrobClass * GTK_FROB_CLASS(gpointer ptr) {
  return (((GtkFrobClass * ) g_type_check_class_cast((GTypeClass * )(ptr), (gtk_frob_get_type()))));
}
G_GNUC_UNUSED static inline gboolean GTK_IS_FROB(gpointer ptr) {
  return ((G_GNUC_EXTENSION({
    GTypeInstance * __inst = (GTypeInstance * )(ptr);GType __t = (gtk_frob_get_type());gboolean __r;
    if (!__inst) __r = FALSE;
    else if (__inst -> g_class && __inst -> g_class -> g_type == __t) __r = TRUE;
    else __r = g_type_check_instance_is_a(__inst, __t);__r;
  })));
}
G_GNUC_UNUSED static inline gboolean GTK_IS_FROB_CLASS(gpointer ptr) {
  return ((G_GNUC_EXTENSION({
    GTypeClass * __class = (GTypeClass * )(ptr);GType __t = (gtk_frob_get_type());gboolean __r;
    if (!__class) __r = FALSE;
    else if (__class -> g_type == __t) __r = TRUE;
    else __r = g_type_check_class_is_a(__class, __t);__r;
  })));
}
G_GNUC_UNUSED static inline GtkFrobClass * GTK_FROB_GET_CLASS(gpointer ptr) {
  return (((GtkFrobClass * )(((GTypeInstance * )(ptr)) -> g_class)));
}
G_GNUC_END_IGNORE_DEPRECATIONS

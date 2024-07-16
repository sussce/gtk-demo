G_DEFINE_TYPE_WITH_CODE (GtkFrob, gtk_frob, GTK_TYPE_WIDGET,
                         G_ADD_PRIVATE (GtkFrob)
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                gtk_frob_buildable_iface_init));

static void gtk_frob_init(GtkFrob * self);
static void gtk_frob_class_init(GtkFrobClass * klass);
static GType gtk_frob_get_type_once(void);

static gpointer gtk_frob_parent_class = NULL;
static gint GtkFrob_private_offset;

static void gtk_frob_class_intern_init(gpointer klass) {
  gtk_frob_parent_class = g_type_class_peek_parent(klass);
  if (GtkFrob_private_offset != 0) g_type_class_adjust_private_offset(klass, & GtkFrob_private_offset);
  gtk_frob_class_init((GtkFrobClass * ) klass);
}
G_GNUC_UNUSED static inline gpointer gtk_frob_get_instance_private(GtkFrob * self) {
  return (G_STRUCT_MEMBER_P(self, GtkFrob_private_offset));
}
GType gtk_frob_get_type(void) {
  static volatile gsize g_define_type_id__volatile = 0;
  if (g_once_init_enter( & g_define_type_id__volatile)) {
    GType g_define_type_id = gtk_frob_get_type_once();
    g_once_init_leave( & g_define_type_id__volatile, g_define_type_id);
  }
  return g_define_type_id__volatile;
}
G_GNUC_NO_INLINE static GType gtk_frob_get_type_once(void) {
  GType g_define_type_id = g_type_register_static_simple(GTK_TYPE_WIDGET, g_intern_static_string("GtkFrob"), sizeof(GtkFrobClass), (GClassInitFunc)(void( * )(void)) gtk_frob_class_intern_init, sizeof(GtkFrob), (GInstanceInitFunc)(void( * )(void)) gtk_frob_init, (GTypeFlags) 0);
  {
    {
      {
        GtkFrob_private_offset = g_type_add_instance_private(g_define_type_id, sizeof(GtkFrobPrivate));
      } {
        const GInterfaceInfo g_implement_interface_info = {
          (GInterfaceInitFunc)(void( * )(void)) gtk_frob_buildable_iface_init,
          NULL,
          NULL
        };
        g_type_add_interface_static(g_define_type_id, GTK_TYPE_BUILDABLE, & g_implement_interface_info);
      };
    }
  }
  return g_define_type_id;
}

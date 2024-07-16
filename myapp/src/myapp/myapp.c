#include "myapp/myapp.h"
#include "mywindow/mywindow.h"

#define MY_APP_ID "my.app"

struct _MyApp {
  GtkApplication parent_instance;
};

G_DEFINE_TYPE(MyApp, my_app, GTK_TYPE_APPLICATION);

static void
new_action (GSimpleAction* action, GVariant* param,
            gpointer app) {}

static void
open_action (GSimpleAction* action, GVariant* param,
             gpointer app) {}

static void
save_action (GSimpleAction* action, GVariant* param,
             gpointer app) {}

static void
setting_action (GSimpleAction* action, GVariant* param,
                gpointer app) {}

static void
quit_action (GSimpleAction* action, GVariant* param,
             gpointer app) {
  g_application_quit (G_APPLICATION(app));
}

static void
my_app_activate (GApplication* app) {
  GtkWidget* window;

  window = my_window_new (MY_APP(app));
  gtk_window_present (GTK_WINDOW(window));
}

static void
my_app_open (GApplication* app, GFile** files,
             gint n_files, const gchar* hint) {
  GList* windows;
  GtkWidget* window;
    
  windows = gtk_application_get_windows (GTK_APPLICATION(app));
  if (windows) {
    window = GTK_WIDGET(windows->data);
  }
  else {
    window = my_window_new (MY_APP(app));
  }

  for (int i = 0; i < n_files; i++) {
    my_window_open (MY_WINDOW(window), files[i]);
  }

  gtk_window_present (GTK_WINDOW(window));
}

static GActionEntry app_entries[] = {
  {"new", new_action, NULL, NULL, NULL},
  {"open", open_action, NULL, NULL, NULL},
  {"save", save_action, NULL, NULL, NULL},    
  {"setting", setting_action, NULL, NULL, NULL},
  {"quit", quit_action, NULL, NULL, NULL}
};

static void
my_app_startup (GApplication* app) {
  const char* new_accels[2] = {"<ctrl>n", NULL};
  const char* open_accels[2] = {"<ctrl>o", NULL};
  const char* save_accels[2] = {"<ctrl>s", NULL};
  const char* quit_accels[2] = {"<ctrl>q", NULL};
  
  G_APPLICATION_CLASS(my_app_parent_class)->startup (app);

  g_action_map_add_action_entries (G_ACTION_MAP(app),
                                   app_entries,
                                   G_N_ELEMENTS(app_entries),
                                   app);

  gtk_application_set_accels_for_action (GTK_APPLICATION(app),
                                         "app.new",
                                         new_accels);

  gtk_application_set_accels_for_action (GTK_APPLICATION(app),
                                         "app.open",
                                         open_accels);

  gtk_application_set_accels_for_action (GTK_APPLICATION(app),
                                         "app.save",
                                         save_accels);
  
  gtk_application_set_accels_for_action (GTK_APPLICATION(app),
                                         "app.quit",
                                         quit_accels);
}

static void
my_app_class_init (MyAppClass* klass) {
  GApplicationClass* aklass = G_APPLICATION_CLASS(klass);

  aklass->startup = my_app_startup;
  aklass->activate = my_app_activate;
  aklass->open = my_app_open;
}

static void
my_app_init (MyApp* self) {}

MyApp*
my_app_new () {
  return g_object_new (MY_TYPE_APP,
                       "application-id", MY_APP_ID,
                       "flags", G_APPLICATION_DEFAULT_FLAGS|G_APPLICATION_HANDLES_OPEN,
                       NULL);
}

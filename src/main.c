#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <stdio.h>
#include "serial.h"
#include "gcode.h"

static float num = 99;
static int serial_port;
static char read_buf [256];
static int* len;
static int destination [2];
static int position [2];
static int config [2];

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World: %f\n",num);
}

static void bt2(GtkWidget *widget, gpointer data) {
  g_print("%d\n",config[0]);
}

static void gcode_2(GtkWidget *widget, gpointer data) {
  g_print("gcode2 called\n");
  unsigned char msg[] = {'b','\n'};
  ser_msg(msg, 2, serial_port);
}

static void chang(GtkAdjustment *widget, gpointer data) {
  float value = gtk_adjustment_get_value(widget);
  g_print("%f\n",value);
  num = value;
}

gboolean ReadSerial(void* data) {
  ser_read(read_buf, len, serial_port);
  return TRUE;
}

static void quit_cb (GtkWindow *window) {
  gtk_window_close (window);
}

static void activate (GtkApplication *app, gpointer user_data) {
  /* Construct a GtkBuilder instance and load our UI description */
  GtkBuilder *builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "builder.ui", NULL);

  /* Connect signal handlers to the constructed widgets. */
  GObject *window = gtk_builder_get_object (builder, "window");
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (bt2), NULL);

  GObject *button2 = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button2, "clicked", G_CALLBACK (gcode_2), NULL);

  // GObject *button3 = gtk_builder_get_object (builder, "button3");
  // g_signal_connect (button3, "clicked", G_CALLBACK (ReadSerial), NULL);

  GObject *adj = gtk_builder_get_object (builder, "adjustment2b");
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  GString* string;
  g_timeout_add(500, ReadSerial, string);

  /* We do not need the builder any more */
  g_object_unref (builder);
}

int
main (int argc, char *argv[])
{
#ifdef GTK_SRCDIR
  g_chdir (GTK_SRCDIR);
#endif

  position[0] = 0;
  position[1] = 0;
  destination[0] = 0;
  destination[1] = 0;
  config[0] = 0;
  config[1] = 0;


  serial_port = setup();
  unsigned char msg[] = {'T','e','s','t','\n'}; 
  ser_msg(msg, 5, serial_port);
  ser_read(read_buf, len, serial_port);

  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);


  return status;
}

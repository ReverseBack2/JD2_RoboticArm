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
static int vars [4] = {45,45,45,45};
static int numb [8] = {0,1,2,3,4,5,6,7};


static void bt2(GtkWidget *widget, gpointer data) {
  g_print("%d\n",config[0]);
}

static void Gcode(GtkWidget *widget, gpointer data) {
  int* dat = data;
  switch (*dat) {
    case 0:
      g_print("G0\n");
      destination[0] = vars[0];
      destination[1] = vars[1];
      G0(destination, position, config, serial_port);
      break;
    case 1:
      g_print("G1\n");
      destination[0] = vars[2];
      destination[1] = vars[3];
      G1(destination, position, config, serial_port);
      break;
    case 2:
      g_print("G90\n");
      G90(config, serial_port);
      break;
    case 3:
      g_print("G91\n");
      G91(config, serial_port);
      break;
    case 4:
      g_print("G20\n");
      G20(config, serial_port);
      break;
    case 5:
      g_print("G21\n");
      G21(config, serial_port);
      break;
    case 6:
      g_print("M2\n");
      M2(position, serial_port);
      break;
    case 7:
      g_print("M6\n");
      M6(serial_port);
      break;
    default:
      g_print("not one\n");
  }
}

static void chang(GtkAdjustment *widget, gpointer data) {
  int* dat = data;
  int value = ((int)gtk_adjustment_get_value(widget))%181;
  // g_print("%d\n",value);
  vars[*dat] = value;
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

  gpointer data0 = &(numb[0]);
  gpointer data1 = &(numb[1]);
  gpointer data2 = &(numb[2]);
  gpointer data3 = &(numb[3]);
  gpointer data4 = &(numb[4]);
  gpointer data5 = &(numb[5]);
  gpointer data6 = &(numb[6]);
  gpointer data7 = &(numb[7]);
  GObject *button = gtk_builder_get_object (builder, "button0");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data0);
  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data1);
  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data2);
  button = gtk_builder_get_object (builder, "button3");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data3);
  button = gtk_builder_get_object (builder, "button4");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data4);
  button = gtk_builder_get_object (builder, "button5");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data5);
  button = gtk_builder_get_object (builder, "button6");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data6);
  button = gtk_builder_get_object (builder, "button7");
  g_signal_connect (button, "clicked", G_CALLBACK (Gcode), data7);

  // Numeric inputs
  GObject *adj = gtk_builder_get_object (builder, "adjustment0a");
  GtkAdjustment *adj2 = (GtkAdjustment*)adj;
  gtk_adjustment_set_value(adj2, 45);
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), data0);
  adj = gtk_builder_get_object (builder, "adjustment0b");
  adj2 = (GtkAdjustment*)adj;
  gtk_adjustment_set_value(adj2, 45);
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), data1);
  adj = gtk_builder_get_object (builder, "adjustment1a");
  adj2 = (GtkAdjustment*)adj;
  gtk_adjustment_set_value(adj2, 45);
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), data2);
  adj = gtk_builder_get_object (builder, "adjustment1b");
  adj2 = (GtkAdjustment*)adj;
  gtk_adjustment_set_value(adj2, 45);
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), data3);

  // Set default servo state
  g_print("G0\n");
  destination[0] = vars[0];
  destination[1] = vars[1];
  G0(destination, position, config, serial_port);


  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  GString* string2;
  g_timeout_add(200, ReadSerial, string2);

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


  serial_port = GCS_setup();
  unsigned char msg[] = {'T','e','s','t','\n'}; 
  ser_msg(msg, 5, serial_port);
  ser_read(read_buf, len, serial_port);

  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);


  return status;
}

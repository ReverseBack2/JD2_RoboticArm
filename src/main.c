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
static int numb [8] = {0,1,2,3,4,5,6,7};

static void
print_hello (GtkWidget *widget, gpointer data) {
  g_print ("Hello World: %f\n",num);
}

static void bt2(GtkWidget *widget, gpointer data) {
  g_print("%d\n",config[0]);
}

static void Gcode(GtkWidget *widget, gpointer data) {
  int* dat = data;
  switch (*dat) {
    case 0:
      g_print("G0\n");
      G0(destination, position, config, serial_port);
      break;
    case 1:
      g_print("G1\n");
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


  GObject *adj = gtk_builder_get_object (builder, "adjustment2b");
  g_signal_connect (adj, "value-changed", G_CALLBACK(chang), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);

  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);

  GString* string2;
  g_timeout_add(500, ReadSerial, string2);

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

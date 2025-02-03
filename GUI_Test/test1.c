//gcc test.c -o p1 `pkg-config --cflags --libs gtk+-2.0`

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static int counter = 0;


void my_callback_function(GtkWidget *widget, gpointer data){

}

void greet(GtkWidget *widget, gpointer data){
	g_print("Welcome to GTK\n");
	g_print("%s clicked %d times\n",(char*)data, ++counter);
}

void destroy(GtkWidget *widget, gpointer data) {
	gtk_main_quit();
}


int main(int argc, char* argv[]) {
	GtkWidget *window;
	GtkWidget *button;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);

	button = gtk_button_new_with_label("BUTTON LICK!");

	g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(greet), "button");
	
	gtk_container_add(GTK_CONTAINER(window), button);

	gtk_widget_show_all(window);
	gtk_main();

}


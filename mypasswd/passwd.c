#include <gtk/gtk.h>
#include "mypasswd.h"


static void destroy(GtkWidget *widget, gpointer user_data)
{
    gtk_main_quit();
}


int main(int argc, char *argv[])
{
    GtkWidget *window, *hbox, *label, *passwd;

    /*  Initialization  */
    gtk_init(&argc, &argv);

    /*  Create a GtkWindow widget  */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Input Password");

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(destroy), NULL);
    
    /*  Create a GtkHbox widget  */
    hbox = gtk_hbox_new(FALSE, 10);

    /*  Create a GtkLabel widget  */
    label = gtk_label_new("Enter new password:");

    /*  Create a MyPasswd widget  */
    passwd = my_passwd_new();
    my_passwd_set_length_limit(MY_PASSWD(passwd), 4, 10);
    
    /*  Add label and passwd to hbox  */
    gtk_box_pack_start(GTK_BOX(hbox), label,  FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), passwd, FALSE, TRUE , 0);

    /*  Add hbox to window  */
    gtk_container_add(GTK_CONTAINER(window), hbox);

    /*  Main loop  */
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}



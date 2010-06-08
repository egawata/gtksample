#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <string.h>

#include "mypasswd.h"

#define MY_PASSWD_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MY_PASSWD_TYPE, MyPasswdPrivate))


typedef struct _MyPasswdPrivate MyPasswdPrivate;

struct _MyPasswdPrivate {
    guint   len_min;
    guint   len_max;
};


/*  Property IDs  */
enum {
    PROP_0,     // dummy
    PROP_LEN_MIN,
    PROP_LEN_MAX
};

/*  Background color for entry (red, green ,blue) */
static gint col_valid[]   = { 32768, 65535, 32768 };
static gint col_invalid[] = { 65535, 32768, 32768 };

/*  Configuration for password entry  */
#define ENTRY_WIDTH             20
#define PASSWD_MIN_LENGTH       6
#define PASSWD_MAX_LENGTH       16
#define PASSWD_INVISIBLE_CHAR   '*'


static void my_passwd_get_property (GObject*, guint, GValue*, GParamSpec*);  
static void my_passwd_set_property (GObject*, guint, const GValue*, GParamSpec*);
static void my_passwd_class_init (MyPasswdClass*);
static void my_passwd_init (MyPasswd *);
static gboolean my_passwd_key_released(GtkWidget*, GdkEventKey*, gpointer);



GType
my_passwd_get_type (void) {
    static GType entry_type = 0;

    if (!entry_type) {
        static const GTypeInfo entry_info = {
            sizeof (MyPasswdClass), 
            NULL,
            NULL,
            (GClassInitFunc) my_passwd_class_init,
            NULL,
            NULL,
            sizeof (MyPasswd),
            0, 
            (GInstanceInitFunc) my_passwd_init,
        };

        entry_type = g_type_register_static (GTK_TYPE_ENTRY, "MyPasswd",
                                             &entry_info, 0);
    }

    return entry_type;
}


static void 
my_passwd_get_property (GObject    *object, 
                        guint      prop_id, 
                        GValue     *value, 
                        GParamSpec *pspec)  
{
    MyPasswd *passwd = MY_PASSWD(object);
    MyPasswdPrivate *priv = MY_PASSWD_GET_PRIVATE(passwd);

    switch (prop_id) {
        case PROP_LEN_MIN:
            g_value_set_int(value, priv->len_min);
            break;
        case PROP_LEN_MAX:
            g_value_set_int(value, priv->len_max);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void 
my_passwd_set_property (GObject      *object, 
                        guint        prop_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
    MyPasswd *passwd = MY_PASSWD(object);
    MyPasswdPrivate *priv = MY_PASSWD_GET_PRIVATE(passwd);

    switch (prop_id) {
        case PROP_LEN_MIN:
            priv->len_min = g_value_get_int(value);
            break;
        case PROP_LEN_MAX:
            priv->len_max = g_value_get_int(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}


static void
my_passwd_class_init(MyPasswdClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->set_property = my_passwd_set_property;
    gobject_class->get_property = my_passwd_get_property;

    g_type_class_add_private (klass, sizeof(MyPasswdPrivate));

    g_object_class_install_property(
            gobject_class, 
            PROP_LEN_MIN,
            g_param_spec_int("len-min",
                             "Minimum length",
                             "Minimum length of valid password",
                             0, 255, PASSWD_MIN_LENGTH,
                             G_PARAM_READWRITE));

    g_object_class_install_property(
            gobject_class, 
            PROP_LEN_MAX,
            g_param_spec_int("len-max",
                             "Maximum length",
                             "Maximum length of valid password",
                             1, 255, PASSWD_MAX_LENGTH,
                             G_PARAM_READWRITE));
}


static void 
my_passwd_init(MyPasswd *passwd)
{
    MyPasswdPrivate *priv = MY_PASSWD_GET_PRIVATE(passwd);

    priv->len_min = PASSWD_MIN_LENGTH;
    priv->len_max = PASSWD_MAX_LENGTH;

    gtk_entry_set_width_chars(GTK_ENTRY(passwd), ENTRY_WIDTH);
    gtk_entry_set_visibility(GTK_ENTRY(passwd), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(passwd), PASSWD_INVISIBLE_CHAR);

    g_signal_connect(G_OBJECT(passwd), "key-release-event",
                     G_CALLBACK(my_passwd_key_released), NULL);
}


static gboolean
my_passwd_key_released(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    MyPasswd *passwd = MY_PASSWD(widget);
    MyPasswdPrivate *priv = MY_PASSWD_GET_PRIVATE(passwd);
    GdkColor color;

    const gchar *text = gtk_entry_get_text(GTK_ENTRY(passwd));
    int len = strlen(text);

    if (priv->len_min <= len && len <= priv->len_max) {
        g_printf("Valid: %s\n", text);
        color.red   = col_valid[0];
        color.green = col_valid[1];
        color.blue  = col_valid[2];
    } else {
        g_printf("Invalid: %s\n", text);
        color.red   = col_invalid[0];
        color.green = col_invalid[1];
        color.blue  = col_invalid[2];
    }

    gtk_widget_modify_base(widget, GTK_WIDGET_STATE(widget), &color);
    
    return TRUE;
}


GtkWidget*
my_passwd_new()
{
    return GTK_WIDGET(g_object_new(my_passwd_get_type(), NULL));
}


void
my_passwd_set_length_limit(MyPasswd *passwd, gint len_min, gint len_max)
{
    MyPasswdPrivate *priv = MY_PASSWD_GET_PRIVATE(passwd);

    if (len_min > len_max) {
        g_printf("Min length must be smaller than max length.\n");
        return;
    }

    if (len_min >= 0) 
        priv->len_min = len_min;

    if (len_max >= 0)
        priv->len_max = len_max;
}




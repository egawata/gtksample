#ifndef __MY_PASSWD_H__
#define __MY_PASSWD_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtkentry.h>

G_BEGIN_DECLS


#define MY_PASSWD_TYPE            (my_passwd_get_type ())
#define MY_PASSWD(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MY_PASSWD_TYPE, MyPasswd))
#define MY_PASSWD_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), MY_PASSWD_TYPE, MyPasswdClass))    
#define IS_MY_PASSWD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MY_PASSWD_TYPE))
#define IS_MY_PASSWD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), MY_PASSWD_TYPE))


typedef struct _MyPasswd          MyPasswd;
typedef struct _MyPasswdClass     MyPasswdClass;

struct _MyPasswd
{
    GtkEntry    entry;
};

struct _MyPasswdClass 
{
    GtkEntryClass parent_class;
};


GType       my_passwd_get_type (void) G_GNUC_CONST;
GtkWidget*  my_passwd_new      (void);
void        my_passwd_set_length_limit (MyPasswd*, gint, gint);


G_END_DECLS

#endif  /*  __MY_PASSWD_H__  */



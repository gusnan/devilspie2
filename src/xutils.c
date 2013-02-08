/**
 *	This file is part of devilspie2
 *	Copyright (C) 2001 Havoc Pennington, 2011-2013 Andreas Rönnquist
 *
 *	devilspie2 is free software: you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as published
 *	by the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	devilspie2 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with devilspie2.
 *	If not, see <http://www.gnu.org/licenses/>.
 */
#include <X11/Xatom.h>

#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <string.h>

#include <locale.h>

#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)


#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include "xutils.h"


#if (GTK_MAJOR_VERSION >= 3)
#define HAVE_GTK3
#endif


static GHashTable *atom_hash = NULL;
static GHashTable *reverse_atom_hash = NULL;


/**
 *
 */
Atom my_wnck_atom_get(const char *atom_name)
{
	Atom retval;

	g_return_val_if_fail (atom_name != NULL, None);

	if (!atom_hash) {
		atom_hash = g_hash_table_new (g_str_hash, g_str_equal);
		reverse_atom_hash = g_hash_table_new (NULL, NULL);
	}

	retval = GPOINTER_TO_UINT (g_hash_table_lookup (atom_hash, atom_name));
	if (!retval) {
		retval = XInternAtom (gdk_x11_get_default_xdisplay(), atom_name, FALSE);

		if (retval != None) {
			char *name_copy;

			name_copy = g_strdup (atom_name);

			g_hash_table_insert (atom_hash,
										name_copy,
										GUINT_TO_POINTER (retval));
			g_hash_table_insert (reverse_atom_hash,
										GUINT_TO_POINTER (retval),
										name_copy);
		}
	}
	return retval;
}


/**
 *
 */
void devilspie2_change_state(Screen *screen, Window xwindow,
							gboolean add,
							Atom     state1,
							Atom     state2)
{
	XEvent xev;

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.display = gdk_x11_get_default_xdisplay();
	xev.xclient.window = xwindow;
	xev.xclient.message_type = my_wnck_atom_get ("_NET_WM_STATE");
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = add ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
	xev.xclient.data.l[1] = state1;
	xev.xclient.data.l[2] = state2;

	XSendEvent (gdk_x11_get_default_xdisplay(),
					RootWindowOfScreen (screen),
					False,
					SubstructureRedirectMask | SubstructureNotifyMask,
					&xev);
}


/**
 *
 */
void devilspie2_error_trap_push()
{
	gdk_error_trap_push();
}


/**
 *
 */
int devilspie2_error_trap_pop()
{
	XSync(gdk_x11_get_default_xdisplay(),False);
	return gdk_error_trap_pop();
}


/**
 *
 */
static void set_decorations(Window xid /*WnckWindow *window*/, gboolean decorate)
{
#define PROP_MOTIF_WM_HINTS_ELEMENTS 5
#define MWM_HINTS_DECORATIONS (1L << 1)
	struct {
		unsigned long flags;
		unsigned long functions;
		unsigned long decorations;
		long inputMode;
		unsigned long status;
	} hints = {0,};

	hints.flags = MWM_HINTS_DECORATIONS;
	hints.decorations = decorate ? 1 : 0;

	/* Set Motif hints, most window managers handle these */
	XChangeProperty(gdk_x11_get_default_xdisplay(), xid /*wnck_window_get_xid (window)*/,
						my_wnck_atom_get ("_MOTIF_WM_HINTS"),
						my_wnck_atom_get ("_MOTIF_WM_HINTS"), 32, PropModeReplace,
						(unsigned char *)&hints, PROP_MOTIF_WM_HINTS_ELEMENTS);

	
	   //Window   xid;
   XWindowAttributes attrs;

   //xid = wnck_window_get_xid (window);
   XGetWindowAttributes(gdk_x11_get_default_xdisplay(), xid, &attrs);
	
	/* Apart from OpenBox, which doesn't respect it changing after mapping.
	  Instead it has this workaround. */
	devilspie2_change_state (attrs.screen,
								xid /*wnck_window_get_xid(window)*/, !decorate,
								my_wnck_atom_get ("_OB_WM_STATE_UNDECORATED"), 0);

}


/**
 *
 */
gboolean decorate_window(Window xid)
{
	devilspie2_error_trap_push();

	set_decorations(xid, TRUE);

	if (devilspie2_error_trap_pop()) {
		g_printerr("decorate_window %s\n",_("Failed!"));
		return FALSE;
	}

	return TRUE;
}


/**
 *
 */
gboolean undecorate_window(Window xid)
{
	devilspie2_error_trap_push();

	set_decorations(xid, FALSE);

	if (devilspie2_error_trap_pop()) {
		g_printerr("decorate_window %s\n", _("Failed!"));
		return FALSE;
	}

	return TRUE;
}


/**
 *
 */
Screen *devilspie2_window_get_xscreen(Window xid)
{
   XWindowAttributes attrs;

   XGetWindowAttributes(gdk_x11_get_default_xdisplay(), xid, &attrs);

   return attrs.screen;
}


/**
 *
 */
char* my_wnck_get_string_property_latin1(Window xwindow, Atom atom)
{
	Atom type;
	int format;
	gulong nitems;
	gulong bytes_after;
	unsigned char *property;
	int err, result;
	char *retval;

	devilspie2_error_trap_push();
	property = NULL;
	result = XGetWindowProperty (gdk_x11_get_default_xdisplay (),
											xwindow, atom,
											0, G_MAXLONG,
											False, AnyPropertyType, &type,
											&format, &nitems,
											&bytes_after, &property);

	err = devilspie2_error_trap_pop ();
	if (err != Success || result != Success)
		return NULL;

	retval = NULL;

	if (type == XA_STRING) {
		retval = g_strdup ((char*)property);
	}
	else if (type == XA_ATOM && nitems > 0 && format == 32) {
		long *pp;

		pp = (long *)property; // we can assume (long *) since format == 32
		if (nitems == 1) {
			char* prop_name;
			prop_name = XGetAtomName (gdk_x11_get_default_xdisplay (), *pp);
			if (prop_name) {
				retval = g_strdup (prop_name);
				XFree (prop_name);
			}
		} else {
			int i;
			char** prop_names;

			prop_names = g_new (char *, nitems + 1);
			prop_names[nitems] = NULL;
			for (i=0; i < nitems; i++) {
				prop_names[i] = XGetAtomName (gdk_x11_get_default_xdisplay (),
					*pp++);
			}
			retval = g_strjoinv (", ", prop_names);
			for (i=0; i < nitems; i++) {
				if (prop_names[i]) XFree (prop_names[i]);
			}
			g_free (prop_names);
		}
	} else if (type == XA_CARDINAL && nitems == 1) {
		switch(format) {
		case 32:
			retval = g_strdup_printf("%lu",*(unsigned long*)property);
			break;
		case 16:
			retval = g_strdup_printf("%u",*(unsigned int*)property);
			break;
		case 8:
			retval = g_strdup_printf("%c",*(unsigned char*)property);
			break;
		}
	}

	XFree (property);
	return retval;
}


/**
 *
 */
gboolean
my_wnck_get_cardinal_list (Window xwindow, Atom atom,
                          gulong **cardinals, int *len)
{
	Atom type;
	int format;
	gulong nitems;
	gulong bytes_after;
	gulong *nums;
	int err, result;

	*cardinals=NULL;
	*len=0;

	devilspie2_error_trap_push();
	type=None;
	result=XGetWindowProperty(gdk_x11_get_default_xdisplay (),
	                             xwindow,
	                             atom,
	                             0, G_MAXLONG,
	                             False, XA_CARDINAL, &type, &format, &nitems,
	                             &bytes_after, (void*)&nums);

	err=devilspie2_error_trap_pop();

	if ((err!=Success) || (result!=Success))
		return FALSE;

	if (type!=XA_CARDINAL) {
		XFree (nums);
		return FALSE;
	}

	*cardinals=g_new(gulong, nitems);
	memcpy(*cardinals, nums, sizeof (gulong) * nitems);
	*len=nitems;

	XFree(nums);

	return TRUE;
}


/**
 *
 */
glong my_wnck_get_cardinal(Window xwindow, Atom atom)
{
	Atom type;
	int format;
	gulong nitems;
	gulong bytes_after;
	gulong *nums;
	glong data;
	int err,result;

	devilspie2_error_trap_push();
	type=None;
	result=XGetWindowProperty(gdk_x11_get_default_xdisplay(),
	                          xwindow,
	                          atom,
	                          0, G_MAXLONG,
	                          False, XA_CARDINAL, &type, &format, &nitems,
	                          &bytes_after, (void*)&nums);

	if (result!=Success)
		return -1;

	err=devilspie2_error_trap_pop();
	if (err!=Success)
		return -1;

	if (type!=XA_CARDINAL) {
		XFree(nums);
		return -1;
	}

	data=nums[0];
	XFree(nums);

	return data;
}


/**
 *
 */
int devilspie2_get_viewport_start(Window xid)
{
	gulong *list;
	int len;

	int result=-1;
	
	my_wnck_get_cardinal_list(RootWindowOfScreen(
										devilspie2_window_get_xscreen(xid)),
										my_wnck_atom_get("_NET_DESKTOP_VIEWPORT"),
										&list, &len);

	if (len>0) result=list[0];

	g_free(list);

	return result;
}


/**
 *
 */
void my_window_set_window_type(Window xid, gchar *window_type)
{
	Display *display = gdk_x11_get_default_xdisplay();

	Atom atoms[10];

	/*
	_NET_WM_WINDOW_TYPE_DESKTOP, ATOM
	_NET_WM_WINDOW_TYPE_DOCK, ATOM
	_NET_WM_WINDOW_TYPE_TOOLBAR, ATOM
	_NET_WM_WINDOW_TYPE_MENU, ATOM
	_NET_WM_WINDOW_TYPE_UTILITY, ATOM
	_NET_WM_WINDOW_TYPE_SPLASH, ATOM
	_NET_WM_WINDOW_TYPE_DIALOG, ATOM
	_NET_WM_WINDOW_TYPE_NORMAL, ATOM
	*/

	gchar *type = NULL;

	//	Make it a recognized _NET_WM_TYPE

	if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_DESKTOP")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_DESKTOP");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_DOCK")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_DOCK");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_TOOLBAR")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_TOOLBAR");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_MENU")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_MENU");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_UTILITY")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_UTILITY");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_SPLASH")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_SPLASH");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_DIALOG")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_DIALOG");

	} else if (g_ascii_strcasecmp(window_type, "WINDOW_TYPE_NORMAL")==0) {
		type = g_strdup("_NET_WM_WINDOW_TYPE_NORMAL");

	} else {
		type = g_strdup(window_type);
	}

	atoms[0]=XInternAtom(display, type, False);

	XChangeProperty(gdk_x11_get_default_xdisplay(), xid,
						XInternAtom(display, "_NET_WM_WINDOW_TYPE", False), XA_ATOM, 32,
						PropModeReplace, (unsigned char *) &atoms, 1);

	if (type) g_free(type);
}


/**
 *
 */
void my_window_set_opacity(Window xid, double value)
{
	Display *display = gdk_x11_get_default_xdisplay();

	unsigned int opacity = (uint)(0xffffffff * value);
	Atom atom_net_wm_opacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", False);


	XChangeProperty(gdk_x11_get_default_xdisplay(), xid,
						atom_net_wm_opacity, XA_CARDINAL, 32,
						PropModeReplace, (unsigned char *) &opacity, 1L);

}

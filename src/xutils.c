/**
 *	This file is part of devilspie2
 *	Copyright (C) 2001 Havoc Pennington, 2011-2012 Andreas Rönnquist
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

#include <glib.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

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
void my_wnck_change_state(Screen *screen, Window xwindow,
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
void my_wnck_error_trap_push()
{
	gdk_error_trap_push();
}


/**
 *
 */
int my_wnck_error_trap_pop()
{
	XSync(gdk_x11_get_default_xdisplay(),False);
	return gdk_error_trap_pop();
}


/**
 *
 */
static void set_decorations(WnckWindow *window, gboolean decorate)
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
	XChangeProperty(gdk_x11_get_default_xdisplay(), wnck_window_get_xid (window),
						my_wnck_atom_get ("_MOTIF_WM_HINTS"),
						my_wnck_atom_get ("_MOTIF_WM_HINTS"), 32, PropModeReplace,
						(unsigned char *)&hints, PROP_MOTIF_WM_HINTS_ELEMENTS);

	/* Apart from OpenBox, which doesn't respect it changing after mapping.
	  Instead it has this workaround. */
	my_wnck_change_state (my_wnck_window_get_xscreen(window),
								wnck_window_get_xid(window), !decorate,
								my_wnck_atom_get ("_OB_WM_STATE_UNDECORATED"), 0);

}


/**
 *
 */
gboolean decorate_window(WnckWindow *window)
{
	my_wnck_error_trap_push();

	set_decorations(window,TRUE);

	if (my_wnck_error_trap_pop()) {
		g_printerr("decorate_window %s\n",_("Failed!"));
		return FALSE;
	}

	return TRUE;
}


/**
 *
 */
gboolean undecorate_window(WnckWindow *window)
{
	my_wnck_error_trap_push();

	set_decorations(window,FALSE);

	if (my_wnck_error_trap_pop()) {
		g_printerr("decorate_window %s\n", _("Failed!"));
		return FALSE;
	}

	return TRUE;
}


/**
 *
 */
Screen *my_wnck_window_get_xscreen(WnckWindow *window)
{
   Window   xid;
   XWindowAttributes attrs;

   xid = wnck_window_get_xid (window);
   XGetWindowAttributes(gdk_x11_get_default_xdisplay(), xid, &attrs);

   return attrs.screen;
}


/**
 *	This file is part of devilspie2
 *	Copyright (C) 2011-2012 Andreas Rönnquist
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
#include <stdlib.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h>

#include <locale.h>

#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)


#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


#if (GTK_MAJOR_VERSION >= 3)
#define HAVE_GTK3
#endif


#include "script_functions.h"

#include "script.h"

#include "xutils.h"

#include "error_strings.h"


/**
 *
 */
WnckWindow *current_window=NULL;



/**
 * returns the window name
 */
int c_get_window_name(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_name: %s",no_indata_expected_error);
		return 0;
	}
	char *test=NULL;

	WnckWindow *window=get_current_window();
	if (window) {
		test=(char*)wnck_window_get_name(window);
	} else {
		test="";
	}

	lua_pushstring(lua,test);

	// one item returned (the window name as a string)
	return 1;
}


/**
 * c_get_window_name always returns a string, even if a window hasn't
 * got a name - use this function to determine if a window really has
 * a name or not.
 * returns a boolean true or false
 */
int c_get_window_has_name(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_has_name: %s",no_indata_expected_error);
		return 0;
	}

	gboolean has_name=FALSE;

	WnckWindow *window=get_current_window();

	if (window) {
		has_name=wnck_window_has_name(window);
	}

	lua_pushboolean(lua,has_name);

	return 1;
}


/**
 * Set the Window Geometry
 * 	set_window_geometry(x,y,xsize,ysize);
 */
int c_set_window_geometry(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=4) {
		luaL_error(lua,"set_window_geometry: %s",four_indata_expected_error);
		return 0;
	}

	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);
	int type3=lua_type(lua,3);
	int type4=lua_type(lua,4);

	if ((type1!=LUA_TNUMBER) ||
	    (type2!=LUA_TNUMBER) ||
	    (type3!=LUA_TNUMBER) ||
	    (type4!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_geometry: %s", four_indata_expected_error);
		return 0;
	}

	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);
	int xsize=lua_tonumber(lua,3);
	int ysize=lua_tonumber(lua,4);

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			wnck_window_set_geometry(window,
			                         WNCK_WINDOW_GRAVITY_CURRENT,
			                         WNCK_WINDOW_CHANGE_X +
			                         WNCK_WINDOW_CHANGE_Y +
			                         WNCK_WINDOW_CHANGE_WIDTH +
			                         WNCK_WINDOW_CHANGE_HEIGHT,
			                         x,y,xsize,ysize);
		}
	}

	return 0;
}


/**
 * Set the Window Geometry2
 * 	set_window_geometry2(x,y,xsize,ysize);
 */
int c_set_window_geometry2(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=4) {
		luaL_error(lua,"set_window_geometry: %s",four_indata_expected_error);
		return 0;
	}

	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);
	int type3=lua_type(lua,3);
	int type4=lua_type(lua,4);

	if ((type1!=LUA_TNUMBER) ||
	    (type2!=LUA_TNUMBER) ||
	    (type3!=LUA_TNUMBER) ||
	    (type4!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_geometry: %s", four_indata_expected_error);
		return 0;
	}

	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);
	int xsize=lua_tonumber(lua,3);
	int ysize=lua_tonumber(lua,4);

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			XMoveResizeWindow(gdk_x11_get_default_xdisplay(),
			                  wnck_window_get_xid(window),
			                  x,y,
			                  xsize,ysize);
		}
	}

	return 0;
}


/**
 * Set the position of the window
 */
int c_set_window_position(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=2) {
		luaL_error(lua,"set_window_position: %s", two_indata_expected_error);
		return 0;
	}

	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);

	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_position: %s", two_indata_expected_error);
		return 0;
	}

	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);

	if (!devilspie2_emulate) {

		WnckWindow *window=get_current_window();

		if (window) {
			wnck_window_set_geometry(window,
				WNCK_WINDOW_GRAVITY_CURRENT,
				WNCK_WINDOW_CHANGE_X + WNCK_WINDOW_CHANGE_Y,
				x,y,-1,-1);
		}
	}

	return 0;
}


/**
 *
 */
int c_set_window_position2(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=2) {
		luaL_error(lua,"set_window_position2: %s", two_indata_expected_error);
		return 0;
	}

	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);

	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_position2: %s", two_indata_expected_error);
		return 0;
	}

	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);

	if (!devilspie2_emulate) {

		WnckWindow *window=get_current_window();

		if (window) {
			XMoveWindow(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()),
			            wnck_window_get_xid(window),
			            x,y);
		}
	}

	return 0;
}


/**
 * Sets the size of the window
 */
int c_set_window_size(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=2) {
		luaL_error(lua,"set_window_size: %s", two_indata_expected_error);
		return 0;
	}

	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);

	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_size: %s", two_indata_expected_error);
		return 0;
	}

	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);

	if (!devilspie2_emulate) {

		WnckWindow *window=get_current_window();

		if (window) {

			my_wnck_error_trap_push();
			wnck_window_set_geometry(window,
				WNCK_WINDOW_GRAVITY_CURRENT,
				WNCK_WINDOW_CHANGE_WIDTH + WNCK_WINDOW_CHANGE_HEIGHT,
				-1,-1,x,y);

			if (my_wnck_error_trap_pop()) {
				gchar *temperror=g_strdup_printf("set_window_size: %s",failed_string);
				g_printerr("%s",temperror);

				g_free(temperror);
			}
		}
	}

	return 0;
}


/**
 * Sets the window on top of all others and locks it "always on top"
 */
int c_make_always_on_top(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"make_always_on_top: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			wnck_window_make_above(window);
		}
	}

	return 0;
}


/**
 * sets the window on top of all the others
 */
int c_set_on_top(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"set_on_top: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			wnck_window_make_above(window);

			wnck_window_unmake_above(window);
		}
	}

	return 0;
}


/**
 * returns the application name
 */
int c_get_application_name(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_application_name: %s",no_indata_expected_error);
		return 0;
	}

	const char *application_name;

	WnckWindow *window=get_current_window();

	if (window) {

		WnckApplication *application=wnck_window_get_application(get_current_window());
		application_name=wnck_application_get_name(application);

	} else {
		application_name="";
	}

	// one item returned - the application name as a string.
	lua_pushstring(lua,application_name);

	return 1;
}


/**
 *	lua_Bprint from http://www.lua.org/source/5.1/lbaselib.c.html
 * but with the change that fputs is only called if devilspie2_debug is
 * set to TRUE
 */
int c_debug_print(lua_State *lua)
{
	int n = lua_gettop(lua);  /* number of arguments */
	int i;
	lua_getglobal(lua, "tostring");
	for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(lua, -1);  /* function to be called */
		lua_pushvalue(lua, i);   /* value to print */
		lua_call(lua, 1, 1);
		s = lua_tostring(lua, -1);  /* get result */
		if (s == NULL)
			return luaL_error(lua, LUA_QL("tostring") " must return a string to "
									LUA_QL("print"));
		if (i>1) {
			if (devilspie2_debug) fputs("\t", stdout);
		}
		if (devilspie2_debug) fputs(s, stdout);
		lua_pop(lua, 1);  /* pop result */
	}
	if (devilspie2_debug) fputs("\n", stdout);

	return 0;
}


/**
 * "Shades" the window
 */
int c_shade_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"shade_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {

			wnck_window_shade(window);
		}
	}

	return 0;
}


/**
 * "Unshades" the window
 */
int c_unshade_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"unshade_window: %s");
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {

			wnck_window_unshade(window);
		}
	}

	return 0;
}


/**
 * Minimizes the window
 */
int c_minimize_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"minimize_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
				WnckWindow *window=get_current_window();

		if (window) {

			wnck_window_minimize(window);
		}
	}

	return 0;
}


/**
 * unminimizes window
 */
int c_unminimize_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"unminimize_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			wnck_window_unminimize (window, GDK_CURRENT_TIME);
		}
	}

	return 0;
}


/**
 * sets the window that the scripts are affecting
 */
void set_current_window(WnckWindow *window)
{
	current_window=window;
}


/**
 * gets the window that the scripts are affecting
 */
WnckWindow *get_current_window()
{
	return current_window;
}


/**
 * Decorates a window
 */
int c_undecorate_window(lua_State *lua)
{
	gboolean result=TRUE;
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"undecorate_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {
			if (!devilspie2_emulate) {

				if (!undecorate_window(window)) {
					result=FALSE;
				}
			}
		}
	}

	lua_pushboolean(lua,result);

	return 1;
}


/**
 * undecorates a window
 */
int c_decorate_window(lua_State *lua)
{
	gboolean result=TRUE;
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"decorate_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();

		if (window) {

			if (!devilspie2_emulate) {
				if (!decorate_window(window)) {
					result=FALSE;
				}
			}
		}
	}

	lua_pushboolean(lua,result);

	return 1;
}



/**
 * Move a window to a specific workspace
 */
int c_set_window_workspace(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=1) {
		luaL_error(lua,"set_window_workspace: %s",one_indata_expected_error);
		return 0;
	}

	int type=lua_type(lua,1);

	if (type!=LUA_TNUMBER) {
		luaL_error(lua,"set_window_workspace: %s",number_expected_as_indata_error);
		return 0;
	}

	int number=lua_tonumber(lua,1);

	WnckWindow *window=get_current_window();

	if (window) {
		WnckScreen *screen;
		WnckWorkspace *workspace;

		screen=wnck_window_get_screen(window);
		workspace=wnck_screen_get_workspace(screen,number-1);

		if (!workspace) {
			g_warning(_("Workspace number %d does not exist!"),number);
		}
		if (!devilspie2_emulate) {
			wnck_window_move_to_workspace(window,workspace);
		}
	}

	lua_pushboolean(lua,TRUE);

	return 1;
}




/**
 * Makes a workspace the active one
 */
int c_change_workspace(lua_State *lua)
{
	int top=lua_gettop(lua);
	GTimeVal timestamp;

	if (top!=1) {
		luaL_error(lua,"change_workspace: %s",one_indata_expected_error);
		return 0;
	}

	int type=lua_type(lua,1);

	if (type!=LUA_TNUMBER) {
		luaL_error(lua,"change_workspace: %s",number_expected_as_indata_error);
		return 0;
	}

	int number=lua_tonumber(lua,1);

	WnckWindow *window=get_current_window();
	if (window) {
		WnckScreen *screen;
		WnckWorkspace *workspace;

		screen=wnck_window_get_screen(window);
		workspace=wnck_screen_get_workspace(screen,number-1);

		if (!workspace) {
			g_warning(_("Workspace number %d does not exist!"),number);
		}

		g_get_current_time(&timestamp);
		if (!devilspie2_emulate) {
			wnck_workspace_activate(workspace,timestamp.tv_sec);
		}
	}

	lua_pushboolean(lua,TRUE);

	return 1;
}


/**
 * Unmaximize window
 */
int c_unmaximize_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"unmaximize: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_unmaximize(window);
		}
	}

	return 0;
}


/**
 * Maximize Window
 */
int c_maximize_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"maximize: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_maximize(window);
		}
	}
	return 0;
}


/**
 * Maximize Window Vertically
 */
int c_maximize_window_vertically(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"maximize_vertically: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_maximize_vertically(window);
		}
	}

	return 0;
}


/**
 * Maximize the window horisontally
 */
int c_maximize_window_horisontally(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"maximize_horisontally: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_maximize_horizontally(window);
		}
	}

	return 0;
}



/**
 * Pins the window
 */
int c_pin_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"pin_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_pin(window);
		}
	}

	return 0;
}



/**
 * Unpin the window
 */
int c_unpin_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"unpin_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_unpin(window);
		}
	}

	return 0;
}


/**
 * Sticks the window
 */
int c_stick_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"stick_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_stick(window);
		}
	}

	return 0;
}


/**
 * Unstick the window
 */
int c_unstick_window(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"unstick_window: %s",no_indata_expected_error);
		return 0;
	}

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_unstick(window);
		}
	}

	return 0;
}


/**
 * return the geometry of the current window to the LUA script
 */
int c_get_window_geometry(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_geometry: %s",no_indata_expected_error);
		return 0;
	}

	int x,y,width,height;

	WnckWindow *window=get_current_window();
	if (window)
	{
		wnck_window_get_geometry(window,&x,&y,&width,&height);
	}

	lua_pushnumber(lua,x);
	lua_pushnumber(lua,y);
	lua_pushnumber(lua,width);
	lua_pushnumber(lua,height);

	return 4;
}


/**
 * return the client geometry of the current window to the LUA script
 * this is excluding the window manager frame
 */
int c_get_client_window_geometry(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_client_window_geometry: %s",no_indata_expected_error);
		return 0;
	}

	int x,y,width,height;

	WnckWindow *window=get_current_window();
	if (window)
	{
		wnck_window_get_client_window_geometry(window,&x,&y,&width,&height);
	}

	lua_pushnumber(lua,x);
	lua_pushnumber(lua,y);
	lua_pushnumber(lua,width);
	lua_pushnumber(lua,height);

	return 4;
}


/**
 *
 */
int c_set_skip_tasklist(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=1) {
		luaL_error(lua,"set_skip_tasklist: %s", one_indata_expected_error);
		return 0;
	}

	int type=lua_type(lua,1);

	if (type!=LUA_TBOOLEAN) {
		luaL_error(lua,"set_skip_tasklist: %s",boolean_expected_as_indata_error);
		return 0;
	}

	int value=lua_toboolean(lua,1);

	gboolean skip_tasklist=(gboolean)(value);

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_set_skip_tasklist(window,skip_tasklist);
		}
	}

	return 0;
}


/**
 *
 */
int c_set_skip_pager(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=1) {
		luaL_error(lua,"set_skip_pager: %s", one_indata_expected_error);
		return 0;
	}

	int type=lua_type(lua,1);

	if (type!=LUA_TBOOLEAN) {
		luaL_error(lua,"set_skip_pager: %s",boolean_expected_as_indata_error);
		return 0;
	}

	int value=lua_toboolean(lua,1);

	gboolean skip_pager=(gboolean)(value);

	if (!devilspie2_emulate) {
		WnckWindow *window=get_current_window();
		if (window) {
			wnck_window_set_skip_pager(window,skip_pager);
		}
	}

	return 0;
}


/**
 *
 */
int c_get_window_is_maximized(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_is__maximized: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gboolean is_maximized=FALSE;
	if (window) {
		is_maximized=wnck_window_is_maximized(window);
	}

	lua_pushboolean(lua,is_maximized);

	return 1;
}

/**
 *
 */
int c_get_window_is_maximized_vertically(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_is_horisontally_maximized: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gboolean is_vertically_maximized=FALSE;
	if (window) {
		is_vertically_maximized=wnck_window_is_maximized_vertically(window);
	}

	lua_pushboolean(lua,is_vertically_maximized);

	return 1;
}


/**
 *
 */
int c_get_window_is_maximized_horisontally(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_is_horisontally_maximized: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gboolean is_horisontally_maximized=FALSE;
	if (window) {
		is_horisontally_maximized=wnck_window_is_maximized_horizontally(window);
	}

	lua_pushboolean(lua,is_horisontally_maximized);

	return 1;
}


/**
 *
 */
int c_set_window_above(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"set_window_above: %s", no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	if (!devilspie2_emulate) {

		my_wnck_change_state(my_wnck_window_get_xscreen(window),
		                     wnck_window_get_xid(window),
		                     TRUE,
		                     my_wnck_atom_get("_NET_WM_STATE_ABOVE"),
		                     0);
	}

	return 0;
}


/**
 *
 */
int c_set_window_below(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"set_window_above: %s", no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	if (!devilspie2_emulate) {

		my_wnck_change_state(my_wnck_window_get_xscreen(window),
		                     wnck_window_get_xid(window),
		                     TRUE,
		                     my_wnck_atom_get("_NET_WM_STATE_BELOW"),
		                     0);
	}

	return 0;
}


/**
 *
 */
int c_get_window_type(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_type: %s", no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gchar *window_type_string=NULL;

	if (window) {
		WnckWindowType window_type=wnck_window_get_window_type(window);

		switch (window_type) {
		case WNCK_WINDOW_NORMAL:
			window_type_string=g_strdup("WINDOW_TYPE_NORMAL");
			break;
		case WNCK_WINDOW_DESKTOP:
			window_type_string=g_strdup("WINDOW_TYPE_DESKTOP");
			break;
		case WNCK_WINDOW_DOCK:
			window_type_string=g_strdup("WINDOW_TYPE_DOCK");
			break;
		case WNCK_WINDOW_DIALOG:
			window_type_string=g_strdup("WINDOW_TYPE_DIALOG");
			break;
		case WNCK_WINDOW_TOOLBAR:
			window_type_string=g_strdup("WINDOW_TYPE_TOOLBAR");
			break;
		case WNCK_WINDOW_MENU:
			window_type_string=g_strdup("WINDOW_TYPE_MENU");
			break;
		case WNCK_WINDOW_UTILITY:
			window_type_string=g_strdup("WINDOW_TYPE_UTILITY");
			break;
		case WNCK_WINDOW_SPLASHSCREEN:
			window_type_string=g_strdup("WINDOW_TYPE_SPLASHSCREEN");
			break;
		default:
			window_type_string=g_strdup("WINDOW_TYPE_UNRECOGNIZED");
		};
	} else {
		window_type_string=g_strdup("WINDOW_ERROR");
	}

	lua_pushstring(lua,window_type_string);

	g_free(window_type_string);

	return 1;
}


#ifdef HAVE_GTK3
/**
 * c_get_class_instance_name
 * Only available on libwnck 3+
 */
int c_get_class_instance_name(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_class_instance_name: %s",no_indata_expected_error);
		return 0;
	}

	const char *class_instance_name;

	WnckWindow *window=get_current_window();

	if (window) {
		class_instance_name=wnck_window_get_class_instance_name(window);

	} else {
		class_instance_name="";
	}

	// one item returned - the window class instance name as a string.
	lua_pushstring(lua,class_instance_name);

	return 1;
}

#endif


/**
 *
 */
int c_get_window_property(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=1) {
		luaL_error(lua,"get_window_property: %s",one_indata_expected_error);
		return 0;
	}

	//	gchar *property=
	int type=lua_type(lua,1);

	if (type!=LUA_TSTRING) {
		luaL_error(lua,"get_window_property: %s",string_expected_as_indata_error);
		return 0;
	}

	const gchar *value=lua_tostring(lua,1);

	WnckWindow *window=get_current_window();

	gchar *result=NULL;

	if (window) {
		result=my_wnck_get_string_property_latin1(wnck_window_get_xid(window),my_wnck_atom_get(value));
	} else {
		result=g_strdup_printf("NO RESULT");
	}

	if (result) {
		lua_pushstring(lua,result);
	} else {
		lua_pushstring(lua,"");
	}

	g_free(result);

	return 1;
}


/**
 *
 */
int c_get_window_role(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_role: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gchar *result=NULL;

	if (window) {
		result=my_wnck_get_string_property_latin1(wnck_window_get_xid(window),my_wnck_atom_get("WM_WINDOW_ROLE"));
	}

	if (result) {
		lua_pushstring(lua,result);
	} else {
		lua_pushstring(lua,"");
	}

	g_free(result);

	return 1;
}


/**
 *
 */
int c_get_window_xid(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_xid: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gulong result;

	if (window) {
		result=wnck_window_get_xid(window);
	} else {
		result=0;
	}

	lua_pushnumber(lua,result);

	return 1;
}


/**
 *
 */
int c_get_window_class(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=0) {
		luaL_error(lua,"get_window_class: %s",no_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	gchar *result;

	if (window) {
		result=g_strdup(wnck_class_group_get_res_class (wnck_window_get_class_group(window)));

	} else {
		result=g_strdup("");
	}

	lua_pushstring(lua,result);

	g_free(result);

	return 1;
}


/**
 *
 */
int c_set_window_fullscreen(lua_State *lua)
{
	int top=lua_gettop(lua);

	if (top!=1) {
		luaL_error(lua,"set_window_fullscreen: %s", one_indata_expected_error);
		return 0;
	}

	WnckWindow *window=get_current_window();

	int type=lua_type(lua,1);

	if (type!=LUA_TBOOLEAN) {
		luaL_error(lua,"set_window_fullscreen: %s",boolean_expected_as_indata_error);
		return 0;
	}

	gboolean fullscreen=lua_toboolean(lua,1);

	if (window) {
		if (!devilspie2_emulate) {
			wnck_window_set_fullscreen(window,fullscreen);
		}
	}


	return 0;
}


/**
 *
 */
int c_set_viewport(lua_State *lua)
{
	int top=lua_gettop(lua);
	WnckScreen *screen;
	int x,y,width,height, viewport_start;

	if (top!=1) {
		luaL_error(lua,"set_viewport: %s", one_indata_expected_error);
		return 0;
	}

	int type=lua_type(lua,1);
	if (type!=LUA_TNUMBER) {
		luaL_error(lua,"set_viewport: %s",number_expected_as_indata_error);
		return 0;
	}

	int num=lua_tonumber(lua,1);

	if (num<=0) {
		g_error("set_viewport: %s",integer_greater_than_zero_expected_error);
		lua_pushboolean(lua,FALSE);
		return 1;
	}

	WnckWindow *window=get_current_window();

	screen=wnck_window_get_screen(window);

	wnck_window_get_geometry(window, &x, &y, &width, &height);

	viewport_start=my_wnck_get_viewport_start(window);
	if (viewport_start<0) {
		g_printerr("set_viewport: %s",could_not_find_current_viewport_error);
		lua_pushboolean(lua,FALSE);
		return 1;
	}

	x=((num-1) * wnck_screen_get_width(screen)) - viewport_start + x;

	my_wnck_error_trap_push();
	XMoveResizeWindow(gdk_x11_get_default_xdisplay(),
	                  wnck_window_get_xid(window),
	                  x,y,width,height);

	if (my_wnck_error_trap_pop()) {
		g_printerr("set_viewport: %s",setting_viewport_failed_error);
		lua_pushboolean(lua,FALSE);
		return 1;
	}

	lua_pushboolean(lua,TRUE);
	return 1;
}

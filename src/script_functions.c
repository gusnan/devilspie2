/**
 *
 *	This file is part of devilspie2.
 *	Copyright 2011 Andreas Rönnquist
 *
 *	devilspie2 is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	devilspie2 is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with devilspie2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "script_functions.h"

#include "script.h"

#include "xutils.h"


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
		luaL_error(lua,"get_window_name: No indata expected");
		return 0;
	}

	char *test=(char*)wnck_window_get_name(get_current_window());

	lua_pushstring(lua,test);
	
	// one item returned (the window name as a string)
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
		luaL_error(lua,"set_window_geometry: 4 indata required - xpos,ypos, xsize,ysize");
		return 0;
	}
	
	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);
	int type3=lua_type(lua,3);
	int type4=lua_type(lua,4);
	
	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER) || (type3!=LUA_TNUMBER) || (type4!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_geometry: 4 numbers expected as indata.");
		return 0;
	}
	
	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);
	int xsize=lua_tonumber(lua,3);
	int ysize=lua_tonumber(lua,4);
	
	if (!devilspie2_emulate) {
		wnck_window_set_geometry(get_current_window(),
			WNCK_WINDOW_GRAVITY_CURRENT,
			WNCK_WINDOW_CHANGE_X + WNCK_WINDOW_CHANGE_Y + WNCK_WINDOW_CHANGE_WIDTH + WNCK_WINDOW_CHANGE_HEIGHT,
			x,y,xsize,ysize);
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
		luaL_error(lua,"set_window_position: 2 indata required - xpos,ypos");
		return 0;
	}
	
	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);
	
	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_position: 2 numbers expected as indata.");
		return 0;
	}
	
	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);
	
	if (!devilspie2_emulate) {
		wnck_window_set_geometry(get_current_window(),
			WNCK_WINDOW_GRAVITY_CURRENT,
			WNCK_WINDOW_CHANGE_X + WNCK_WINDOW_CHANGE_Y,
			x,y,-1,-1);
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
		luaL_error(lua,"set_window_size: 2 numbers required - xsize,ysize");
		return 0;
	}
	
	int type1=lua_type(lua,1);
	int type2=lua_type(lua,2);
	
	if ((type1!=LUA_TNUMBER) || (type2!=LUA_TNUMBER)) {
		luaL_error(lua,"set_window_size: 2 numbers expected as indata.");
		return 0;
	}
	
	int x=lua_tonumber(lua,1);
	int y=lua_tonumber(lua,2);
	
	if (!devilspie2_emulate) {
		
		my_wnck_error_trap_push();
		wnck_window_set_geometry(get_current_window(),
			WNCK_WINDOW_GRAVITY_CURRENT,
			WNCK_WINDOW_CHANGE_WIDTH + WNCK_WINDOW_CHANGE_HEIGHT,
			-1,-1,x,y);
		
		if (my_wnck_error_trap_pop()) {
			g_printerr("set_window_size failed");
						
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
		luaL_error(lua,"make_always_on_top: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_make_above(get_current_window());
	
	return 0;
}


/**
 * sets the window on top of all the others
 */
int c_set_on_top(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"set_on_top: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate) {
		wnck_window_make_above(get_current_window());
	
		wnck_window_unmake_above(get_current_window());
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
		luaL_error(lua,"get_application_name: No indata expected");
		return 0;
	}
	
	const char *application_name;
	
	WnckApplication *application=wnck_window_get_application(get_current_window());
	application_name=wnck_application_get_name(application);

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
		luaL_error(lua,"shade_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_shade(get_current_window());
	
	return 0;
}


/**
 * "Unshades" the window
 */
int c_unshade_window(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"unshade_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_unshade(get_current_window());
	
	return 0;
}


/**
 *
 */
int c_minimize_window(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"minimize_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_minimize(get_current_window());
	
	return 0;
}


/**
 *
 */
int c_unminimize_window(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"unminimize_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_unminimize (get_current_window(), GDK_CURRENT_TIME);
	
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
		luaL_error(lua,"undecorate_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate) {
		if (!undecorate_window(get_current_window())) {
			result=FALSE;
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
		luaL_error(lua,"decorate_window: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate) {
		if (!decorate_window(get_current_window())) {
			result=FALSE;
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
	WnckScreen *screen;
	WnckWorkspace *workspace;
	
	if (top!=1) {
		luaL_error(lua,"set_workspace: 1 indata required");
		return 0;
	}
	
	int type=lua_type(lua,1);
	
	if (type!=LUA_TNUMBER) {
		luaL_error(lua,"set_workspace: a number expected as indata");
		return 0;
	}
	
	int number=lua_tonumber(lua,1);
	
	screen=wnck_window_get_screen(current_window);
	workspace=wnck_screen_get_workspace(screen,number-1);
	
	if (!workspace) {
		g_warning("Workspace number %d does not exist!",number);
	}
	wnck_window_move_to_workspace(current_window,workspace);
	
	lua_pushboolean(lua,TRUE);
	
	return 1;
}




/**
 * TODO! Implementation
 */
int c_change_workspace(lua_State *lua)
{
	int top=lua_gettop(lua);
	WnckScreen *screen;
	WnckWorkspace *workspace;
	GTimeVal timestamp;
	
	if (top!=1) {
		luaL_error(lua,"change_workspace: 1 indata required");
		return 0;
	}
	
	int type=lua_type(lua,1);
	
	if (type!=LUA_TNUMBER) {
		luaL_error(lua,"change_workspace: a number expected as indata");
		return 0;
	}
	
	int number=lua_tonumber(lua,1);
	
	screen=wnck_window_get_screen(current_window);
	workspace=wnck_screen_get_workspace(screen,number-1);
	
	if (!workspace) {
		g_warning("Workspace number %d does not exist!",number);
	}
	
	g_get_current_time(&timestamp);
	wnck_workspace_activate(workspace,timestamp.tv_sec);
	
	lua_pushboolean(lua,TRUE);
	
	return 1;
}



/**
 *
 */
int c_maximize_window(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"maximize: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_maximize(get_current_window());
	
	return 0;
}


/**
 *
 */
int c_maximize_window_vertically(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"maximize_vertically: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_maximize_vertically(get_current_window());
	
	return 0;
}


/**
 *
 */
int c_maximize_window_horisontally(lua_State *lua)
{
	int top=lua_gettop(lua);
	
	if (top!=0) {
		luaL_error(lua,"maximize_horisontally: No indata expected");
		return 0;
	}
	
	if (!devilspie2_emulate)
		wnck_window_maximize_horizontally(get_current_window());
	
	return 0;
}

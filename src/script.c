/**
 *	This file is part of devilspie2
 *	Copyright (C) 2011-2013 Andreas Rönnquist
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


#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <glib.h>

#include <gdk/gdk.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <locale.h>

#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)


#include "script.h"

#if (GTK_MAJOR_VERSION >= 3)
#define HAVE_GTK3
#endif

#include "script_functions.h"



/**
 *
 */
gboolean script_loaded=FALSE;

gboolean devilspie2_debug=FALSE;
gboolean devilspie2_emulate=FALSE;

lua_State *global_lua_state=NULL;

/**
 *
 */
lua_State *
init_script()
{
	lua_State *lua=luaL_newstate();
	luaL_openlibs(lua);

	register_cfunctions(lua);

	return lua;
}


/**
 *
 */
void
register_cfunctions(lua_State *lua)
{
	lua_register(lua, "get_window_name", c_get_window_name);
	lua_register(lua, "get_window_has_name", c_get_window_has_name);

	lua_register(lua, "set_window_position", c_set_window_position);
	lua_register(lua, "set_window_position2", c_set_window_position2);
	lua_register(lua, "set_window_size", c_set_window_size);
	lua_register(lua, "set_window_strut", c_set_window_strut);

	lua_register(lua, "set_window_geometry", c_set_window_geometry);
	lua_register(lua, "set_window_geometry2", c_set_window_geometry2);

	lua_register(lua, "get_application_name", c_get_application_name);

	lua_register(lua, "make_always_on_top", c_make_always_on_top);

	lua_register(lua, "set_on_top", c_set_on_top);

	lua_register(lua, "debug_print", c_debug_print);

	lua_register(lua, "shade", c_shade_window);
	lua_register(lua, "unshade", c_unshade_window);

	lua_register(lua, "maximize", c_maximize_window);
	lua_register(lua, "maximize_horisontally", c_maximize_window_horisontally);
	lua_register(lua, "maximize_horizontally", c_maximize_window_horisontally);
	lua_register(lua, "maximize_vertically", c_maximize_window_vertically);
	lua_register(lua, "unmaximize", c_unmaximize_window);

	lua_register(lua, "minimize", c_minimize_window);
	lua_register(lua, "unminimize", c_unminimize_window);

	lua_register(lua, "decorate_window", c_decorate_window);
	lua_register(lua, "undecorate_window", c_undecorate_window);

	lua_register(lua, "set_window_workspace", c_set_window_workspace);
	lua_register(lua, "change_workspace", c_change_workspace);
	lua_register(lua, "get_workspace_count", c_get_workspace_count);

	lua_register(lua, "pin_window", c_pin_window);
	lua_register(lua, "unpin_window", c_unpin_window);
	lua_register(lua, "stick_window", c_stick_window);
	lua_register(lua, "unstick_window", c_unstick_window);

	lua_register(lua, "close_window", c_close_window);

	lua_register(lua, "get_window_geometry", c_get_window_geometry);
	lua_register(lua, "get_window_client_geometry", c_get_client_window_geometry);

	lua_register(lua, "set_skip_tasklist", c_set_skip_tasklist);
	lua_register(lua, "set_skip_pager", c_set_skip_pager);

	lua_register(lua, "get_window_is_maximized", c_get_window_is_maximized);

	lua_register(lua, "get_window_is_maximized_vertically",
		c_get_window_is_maximized_vertically);

	lua_register(lua, "get_window_is_maximized_horisontally",
		c_get_window_is_maximized_horisontally);
	lua_register(lua, "get_window_is_maximized_horizontally",
		c_get_window_is_maximized_horisontally);

	lua_register(lua, "set_window_below", c_set_window_below);
	lua_register(lua, "set_window_above", c_set_window_above);
	lua_register(lua, "set_window_fullscreen", c_set_window_fullscreen);

	lua_register(lua, "get_window_type", c_get_window_type);

	lua_register(lua, "get_window_property", c_get_window_property);
	lua_register(lua, "get_window_role", c_get_window_role);
	lua_register(lua, "get_window_xid", c_get_window_xid);

	lua_register(lua, "get_window_class", c_get_window_class);

	lua_register(lua, "set_viewport", c_set_viewport);

	lua_register(lua, "center", c_center);

	lua_register(lua, "set_opacity", c_set_opacity);
	lua_register(lua, "set_window_opacity", c_set_opacity);

	lua_register(lua, "set_window_type", c_set_window_type);

	lua_register(lua, "get_screen_geometry", c_get_screen_geometry);

#ifdef HAVE_GTK3
	// wnck_window_get_class_instance_name is only availible on wnck 3 and later
	lua_register(lua, "get_class_instance_name",c_get_class_instance_name);
#endif

	lua_register(lua, "focus", c_focus);
	lua_register(lua, "focus_window", c_focus);

}


/**
 *
 */
int
load_script(lua_State *lua,char *filename)
{
	if (lua) {
		int result=luaL_loadfile(lua,filename);

		if (!result) {
			script_loaded=TRUE;
		} else {

			// We got an error, print it
			printf("%s\n",lua_tostring(lua,-1));

			lua_pop(lua,1);

			return -1;
		}
	} else {
		return -1;
	}

	return 0;
}


/**
 *
 */
void
run_script(lua_State *lua)
{
	int s = lua_pcall( lua, 0, LUA_MULTRET, 0 );

	if (s) {

		char *error_msg;

		error_msg=(char*)lua_tostring( lua, -1 );

		//std::string luaErrorString=getLuaErrorString(s);

		/*
		mssOut.str( "" );
		mssOut << "Script::runScript : Error caught running script "
			<< sScriptName << "\n"
			<< "    Error code is " << luaErrorString << ".\n"
			<< "    Error msg is: " << errorMsg;
		throw( ScriptException( mssOut.str().c_str(), errorMsg ) );
		*/

		printf(_("Error: %s"),error_msg);
		printf("\n");
	}
}


/**
 *
 */
void
done_script(lua_State *lua)
{
	if (lua)
		lua_close(lua);

	//lua=NULL;
}


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


#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <glib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <locale.h>

#include <libintl.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)


#include "script.h"

#include "script_functions.h"


/**
 *
 */
lua_State *lua=NULL;

gboolean script_loaded=FALSE;

gboolean devilspie2_debug=FALSE;
gboolean devilspie2_emulate=FALSE;


/**
 *
 */
void init_script()
{

	if (lua) {
	}

	lua=lua_open();
	luaL_openlibs(lua);

	register_cfunctions();
}


/**
 *
 */
void register_cfunctions()
{
	lua_register(lua,"get_window_name",c_get_window_name);
	lua_register(lua,"get_window_has_name",c_get_window_has_name);

	lua_register(lua,"set_window_position",c_set_window_position);
	lua_register(lua,"set_window_size",c_set_window_size);
	lua_register(lua,"set_window_geometry",c_set_window_geometry);
	lua_register(lua,"set_window_geometry2",c_set_window_geometry2);

	lua_register(lua,"get_application_name",c_get_application_name);

	lua_register(lua,"make_always_on_top",c_make_always_on_top);

	lua_register(lua,"set_on_top",c_set_on_top);

	lua_register(lua,"debug_print",c_debug_print);

	lua_register(lua,"shade",c_shade_window);
	lua_register(lua,"unshade",c_unshade_window);

	lua_register(lua,"maximize",c_maximize_window);
	lua_register(lua,"maximize_horisontally",c_maximize_window_horisontally);
	lua_register(lua,"maximize_vertically",c_maximize_window_vertically);
	lua_register(lua,"unmaximize",c_unmaximize_window);

	lua_register(lua,"minimize",c_minimize_window);
	lua_register(lua,"unminimize",c_unminimize_window);

	lua_register(lua,"decorate_window",c_decorate_window);
	lua_register(lua,"undecorate_window",c_undecorate_window);

	lua_register(lua,"set_window_workspace",c_set_window_workspace);
	lua_register(lua,"change_workspace",c_change_workspace);

	lua_register(lua,"pin_window",c_pin_window);
	lua_register(lua,"unpin_window",c_unpin_window);
	lua_register(lua,"stick_window",c_stick_window);
	lua_register(lua,"unstick_window",c_unstick_window);

	lua_register(lua,"get_window_geometry",c_get_window_geometry);
	lua_register(lua,"get_window_client_geometry",c_get_client_window_geometry);

	lua_register(lua,"set_skip_tasklist",c_set_skip_tasklist);
	lua_register(lua,"set_skip_pager",c_set_skip_pager);

	lua_register(lua,"get_window_is_maximized",c_get_window_is_maximized);

	lua_register(lua,"get_window_is_maximized_vertically",c_get_window_is_maximized_vertically);
	lua_register(lua,"get_window_is_maximized_horisontally",c_get_window_is_maximized_horisontally);
}


/**
 *
 */
int load_script(char *filename)
{

	int result=luaL_loadfile(lua,filename);

	if (!result) {
		script_loaded=TRUE;
	} else {

		// We got an error, print it
		printf("%s\n",lua_tostring(lua,-1));

		lua_pop(lua,1);

		return -1;
	}

	return 0;
}


/**
 *
 */
void run_script()
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
void done_script()
{
	if (lua)
		lua_close(lua);

	lua=NULL;
}


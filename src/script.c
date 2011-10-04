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

#include <glib.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

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
	
	lua_register(lua,"set_window_position",c_set_window_position);
	lua_register(lua,"set_window_size",c_set_window_size);
	lua_register(lua,"set_window_geometry",c_set_window_geometry);
	
	lua_register(lua,"get_application_name",c_get_application_name);
	
	lua_register(lua,"make_always_on_top",c_make_always_on_top);
	
	lua_register(lua,"set_on_top",c_set_on_top);
	
	lua_register(lua,"debug_print",c_debug_print);
	
	lua_register(lua,"shade",c_shade_window);
	lua_register(lua,"unshade",c_unshade_window);
	
	lua_register(lua,"minimize",c_minimize_window);
	lua_register(lua,"unminimize",c_unminimize_window);
	
	lua_register(lua,"decorate_window",c_decorate_window);
	lua_register(lua,"undecorate_window",c_undecorate_window);
}


/**
 *
 */
int load_script(char *filename)
{
	
	int result=luaL_loadfile(lua,filename);
	
	if (result==0) {
		script_loaded=TRUE;
	} else {
		
		// We got an error, print it
		printf(lua_tostring(lua,-1));
		printf("\n");
		
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

	if( s != 0 ) {
	
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
		
		printf("Error:%s\n",error_msg);
	}
}


/**
 *
 */
void done_script()
{
	
	if (lua!=NULL)
		lua_close(lua);
	
	
	lua=NULL;
}


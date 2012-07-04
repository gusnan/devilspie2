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

#ifndef __HEADER_SCRIPT_FUNCTIONS_
#define __HEADER_SCRIPT_FUNCTIONS_

/**
 *
 */
int init_script_error_messages();
void done_script_error_messages();

int c_get_window_name(lua_State *lua);
int c_get_window_has_name(lua_State *lua);

int c_set_window_position(lua_State *lua);
int c_set_window_position2(lua_State *lua);

int c_set_window_geometry(lua_State *lua);
int c_set_window_geometry2(lua_State *lua);

int c_set_window_size(lua_State *lua);

int c_make_always_on_top(lua_State *lua);

int c_set_on_top(lua_State *lua);

int c_get_application_name(lua_State *lua);

int c_debug_print(lua_State *lua);

int c_shade_window(lua_State *lua);
int c_unshade_window(lua_State *lua);

int c_minimize_window(lua_State *lua);
int c_unminimize_window(lua_State *lua);

int c_decorate_window(lua_State *lua);
int c_undecorate_window(lua_State *lua);

int c_set_window_workspace(lua_State *lua);
int c_change_workspace(lua_State *lua);

int c_unmaximize_window(lua_State *lua);
int c_maximize_window(lua_State *lua);
int c_maximize_window_vertically(lua_State *lua);
int c_maximize_window_horisontally(lua_State *lua);

int c_pin_window(lua_State *lua);
int c_unpin_window(lua_State *lua);
int c_stick_window(lua_State *lua);
int c_unstick_window(lua_State *lua);

void set_current_window(WnckWindow *window);
WnckWindow *get_current_window();

int c_get_window_geometry(lua_State *lua);
int c_get_client_window_geometry(lua_State *lua);

int c_set_skip_tasklist(lua_State *lua);
int c_set_skip_pager(lua_State *lua);

int c_get_window_is_maximized(lua_State *lua);
int c_get_window_is_maximized_vertically(lua_State *lua);
int c_get_window_is_maximized_horisontally(lua_State *lua);

int c_set_window_above(lua_State *lua);
int c_set_window_below(lua_State *lua);

#endif /*__HEADER_SCRIPT_FUNCTIONS_*/

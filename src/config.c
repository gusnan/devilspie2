/**
 *	This file is part of devilspie2
 *	Copyright (C) 2013 Andreas Rönnquist
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

#include <glib/gi18n.h>

#include <locale.h>


#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "script.h"

#include "config.h"

/**
 *
 */
GSList *file_window_open_list = NULL;
GSList *file_window_close_list = NULL;


/**
 * filename_list_sortfunc
 *   function to sort the inserted filenames, to be able to determine
 *   which order files are loaded.
 */
gint filename_list_sortfunc(gconstpointer a,gconstpointer b)
{
	struct lua_File *file1 = (struct lua_File *)a;
	struct lua_File *file2 = (struct lua_File *)b;

	return g_ascii_strcasecmp(file1->file_name, file2->file_name);
}


/**
 *
 */
GSList *add_lua_file_to_list(GSList *list, gchar *filename)
{
	struct lua_File *lua_file;

	lua_file = g_slice_alloc(sizeof(struct lua_File));
	lua_file->file_name = g_strdup(filename);
	lua_file->lua_state = init_script();

	if (load_script(lua_file->lua_state, lua_file->file_name)!=0)
		printf("Error!\n");

	list=g_slist_insert_sorted(list,
										(struct lua_File*)lua_file,
										filename_list_sortfunc);
	
	return list;
}



/**
 *
 */
GSList *get_table_of_strings(lua_State *lua,
									  gchar *script_folder,
									  gchar *table_name)
{
	GSList *list=NULL;
	
	if (lua) {
	
		lua_getglobal(lua, table_name);

		// Do we have a value?
		if (lua_isnil(lua, -1)) {
			goto EXITPOINT;
		}

		// Is it a table?
		if (!lua_istable(lua, -1)) {
			goto EXITPOINT;
		}

		lua_pushnil(lua);

		while(lua_next(lua, -2)) {
			if (lua_isstring(lua, -1)) {
				char *temp = (char *)lua_tostring(lua, -1);
				
				gchar *added_filename = g_build_path(G_DIR_SEPARATOR_S,
													script_folder,
													temp,
													NULL);
				
				list = add_lua_file_to_list(list, added_filename);
			}
			lua_pop(lua, 1);
		}
		lua_pop(lua, 1);
	}

EXITPOINT:
	
	return list;
}


/**
 *
 */
gboolean is_in_list(gchar *filename)
{
	GSList *temp_list = file_window_close_list;
	gboolean result = FALSE;
	
	
	while (temp_list) {
		
		//printf("C1 : %s\n", (gchar*)(temp_list->data));
		
		struct lua_File *temp = (struct lua_File *)(temp_list->data);
		
		gchar *list_filename = (temp->file_name);
		
		if (g_ascii_strcasecmp(list_filename, filename)==0) {
			result=TRUE;
		}
		temp_list = temp_list->next;
	}
	
	return result;
}



/**
 *
 */
int load_config(gchar *filename)
{
	lua_State *lua = NULL;
	int result = 0;
	const gchar *current_file = NULL;
	GSList *temp_window_open_file_list = NULL;
	
	// First get list of Lua files in folder - Then read variables from
	// devilspie2.lua and put the files in the required lists.
	
	gchar *script_folder = g_path_get_dirname(filename);

	GDir *dir = g_dir_open(script_folder, 0, NULL);
	if (!g_file_test(script_folder, G_FILE_TEST_IS_DIR)) {

		printf(_("script_folder isn't a folder."));
		printf("\n");
		return -1;
	}

	int total_number_of_files = 0;


	
	if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
		
		lua = init_script();
		
		if (load_script(lua, filename)!=0) {
			printf("Error loading script: %s\n", filename);
			result = -1;
			goto EXITPOINT;
		}
		
		run_script(lua);
		
		lua_getglobal(lua, "scripts_window_close");
		
		file_window_close_list = get_table_of_strings(lua, 
																	 script_folder,
																	 "scripts_window_close");
		
		// Step through the 		
		
	}

	// add the files in the folder to our linked list
	while ((current_file = g_dir_read_name(dir))) {
		
		gchar *temp_filename = g_build_path(G_DIR_SEPARATOR_S,
		                                 script_folder,
		                                 current_file,
		                                 NULL);

		// we only bother with *.lua in the folder
		if (g_str_has_suffix(current_file, ".lua")) {
			
			if (!is_in_list(temp_filename)) {
				temp_window_open_file_list = 
					add_lua_file_to_list(temp_window_open_file_list, temp_filename);
			}
			total_number_of_files++;
		}
		
		g_free(temp_filename);
	}
	
	file_window_open_list = temp_window_open_file_list; 
EXITPOINT:
	if (lua)
		done_script(lua);
	
	return result;
}

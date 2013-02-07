/**
 *	This file is part of devilspie2
 *	Copyright (C) 2005 Ross Burton, 2011-2013 Andreas Rönnquist
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

#include <string.h>

#include <stdlib.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <glib/gi18n.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <locale.h>

#include "script.h"
#include "script_functions.h"

#include "error_strings.h"

#include "config.h"


/**
 *
 */
GMainLoop *loop = NULL;

static gboolean debug = FALSE;
static gboolean emulate = FALSE;
static gboolean show_version = FALSE;

#ifdef HAVE_GTK3
	// libwnck Version Information is only availible if you have
	// libwnck 3.0 or later
	static gboolean show_wnck_version = FALSE;
#endif

static gchar *script_folder = NULL;
static gchar *temp_folder = NULL;

GFileMonitor *mon = NULL;

gchar *config_filename = NULL;

/**
 *
 */
static void load_list_of_scripts(WnckScreen *screen, WnckWindow *window,
											GSList *file_list)
{
	GSList *temp_file_list = file_list;
	// set the window to work on
	set_current_window(window);

	// for every file in the folder - load the script
	if (file_window_open_list != NULL) {

		while(temp_file_list) {
			gchar *filename = (gchar*)temp_file_list->data;

			// is it a LUA file?
			if (g_str_has_suffix((gchar*)filename,".lua")) {

				// init the script, run it
				if (!load_script(global_lua_state, filename)) {
				}

				run_script(global_lua_state);

			}
			temp_file_list=temp_file_list->next;
		}
	}
	return;

}


/**
 *
 */
static void window_opened_cb(WnckScreen *screen, WnckWindow *window)
{
	load_list_of_scripts(screen, window, file_window_open_list);
}


/**
 *
 */
static void window_closed_cb(WnckScreen *screen, WnckWindow *window)
{
	load_list_of_scripts(screen, window, file_window_close_list);
}


/**
 *
 */
void init_screens()
{
	int i;
	const int num_screens =
		gdk_display_get_n_screens(gdk_display_get_default());

	for (i=0; i<num_screens; i++) {
		WnckScreen *screen = wnck_screen_get(i);

		g_signal_connect(screen, "window-opened",
								(GCallback)window_opened_cb, NULL);
		g_signal_connect(screen, "window-closed",
								(GCallback)window_closed_cb, NULL);
	}
}


/**
 * atexit handler - kill the script
 */
void devilspie_exit()
{
	clear_file_lists();

	if (temp_folder != NULL) g_free(temp_folder);
	
	if (mon) g_object_unref(mon);
	
	if (config_filename) g_free(config_filename);
}


/**
 * handle signals that are sent to the application
 */
static void signal_handler(int sig)
{
	printf("\n");
	printf(_("Received signal:"));
	printf(" %d (%s)\n", sig, strsignal(sig));

	done_script_error_messages();

	if (sig == SIGINT) {
		exit(EXIT_FAILURE);
	}

}


/**
 *
 */
void print_list(GSList *list)
{
	GSList *temp_list;
	if (list != NULL) {
		temp_list = list;
		
		while(temp_list) {
			gchar *file_name = temp_list->data;
			
			if (file_name) {
				if (g_str_has_suffix((gchar*)file_name, ".lua")) {
					printf("%s\n", (gchar*)file_name);
				}
			}
			temp_list = temp_list->next;
		}
	}
}


/**
 *
 */
void print_script_lists()
{
	if (debug)
		printf("------------\n");

	if ((file_window_open_list == NULL) && (file_window_close_list == NULL)) {
		printf(_("No script files found in the script folder - exiting."));
		printf("\n\n");
		exit(EXIT_SUCCESS);
	}

	// If we are running debug mode - print the list of files:
	if (debug) {
		printf(_("List of LUA files handling \"%s\" events in folder:"), "window_open");
		printf("\n");
		if (file_window_open_list)
			print_list(file_window_open_list);
	}

	if (debug) {
		printf(_("List of LUA files handling \"%s\" events in folder:"), "window_close");
		printf("\n");
		if (file_window_close_list)
			print_list(file_window_close_list);
	}
}


/**
 *
 */
void folder_changed_callback(GFileMonitor *mon, 
										GFile *first_file,
										GFile *second_file, 
										GFileMonitorEvent event,
										gpointer user_data)
{
	gchar *our_filename = (gchar*)(user_data);

	// If a file is created or deleted, we need to check the file lists again
	if ((event == G_FILE_MONITOR_EVENT_CREATED) || 
		 (event == G_FILE_MONITOR_EVENT_DELETED)) {

		clear_file_lists();

		load_config(our_filename);

		if (debug)
			printf("Files in folder updated!\n - new lists:\n\n");

		print_script_lists();

		if (debug)
			printf("-----------\n");
	}

	// Also monitor if our devilspie2.lua file is changed - since it handles 
	// which files are window close or window open scripts.
	if (event == G_FILE_MONITOR_EVENT_CHANGED) {
		if (first_file) {
			gchar *short_filename = g_file_get_basename(first_file);
			
			if (g_strcmp0(short_filename, "devilspie2.lua")==0) {

				clear_file_lists();

				load_config(our_filename);

				print_script_lists();

				if (debug)
					printf("----------");
			}
		}
	}
}


/**
 * Program main entry
 */
int main(int argc, char *argv[])
{
	static const GOptionEntry options[]={
		{ "debug",			'd',	0,	G_OPTION_ARG_NONE,		&debug,
			N_("Print debug info to stdout")},
		{ "emulate",		'e',	0,	G_OPTION_ARG_NONE,		&emulate,
			N_("Don't apply any rules, only emulate execution")},
		{ "folder",			'f',	0,	G_OPTION_ARG_STRING,		&script_folder,
			N_("Search for scripts in this folder"),N_("FOLDER")},
		{ "version",		'v',	0,	G_OPTION_ARG_NONE,		&show_version,
			N_("Show Devilspie2 version and quit")},
#ifdef HAVE_GTK3
		// libwnck Version Information is only availible if you have
		// libwnck 3.0 or later
		{ "wnck-version",	'w',	0,	G_OPTION_ARG_NONE,		&show_wnck_version,
			N_("Show libwnck version and quit")},
#endif
		{ NULL }
	};

	GError *error = NULL;
	GOptionContext *context;

	// Init gettext stuff
	setlocale(LC_ALL, "");

	bindtextdomain(PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE, "");
	textdomain(PACKAGE);

	gdk_init(&argc, &argv);

	gchar *devilspie2_description =
		g_strdup_printf(_("apply rules on windows"));

	gchar *full_desc_string = g_strdup_printf("- %s", devilspie2_description);

	context=g_option_context_new(full_desc_string);
	g_option_context_add_main_entries(context,options,NULL);
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_print(_("option parsing failed: %s"), error->message);
		printf("\n");
		exit(EXIT_FAILURE);
	}

	g_free(full_desc_string);
	g_free(devilspie2_description);

	// if the folder is NULL, default to ~/.config/devilspie2/
	if (script_folder == NULL) {

		temp_folder = g_build_path(G_DIR_SEPARATOR_S,
										g_get_user_config_dir(),
										"devilspie2",
										NULL);

		// check if the folder does exist
		if (!g_file_test(temp_folder, G_FILE_TEST_IS_DIR)) {

			// - and if it doesn't, create it.
			if (g_mkdir(temp_folder, 0700) != 0) {
				printf(_("Couldn't create the default folder for devilspie2 scripts."));
				printf("\n");
				exit(EXIT_FAILURE);
			}
		}

		script_folder = temp_folder;
	}

	if (show_version) {
		printf("Devilspie2 v%s\n\n", DEVILSPIE2_VERSION);
		exit(EXIT_SUCCESS);
	}
#ifdef HAVE_GTK3
	// libwnck Version Information is only availible if you have
	// libwnck 3.0 or later
	if (show_wnck_version) {
		printf("libwnck v%d.%d.%d\n\n",
		       WNCK_MAJOR_VERSION,
		       WNCK_MINOR_VERSION,
		       WNCK_MICRO_VERSION);
		exit(EXIT_SUCCESS);
	}
#endif

#if (GTK_MAJOR_VERSION >= 3)
	if (!GDK_IS_X11_DISPLAY(gdk_display_get_default())) {
		printf(_("An X11 display is required for devilspie2."));
		printf("\n\n");
		return EXIT_FAILURE;
	}

#endif

	if (init_script_error_messages()!=0) {
		printf(_("Couldn't init script error messages!"));
		printf("\n");
		exit(EXIT_FAILURE);
	}

	config_filename =
		g_build_filename(script_folder, "devilspie2.lua", NULL);

	if (load_config(config_filename)!=0) {

		devilspie_exit();
		return EXIT_FAILURE;
	}

	if (debug) {

		if (emulate) {
			printf(_("Running devilspie2 in debug and emulate mode."));
		} else {
			printf(_("Running devilspie2 in debug mode."));
		}

		printf("\n\n");

		printf(_("Using scripts from folder: %s"), script_folder);

		printf("\n");

		devilspie2_debug = TRUE;
	}

	// Should we only run an emulation (don't modify any windows)
	if (emulate) devilspie2_emulate = emulate;
	
	GFile *directory_file;
	directory_file = g_file_new_for_path(script_folder);
//	mon = g_file_monitor_directory(directory_file, G_FILE_MONITOR_WATCH_MOUNTS,
	mon = g_file_monitor_directory(directory_file, G_FILE_MONITOR_NONE,
												NULL, NULL);
	if (!mon) {
		printf(_("Couldn't create directory monitor!"));
		printf("\n");
		return EXIT_FAILURE;
	}
	
	g_signal_connect(mon, "changed", G_CALLBACK(folder_changed_callback), (gpointer)(config_filename));

	global_lua_state = init_script();
	print_script_lists();

	if (debug) printf("------------\n");

	// remove stuff cleanly
	atexit(devilspie_exit);

	struct sigaction signal_action;

	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags = 0;
	signal_action.sa_handler = signal_handler;

	if (sigaction(SIGINT, &signal_action, NULL) == -1) {
		exit(EXIT_FAILURE);
	}

	init_screens();

	loop=g_main_loop_new(NULL, TRUE);
	g_main_loop_run(loop);

	return EXIT_SUCCESS;
}

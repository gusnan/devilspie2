/**
 *	This file is part of devilspie2
 *	Copyright (C) 2005 Ross Burton, 2011-2012 Andreas Rönnquist
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
#include <glib/gi18n.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include <lua.h>
#include <lualib.h>

#include <locale.h>

#include "script.h"
#include "script_functions.h"

#if (GTK_MAJOR_VERSION >= 3)
#define HAVE_GTK3
#endif

/**
 *
 */
GMainLoop *loop=NULL;

static gboolean debug=FALSE;
static gboolean emulate=FALSE;
static gboolean show_version=FALSE;

#ifdef HAVE_GTK3
	// libwnck Version Information is only availible if you have 
	// libwnck 3.0 or later
	static gboolean show_wnck_version=FALSE;
#endif

static gchar *script_folder=NULL;
static gchar *temp_folder=NULL;

static GSList *file_list=NULL;


/**
 *
 */
static void window_opened_cb(WnckScreen *screen, WnckWindow *window) {
	
	GSList *temp_list=file_list;
	// set the window to work on
	set_current_window(window);
	
	// for every file in the folder:	// print the list
	
	if (file_list!=NULL) {
		
		while(temp_list) {

			// is it a LUA file?
			if (g_str_has_suffix((gchar*)(temp_list->data),".lua")) {

				// init the script, run it, and clean up afterwards
				init_script();

				if (load_script((gchar*)temp_list->data)!=0) {
					return;
				}

				run_script();
				done_script();
			}
			temp_list=temp_list->next;
		}
	}
	return;
}


/**
 *
 */
void init_screens()
{
	int i;
	const int num_screens=gdk_display_get_n_screens(gdk_display_get_default());
	for (i=0;i<num_screens;i++) {
		WnckScreen *screen=wnck_screen_get(i);
		
		g_signal_connect(screen,"window_opened",(GCallback)window_opened_cb,NULL);
	}
}


/**
 * atexit handler - kill the script
 */
void devilspie_exit()
{
	done_script();
	
	if (temp_folder!=NULL) g_free(temp_folder);
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

	if (sig==SIGINT)
		exit(1);

}


/**
 *
 */
void load_scripts()
{
	GDir *dir;
	const gchar *current_file;
	
	// add all the files in the script_folder to the file_list
	dir=g_dir_open(script_folder,0,NULL);
	if (!g_file_test(script_folder,G_FILE_TEST_IS_DIR)) {

		printf(_("script_folder isn't a folder."));
		printf("\n");
		
		devilspie_exit();
		exit(EXIT_FAILURE);
	}
	
	// a temp list so we dont ruin the start of the list that is stored
	// in file_list
	GSList *temp_list=file_list;
	
	int number_of_files=0;
	
	// add the files in the folder to our linked list
	while ((current_file=g_dir_read_name(dir))) {

		// we only bother with *.lua in the folder
		if (g_str_has_suffix(current_file,".lua")) {
			temp_list=g_slist_prepend(temp_list,g_build_path(G_DIR_SEPARATOR_S,g_get_user_config_dir(),"devilspie2",current_file,NULL));
			number_of_files++;
		}
	}
	
	file_list=temp_list;
	
	g_dir_close(dir);
	
	if (number_of_files==0) {
		printf(_("No script files found in the script folder - exiting."));
		printf("\n\n");
		exit(EXIT_SUCCESS);
	}
	
	// print the list of files:
	
	if (debug) {
		printf(_("List of LUA files in folder:"));
		printf("\n");
	}
		
	if (file_list!=NULL) {
		
		while(temp_list) {

			// is it a LUA file?
			if (g_str_has_suffix((gchar*)(temp_list->data),".lua")) {
				if (debug) printf("%s\n",(gchar*)temp_list->data);
			}
			temp_list=temp_list->next;
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
	
	GError *error=NULL;
	GOptionContext *context;
	
	// Init gettext stuff
	setlocale(LC_ALL,"");
	
	bindtextdomain(PACKAGE,LOCALEDIR);
	bind_textdomain_codeset(PACKAGE,"");
	textdomain(PACKAGE);
	
	gdk_init(&argc, &argv);
	
	gchar *devilspie2_description=NULL;
	
	devilspie2_description=g_strdup_printf(_("apply rules on windows"));
	
	gchar *full_desc_string=g_strdup_printf("- %s",devilspie2_description);

	context=g_option_context_new(full_desc_string);
	g_option_context_add_main_entries(context,options,NULL);
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_print(_("option parsing failed: %s"),error->message);
		printf("\n");
		exit(EXIT_FAILURE);
	}
	
	g_free(full_desc_string);
	g_free(devilspie2_description);

	// if the folder is NULL, default to ~/.config/devilspie2/
	if (script_folder==NULL) {
		
		temp_folder=g_build_path(G_DIR_SEPARATOR_S,g_get_user_config_dir(),"devilspie2",NULL);
		
		// check if the folder does exist
		if (!g_file_test(temp_folder,G_FILE_TEST_IS_DIR)) {

			// - and if it doesn't, create it.
			if (g_mkdir(temp_folder,0700)!=0) {
				printf(_("Couldn't create the default folder for devilspie2 scripts."));
				printf("\n");
				exit(EXIT_FAILURE);
			}
		}
		
		script_folder=temp_folder;
	}
	
	if (show_version) {
		printf("Devilspie2 v%s\n\n",DEVILSPIE2_VERSION);
		exit(EXIT_SUCCESS);
	}
#ifdef HAVE_GTK3
	// libwnck Version Information is only availible if you have 
	// libwnck 3.0 or later
	if (show_wnck_version) {
		printf("libwnck v%d.%d.%d\n\n",WNCK_MAJOR_VERSION,WNCK_MINOR_VERSION,WNCK_MICRO_VERSION);
		exit(EXIT_SUCCESS);
	}
#endif
	
	if (debug) {
		
		if (emulate) {
			printf(_("Running devilspie2 in debug and emulate mode."));
		} else {
			printf(_("Running devilspie2 in debug mode."));
		}
		
		printf("\n\n");
		
		printf(_("Using scripts from folder: %s"),script_folder);
		
		printf("\n");
		
		devilspie2_debug=TRUE;
	}
	
	// Should we only run an emulation (don't modify any windows)
	if (emulate) devilspie2_emulate=emulate;
	
	if (init_script_error_messages()!=0) {
		printf(_("Couldn't init script error messages!"));
		printf("\n");
		exit(EXIT_FAILURE);
	}
	
	load_scripts();
	
	if (debug) printf("------------\n");
	
	// remove stuff cleanly
	atexit(devilspie_exit);

	struct sigaction signal_action;

	sigemptyset(&signal_action.sa_mask);
	signal_action.sa_flags = 0;
	signal_action.sa_handler = signal_handler;

	if (sigaction(SIGINT, &signal_action, NULL) == -1)
	{
		exit(EXIT_FAILURE);
	}

	init_screens();
	
	loop=g_main_loop_new(NULL,TRUE);
	g_main_loop_run(loop);

	return EXIT_SUCCESS;
}

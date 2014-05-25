/**
 *	This file is part of devilspie2
 *	Copyright (C) 2012-2013 Andreas Rönnquist
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

#include <stdio.h>

#include "error_strings.h"


gchar *no_indata_expected_error=NULL;
gchar *one_indata_expected_error=NULL;
gchar *two_indata_expected_error=NULL;
gchar *four_indata_expected_error=NULL;

gchar *number_expected_as_indata_error=NULL;
gchar *boolean_expected_as_indata_error=NULL;

gchar *string_expected_as_indata_error=NULL;

gchar *integer_greater_than_zero_expected_error=NULL;
gchar *could_not_find_current_viewport_error=NULL;

gchar *setting_viewport_failed_error=NULL;

gchar *failed_string=NULL;

/**
 *
 */
int init_script_error_messages()
{
#define ALLOCATE_ERROR_STRING _("Couldn't allocate error string!")

	no_indata_expected_error = g_strdup_printf(_("No indata expected"));
	if (!no_indata_expected_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	one_indata_expected_error = g_strdup_printf(_("One indata expected"));
	if (!one_indata_expected_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	two_indata_expected_error = g_strdup_printf(_("Two indata expected"));
	if (!two_indata_expected_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	four_indata_expected_error = g_strdup_printf(_("Four indata expected"));
	if (!four_indata_expected_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	number_expected_as_indata_error=
		g_strdup_printf(_("Number expected as indata"));
	if (!number_expected_as_indata_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	boolean_expected_as_indata_error =
		g_strdup_printf(_("Boolean expected as indata"));
	if (!boolean_expected_as_indata_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	string_expected_as_indata_error =
		g_strdup_printf(_("String expected as indata"));
	if (!string_expected_as_indata_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	integer_greater_than_zero_expected_error =
		g_strdup_printf(_("Integer greater than zero expected"));
	if (!integer_greater_than_zero_expected_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	could_not_find_current_viewport_error =
		g_strdup_printf(_("Could not find current viewport"));
	if (!could_not_find_current_viewport_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	setting_viewport_failed_error =
		g_strdup_printf(_("Setting viewport failed"));
	if (!setting_viewport_failed_error) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	failed_string = g_strdup_printf(_("Failed!"));
	if (!failed_string) {
		printf("%s", ALLOCATE_ERROR_STRING);
		printf("\n");
		return -1;
	}

	return 0;
}


/**
 *
 */
void done_script_error_messages()
{
	g_free(no_indata_expected_error);
	g_free(one_indata_expected_error);
	g_free(two_indata_expected_error);
	g_free(four_indata_expected_error);

	g_free(number_expected_as_indata_error);
	g_free(boolean_expected_as_indata_error);
	g_free(string_expected_as_indata_error);

	g_free(integer_greater_than_zero_expected_error);
	g_free(could_not_find_current_viewport_error);
	g_free(setting_viewport_failed_error);

	g_free(failed_string);
}

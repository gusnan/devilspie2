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
#ifndef __HEADER_CONFIG_
#define __HEADER_CONFIG_

/**
 *
 */
int load_config(gchar *config_filename);

void clear_file_lists();

extern GSList *file_window_open_list;
extern GSList *file_window_close_list;

// Our git version which is defined through some magic in the build system
extern const char *gitversion;

#endif /*__HEADER_CONFIG_*/

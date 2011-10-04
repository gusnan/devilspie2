/**
 *
 *	This file is part of devilspie2.
 *	Copyright (C) 2001 Havoc Pennington, 2011 Andreas Rönnquist
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
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include "xutils.h"

/**
 *
 */
void my_wnck_error_trap_push()
{
	gdk_error_trap_push();
}


/**
 *
 */
int my_wnck_error_trap_pop()
{
	XSync(gdk_x11_get_default_xdisplay(),False);
	return gdk_error_trap_pop();
}

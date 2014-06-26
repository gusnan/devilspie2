#
# Copyright © 2011-2014 Andreas Rönnquist.
# This file is distributed under the same license
# as the devilspie2 package, see COPYING file.
#

Devilspie 2
-----------

Devilspie 2 is based on the excellent program Devil's Pie by Ross Burton, and
takes a folder as indata, and checks that folder for LUA scripts. These
scripts are run each time a window is opened or closed, and the rules in them
are applied on the window.

Unfortunately the rules of the original Devils Pie are not supported.

If you don't give devilspie2 any folder with --folder, it will read LUA scripts
from the folder that it gets from the GLib function g_get_user_config_dir with
devilspie2/ added to the end - g_get_user_config_dir returns the config
directory as defined in the XDG Base Directory Specification. In most cases
would be the ~/.config/devilspie2/ folder, and this folder will be created if
it doesn't already exist. This folder is changeable with the --folder option.
If devilspie2 doesn't find any LUA files in the folder, it will stop execution.

Devilspie2 will load all the LUA files in this folder in alphabetical order.

Devilspie2 takes some options:

-h, --help          Show help options

-d, --debug         Print debug information to stdout
-e, --emulate       Don't apply any rules, but only emulate execution
-f, --folder        From which folder should we load our LUA scripts
-v, --version       Print program version and quit
-w, --wnck-version  Show version of libwnck and quit


Config
-------

A config is read from the folder where we read all scripts, and is customizable
by the --folder option. By default this folder is ~/.config/devilspie2/.

If there is a file named devilspie2.lua in this folder, it is read and it is
searched for a variable (a lua table of strings) named scripts_window_close -
The filenames in the strings in this table will be called when windows are
closed. If these variables isn't present in this file, it will be called as a
devilspie2 script file like any other.

For example:

--
scripts_window_close = {
   "file1.lua",
   "file2.lua"
}

This would make the files file1.lua and file2.lua interpreted when windows are
closing instead of when windows are opening.


Scripting
---------

You can choose to have all script functionality in one file, or you can split
it up into several, Devilspie2 runs on all LUA files in your requested
devilspie2 folder, by default ~/.config/devilspie2 - that is all files with a
.lua extension.

The scripting language used is LUA (see www.lua.org) - See FAQ at

www.lua.org/FAQ.html

, documentation at

www.lua.org/docs.html

, and tutorials at

http://lua-users.org/wiki/TutorialDirectory


The following commands are recognized by the Devilspie2 LUA interpreter:

First; a function to show some debug info


debug_print(string)

   Debug helper that prints a string to stdout. The string is only printed to
   stdout if devilspie2 is run with the --debug option, otherwise nothing will
   be printed.




Then, there's the functions to get the properties of a window, and related
information:


get_window_name()

   returns a string containing the name of the current window.


get_window_has_name()

   return true or false depending on if the window has a name or not
   (from version 0.20)


get_application_name()

   returns the application name of the current window.


get_window_geometry()

   Returns the window geometry as four numbers - x-position, y-position,
   width and height. (from version 0.16) - for example you can do something
	like this:

	x, y, width, height = get_window_geometry();
	print("X: "..x..", Y: "..y..", width: "..width..", height: "..height);


get_window_client_geometry()

   returns the window geometry excluding the window manager borders as four
   numbers, x-position, y-position, width and height. (from version 0.16) -
	see get_window_geometry for an example on how to use this function.


get_window_is_maximized()

   Returns true if the window is maximized, false otherwise. (available from
   version 0.21)


get_window_is_maximized_vertically()

   Returns true if the window is vertically maximized, false otherwise.
   (available from version 0.21)


get_window_is_maximized_horizontally()

   Returns true if the window is horizontally maximized, false otherwise.
   (available from version 0.21)


get_window_type()

   Returns the type of the window - The result type is a string, and can
   be one of the following:

   "WINDOW_TYPE_NORMAL"
   "WINDOW_TYPE_DESKTOP"
   "WINDOW_TYPE_DOCK"
   "WINDOW_TYPE_DIALOG"
   "WINDOW_TYPE_TOOLBAR"
   "WINDOW_TYPE_MENU"
   "WINDOW_TYPE_UTILITY"
   "WINDOW_TYPE_SPLASHSCREEN"

   or "WINDOW_TYPE_UNRECOGNIZED" if libwnck didn't recognize the type.

   If the function for some reason didn't have a window to work on the string
   result is "WINDOW_ERROR".

   (available from version 0.21)


get_class_instance_name()

   Gets the class instance name from the WM_CLASS Property for the current
   window. Only available on libwnck 3+, and in devilspie2 version 0.21 or
   later.


get_window_property(property)

   Returns the window property described in the property string. For a list of
   available properties, you should see the page
   http://standards.freedesktop.org/wm-spec/wm-spec-latest.html

   (Available from version 0.21)


get_window_role

   Returns a string describing the current window role of the matched window as
   defined by it's WM_WINDOW_ROLE hint.


get_window_xid

   Returns the X window id of the current window.


get_window_class

   Returns a string representing the class of the current window.


get_workspace_count

	Return the number of workspaces available (available from version 0.27)


get_screen_geometry

	Returns the screen geometry (two numbers) for the screen of the
	current window (available from version 0.29)


get_window_fullscreen
get_fullscreen

	Returns TRUE if the window is fullscreen, FALSE otherwise
	(available from version 0.32)


and the rest of the commands are used to modify the properties of the windows:


set_window_position(xpos,ypos)

   Set the position of a window.


set_window_position2(xpos,ypos)

   Set the position of a window - Compared to set_window_position, this
   function uses XMoveWindow instead of wnck_window_set_geometry which
   gives a slightly different result.
   (Available from version 0.21)


set_window_size (xsize, ysize)

   Sets the size of a window - takes xsize and ysize as parameters.


set_window_geometry (xpos, ypos, xsize, ysize)

   Sets both size and position of a window in one command. Takes four
   parameters, xpos, ypos, xsize and ysize.


set_window_geometry2 (xpos, ypos, xsize, ysize)

   Sets the window geometry just as set_window_geometry, using
   XMoveResizeWindow instead of its libwnck alternative. This results in
   different coordinates than the set_window_geometry function, and results
   are more similar to the results of the original devilspie geometry function.
   (available from version 0.21)


make_always_on_top()

   Sets a window always on top.


set_on_top()

   sets a window on top of the others. (unlike make_always_on_top, it doesn't
   lock the window in this position.)


shade()

   "Shades" a window, showing only the title-bar.


unshade()

   Unshades a window - the opposite of "shade"


maximize()

   maximizes a window


unmaximize()

   unmaximizes a window


maximize_vertically()

   maximizes the current window vertically.


maximize_horizontally()

   maximizes the current window horizontally.


minimize()

   minimizes a window


unminimize()

   unminimizes a window, that is bringing it back to screen from the minimized
   position/size.


decorate_window()

   Shows all window decoration.


undecorate_window()

   Removes all window decorations.


close_window()

	Closes the window. (Available from 0.31)


set_window_workspace(number)

   Moves a window to another workspace. The number variable starts counting at
   1.


change_workspace(number)

   Changes the current workspace to another. The number variable starts counting
   at 1.


pin_window()

   asks the window manager to put the window on all workspaces.


unpin_window()

   Asks the window manager to put window only in the currently active workspace.


stick_window()

   Asks the window manager to keep the window's position fixed on the screen,
   even when the workspace or viewport scrolls.


unstick_window()

   Asks the window manager to not have window's position fixed on the screen
   when the workspace or viewport scrolls.


set_skip_tasklist(skip)

   Set this to true if you would like the window to skip listing in your
   tasklist. Takes a boolean(true or false) as value. (from version 0.16)


set_skip_pager(skip)

   Set this to true if you would like the window to skip listing in your pager.
   Takes a boolean(true or false) as value. (from version 0.16)


set_window_above([above])

   Set the current window above all normal windows. (available from version
   0.21)
   Optionally takes a boolean(true or false) as value.


set_window_below([below])

   Set the current window below all normal windows. (available from version
   0.21)
   Optionally takes a boolean(true or false) as value.


set_window_fullscreen(fullscreen)

   Asks the window manager to set the fullscreen state of the window according
   to the fullscreen boolean. (available from version 0.24)


set_viewport(viewport)

   Moves the window to the requested viewport - Counting starts at number 1.
   (Available from version 0.25)


center()

   Centers the current window on the current workspace.
   (Available from version 0.26)


set_opacity(value)
set_window_opacity(value)

	Sets the window opacity, takes a float value, 1.0 = completely opaque,
	0.0, completely see-through. Both set_opacity and set_window_opacity
	will do the same thing.
	(Available from version 0.28, set_window_opacity from 0.29)


set_window_type(type)

	Sets the window type, according to _NET_WM_WINDOW_TYPE. The allowed types
	are the standard _NET_WM ones (formatted as a string):

	"_NET_WM_WINDOW_TYPE_DESKTOP"
	"_NET_WM_WINDOW_TYPE_DOCK"
	"_NET_WM_WINDOW_TYPE_TOOLBAR"
	"_NET_WM_WINDOW_TYPE_MENU"
	"_NET_WM_WINDOW_TYPE_UTILITY"
	"_NET_WM_WINDOW_TYPE_SPLASH"
	"_NET_WM_WINDOW_TYPE_DIALOG"
	"_NET_WM_WINDOW_TYPE_NORMAL"

	or shorter versions of the same values

	"WINDOW_TYPE_DESKTOP"
	"WINDOW_TYPE_DOCK"
	"WINDOW_TYPE_TOOLBAR"
	"WINDOW_TYPE_MENU"
	"WINDOW_TYPE_UTILITY"
	"WINDOW_TYPE_SPLASH"
	"WINDOW_TYPE_DIALOG"
	"WINDOW_TYPE_NORMAL"

	(Function is available from version 0.28)

focus()
focus_window()

	Focuses the current window. (Function is available from version 0.30)


set_window_strut(left, right, top, bottom)

	Set the reserved area at the borders of the desktop for a docking area such
	as a taskbar or a panel (available from version 0.32)


Please note that strings comparisons are case sensitive, comparing
"SomeProgram" with "someprogram" will not report equality.


Simple script example
---------------------


---------------------8<---------------------

-- the debug_print command does only print anything to stdout
-- if devilspie2 is run using the --debug option
debug_print("Window Name: " .. get_window_name());
debug_print("Application name: " .. get_application_name())

-- I want my Xfce4-terminal to the right on the second screen of my two-monitor
-- setup. (String comparison are case sensitive, please note this when
-- creating rule scripts.)
if (get_window_name()=="Terminal") then
   -- x,y, xsize, ysize
   set_window_geometry(1600,300,900,700);
end

-- Make Iceweasel always start maximized.
if (get_application_name()=="Iceweasel") then
   maximize();
end

--------------------->8---------------------


Translations
------------

Devilspie2 is now translatable using gettext - see README.translators for more
information.


Authors
-------

see the AUTHORS file


Contact
-------

Author: Andreas Rönnquist
E-mail: devilspie2 (at) gusnan (dot) se
Homepage: http://www.gusnan.se/devilspie2
Mailinglist: devilspie2-discuss@nongnu.org, 
             https://lists.nongnu.org/mailman/listinfo/devilspie2-discuss

IRC: #devilspie2 on irc.freenode.net

#
#
#	This file is part of Devilspie2.
#	Copyright 2011 Andreas Rönnquist
#
#	Devilspie2 is free software: you can redistribute it and/or modify
#	it under the terms of the GNU Lesser General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	Devilspie2 is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU Lesser General Public License for more details.
#
#	You should have received a copy of the GNU Lesser General Public License
#	along with Devilspie2.  If not, see <http://www.gnu.org/licenses/>.
#
#

# Try to find wnck library and include path.
# Once done this will define
#
# WNCK_FOUND
# WNCK_INCLUDE_DIR
# WNCK_LIBRARY
# 

FIND_PATH( WNCK_INCLUDE_DIR libwnck/libwnck.h
	$ENV{PROGRAMFILES}/wnck/include
	$ENV{PROGRAMFILES}
	/usr/include
	/usr/include/libwnck-1.0
	/usr/local/include
	/usr/local/include/libwnck-1.0
	/sw/include
	/opt/local/include
	DOC "The directory where libwnck/libwnck.h resides")

FIND_LIBRARY( WNCK_LIBRARY
	NAMES WNCK wnck wnck-1 libwnck libwnck-1
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The wnck library")

IF (WNCK_INCLUDE_DIR)
	SET( WNCK_FOUND 1 CACHE STRING "Set to 1 if WNCK is found, 0 otherwise")
ELSE (WNCK_INCLUDE_DIR)
	SET( WNCK_FOUND 0 CACHE STRING "Set to 1 if WNCK is found, 0 otherwise")
ENDIF (WNCK_INCLUDE_DIR)

IF (WNCK_FOUND)
   IF (NOT WNCK_FIND_QUIETLY)
      MESSAGE(STATUS "Found WNCK: ${WNCK_LIBRARY}")
   ENDIF (NOT WNCK_FIND_QUIETLY)
ENDIF (WNCK_FOUND)

MARK_AS_ADVANCED( WNCK_FOUND )

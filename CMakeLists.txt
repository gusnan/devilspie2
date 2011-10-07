# 
# 
# This file is part of devilspie2.
# Copyright 2011 Andreas Rönnquist
# 
# devilspie2 is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# devilspie2 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with devilspie2.  If not, see <http://www.gnu.org/licenses/>.
# 
# 

project(devilspie2)

cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

set(EXECUTABLE_NAME devilspie2)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

message(STATUS "cmake module path:" ${CMAKE_SOURCE_DIR}/cmake)

option (DEBUG "Build Debug executable" off)

if (DEBUG)
	message(STATUS "Building Debug executable")
	set(CMAKE_BUILD_TYPE "Debug")
	add_definitions(-D_DEBUG)

	add_definitions(-Wno-write-strings)
	# add_definitions(-Wno-missing-field-initializers)
	add_definitions(-Wall)
	
	#add_definitions(-Wc++-compat)

else (DEBUG)
	message(STATUS "Building Release executable")
	add_definitions(-fomit-frame-pointer)
	
	add_definitions(-Wall)
	
endif (DEBUG)

add_definitions(-DGTK_DISABLE_DEPRECATED)
add_definitions(-DGDK_DISABLE_DEPRECATED)
add_definitions(-DGSEAL_ENABLE)


Find_Package ( GTK2 REQUIRED )
if (NOT GTK2_FOUND)
	message(FATAL_ERROR "GTK not found!")
endif (NOT GTK2_FOUND)

Find_Package ( WNCK REQUIRED )
if (NOT WNCK_FOUND)
	message(FATAL_ERROR "WNCK not found!")
endif (NOT WNCK_FOUND)

Find_Package(Lua51 REQUIRED)
if (NOT LUA51_FOUND)
	message(FATAL_ERROR "LUA not found!")
endif (NOT LUA51_FOUND)

include_directories(include	${GTK2_INCLUDE_DIRS} 
										${GTK2_GDK_INCLUDE_DIR} 
										${WNCK_INCLUDE_DIR} 
										${LUA_INCLUDE_DIR})

message(STATUS "System name:${CMAKE_SYSTEM_NAME}")

set(SOURCES
	src/devilspie2.c
	src/script.c
	src/script.h
	src/script_functions.c
	src/script_functions.h
	src/xutils.c
	src/xutils.h
)

link_libraries (
	${GTK2_GDK_LIBRARY} 
	#${GTK2_GTK_LIBRARY} 
	#gthread-2.0 
	gobject-2.0 
	X11 
	glib-2.0 
	${WNCK_LIBRARY}
	${LUA_LIBRARIES}
)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

#
# Installation stuff:
#

install(FILES devilspie2.1 DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
install(FILES README DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/devilspie2/)

install_targets(/bin ${EXECUTABLE_NAME})

# can probably go lower
CMake_Minimum_Required (VERSION 3.0)

# any supplementary stuff..
 
Project(3df)

#Set_Property(GLOBAL PROPERTY USE_FOLDERS ON)
List(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

Find_Package(Allegro COMPONENTS
            image color font ttf dialog audio acodec video)
if(WIN32)
    if(NOT ALLEGRO_FOUND)
		Message(FATAL_ERROR "The allegro package must be placed somewhere in this directory tree.  The suggested method is to get the command line version of nuget and run 'nuget install Allegro' from the top level directory.")
    endif()
else()
	Message(FATAL_ERROR, "blah about how to set up allegro")
endif()

# if dr_data.h ends up being the only file read from the dfhack directory,
# search for it specifically
File(GLOB_RECURSE DFHACK_FILE FOLLOW_SYMLINKS dfhack.init-example)
#Message("DFF 1 = ${DFHACK_FILE}")
Get_Filename_Component(DFHACK "${DFHACK_FILE}" DIRECTORY CACHE)
Set_Property(CACHE DFHACK PROPERTY TYPE PATH)
Set_Property(CACHE DFHACK
             PROPERTY HELPSTRING "DFHack source directory (plugins/dr_data.h)")

Add_Subdirectory(src)


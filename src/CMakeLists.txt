CMake_Minimum_Required (VERSION 3.0)

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/clsocket/CMakeLists.txt)
#   FIXME  attempt to run the submodule update ourselves first
    Message(FATAL_ERROR "clsocket/CMakeLists.txt not found.  This probably means the clsocket submodule needs to be updated which can be done with 'git submodule update --init' from the 3df root directory.")
endif()

Include_Directories(${ALLEGRO_INCLUDE_DIRS}
                    ${DFHACK}
                    clsocket/src)

OPTION(CLSOCKET_SHARED "Build clsocket lib as shared." OFF)
OPTION(CLSOCKET_DEP_ONLY "Build clsocket as a dependency." ON)
Add_Subdirectory(clsocket)

Set(3DF_SOURCE
    "main.cpp"
    "dr_connection.cpp"
   )

Set(3DF_HEADER
    "dr_connection.h"
   )
# shouldn't need this
#Set_Source_Files_Properties(${3DF_HEADER} PROPERTIES HEADER_FILE_ONLY TRUE)

# This is discouraged, for.. some reason.
# Without this, FindAllegro needs to set ALLEGRO_LIBRARIES with full paths
Link_Directories(${ALLEGRO_LIBRARY})

Add_Executable(3df ${3DF_SOURCE} ${3DF_HEADER})
#Set_Target_Properties(3df PROPERTIES FOLDER 3df)

Target_Link_Libraries(3df ${ALLEGRO_LIBRARIES} clsocket)

Source_Group("Source Files" FILES ${3DF_SOURCE})
Source_Group("Header Files" FILES ${3DF_HEADER})


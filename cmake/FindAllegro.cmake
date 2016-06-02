#   This currently only (mostly) handles Allegro obtained via nuget on Windows.

#   If found, will define:
#   ALLEGRO_FOUND
#   ALLEGRO_INCLUDE_DIRS
#   ALLEGRO_LIBRARIES

# First, look for Allegro.targets which ?only? the nuget package uses
File(GLOB_RECURSE ALLEGRO_TARGETS FOLLOW_SYMLINKS "Allegro.targets")
#Message("ATs = ${ALLEGRO_TARGETS}")
if((EXISTS ${ALLEGRO_NATIVE}) OR (EXISTS ${ALLEGRO_TARGETS}))
    Get_Filename_Component(ALLEGRO_NATIVE ${ALLEGRO_TARGETS} DIRECTORY CACHE)
#Message("AN = ${ALLEGRO_NATIVE}")
    Set(ALLEGRO_INCLUDE_DIR "${ALLEGRO_NATIVE}/include" CACHE PATH "Allegro include directory.")
    Set(ALLEGRO_FOUND 1)
endif()

#if(NOT ALLEGRO_FOUND)
# now try looking for the header file
#File(GLOB_RECURSE ALLEGRO_HEADER FOLLOW_SYMLINKS "allegro.h")
#    Get_Filename_Component(ALLEGRO_VER_DIR ${ALLEGRO_HEADER} DIRECTORY)
#Message("avd = ${ALLEGRO_VER_DIR}")
#    String(REGEX REPLACE "/[^/]*$" "" ALLEGRO_INCLUDE_DIR ${ALLEGRO_VER_DIR})
#Message("aid = ${ALLEGRO_INCLUDE_DIR}")
#Set_Property(CACHE ALLEGRO_INCLUDE_DIR PROPERTY TYPE PATH)
#    String(REGEX REPLACE "/[^/]*$" "" ALLEGRO_NATIVE ${ALLEGRO_INCLUDE_DIR})
#Message("ad = ${ALLEGRO_INCLUDE_DIR}")
#endif()

# Now setup the libraries based on any selected addons

if(ALLEGRO_FOUND)

# FIXME  logic to choose the arch here?
    Set(ALLEGRO_ARCH "v140/x64" CACHE STRING
        "Architecture to use. (relative from the native directory)")
# FIXME  generate based on the available directories
    Set_Property(CACHE ALLEGRO_ARCH PROPERTY
                 STRINGS "v140/x64" "v140/win32" "v120/x64" "v120/win32")

    Set(ALLEGRO_LIBRARY "${ALLEGRO_NATIVE}/${ALLEGRO_ARCH}/lib"
        CACHE PATH "Directory with Allegro libraries.")
#Message("AL: ${ALLEGRO_LIBRARY}")
    Set(ALLEGRO_LIBS "allegro;allegro_main")
#    Set(ALLEGRO_LIBS "${ALLEGRO_LIBRARY}/allegro;${ALLEGRO_LIBRARY}/allegro_main")

    ForEach(addon ${Allegro_FIND_COMPONENTS})
# FIXME  check that these actually match
        Find_Library(addonlib "allegro_${addon}"
                     PATHS "${ALLEGRO_LIBRARY}"
                     NO_DEFAULT_PATH)
        if(NOT "${addonlib}" STREQUAL "addonlib-NOTFOUND")
            List(APPEND ALLEGRO_LIBS "allegro_${addon}")
#            List(APPEND ALLEGRO_LIBS "${addonlib}")
        else()
            Message(WARNING "Allegro addon library '${addon}' not found")
        endif()
#Message("ALs 2: ${ALLEGRO_LIBS}")
    EndForEach()
    UnSet(addonlib CACHE)
    UnSet(addonlib)

#   make sure this matches the starting value
    if("${ALLEGRO_LIBS}" STREQUAL "allegro;allegro_main")
#    if("${ALLEGRO_LIBS}" STREQUAL "${ALLEGRO_LIBRARY}/allegro;${ALLEGRO_LIBRARY}/allegro_main")
        Message(WARNING_AUTHOR "No Allegro addons enabled.\nSet with: Find_Package(Allegro COMPONENTS image audio ...)")
    endif()

    Set(ALLEGRO_LIBRARIES "${ALLEGRO_LIBS}"
        CACHE STRING "Allegro libraries and selected addons.")
    Set(ALLEGRO_INCLUDE_DIRS ${ALLEGRO_INCLUDE_DIR}
        CACHE STRING "Allegro include directories.")
    Mark_As_Advanced(ALLEGRO_NATIVE
                     ALLEGRO_INCLUDE_DIR
                     ALLEGRO_LIBRARY)

endif()


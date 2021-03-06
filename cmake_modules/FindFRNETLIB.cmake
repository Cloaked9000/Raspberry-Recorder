find_path(FRNETLIB_INCLUDE_DIR
        NAMES frnetlib
        PATH_SUFFIXES include
        HINTS ${FRNETLIB_ROOT})

find_library(FRNETLIB_LIBRARY
        NAMES frnetlib
        PATH_SUFFIXES lib
        HINTS ${FRNETLIB_ROOT})


if(FRNETLIB_INCLUDE_DIR AND FRNETLIB_LIBRARY)
    set(FRNETLIB_FOUND TRUE)
    set(FRNETLIB_LIBRARIES ${FRNETLIB_LIBRARY})
endif()

if(FRNETLIB_FOUND)
    if(NOT FRNETLIB_FIND_QUIETLY)
        message(STATUS "Found FRNETLIB: ${FRNETLIB_LIBRARIES}")
    endif()
else()
    if(FRNETLIB_FIND_REQUIRED)
        message(FATAL_ERROR "FRNETLIB was not found")
    endif()
endif()
add_definitions(-DSSL_ENABLED)
mark_as_advanced(FRNETLIB_INCLUDE_DIR FRNETLIB_LIBRARY)

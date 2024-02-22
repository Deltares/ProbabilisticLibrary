add_subdirectory(wrappers/general/src)

if ( TARGET tools)
else()
    if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
        message("Skipping python and fortran")
    else()
        if (UNIX)
            #add_subdirectory(wrappers/python)
        endif()
        add_subdirectory(wrappers/Fortran/test)
    endif()

endif()



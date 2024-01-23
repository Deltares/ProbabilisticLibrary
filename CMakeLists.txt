# set minimum cmake version
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# project name and language
if(WIN32)
    #set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
    project(probLibProto LANGUAGES CSharp Fortran CXX)
else()
    project(probLibProto LANGUAGES Fortran CXX)
endif(WIN32)

if ( TARGET tools)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
    set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/modules)
endif()

cmake_policy(SET CMP0079 NEW)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-D_USE_MATH_DEFINES)
    set(CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} /check:all")
endif(WIN32)

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Werror")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror")
    set(CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -fcheck=bounds -fopenmp")
    set(CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -fopenmp")
endif (UNIX)

add_subdirectory(src)

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

enable_testing()

add_subdirectory(tests)

add_subdirectory(wrappers/general/src)

if ( TARGET tools)
    add_subdirectory(wrappers/Fortran/src)
else()
    if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "Win32")
        message("Skipping python and fortran")
    else()
        if (UNIX)
            add_subdirectory(wrappers/python)
        endif()
        add_subdirectory(wrappers/Fortran/src)

        add_subdirectory(wrappers/Fortran/test)
    endif()

endif()


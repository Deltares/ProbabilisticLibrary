# set minimum cmake version
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# project name and language
project(probabilisticLibrary LANGUAGES CXX)

option(ENABLE_Fortran "Enable Fortran" ON)
if(ENABLE_Fortran)
  enable_language(Fortran)
else()
  message(STATUS "No Fortran support")
endif()

option(CODECOV "CodeCoverage on/off" OFF)

if ( NOT TARGET tools)
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

    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif(WIN32)

if (UNIX)
    if (CODECOV)
        include(Dart)
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Werror")
    endif()
    set_property(GLOBAL PROPERTY WITH_CODECOV ${CODECOV})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror")
    set(CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} -fcheck=bounds -fopenmp")
    set(CMAKE_Fortran_FLAGS_RELEASE "${CMAKE_Fortran_FLAGS_RELEASE} -fopenmp")
endif (UNIX)

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)

enable_testing()

add_subdirectory(sources)

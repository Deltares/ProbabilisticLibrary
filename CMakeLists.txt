# set minimum cmake version

option(CODECOV "CodeCoverage on/off" OFF)
if (CODECOV)
    cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
else()
    cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
endif()

# project name and language
project(probabilisticLibrary LANGUAGES CXX)

if ( NOT TARGET tools)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

cmake_policy(SET CMP0079 NEW)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Generate and copy configuration for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)

    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
endif(WIN32)

set_property(GLOBAL PROPERTY WITH_CODECOV ${CODECOV})
if (UNIX)
    if (CODECOV)
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-instr-generate -fcoverage-mapping -w ")
        set(CMAKE_C_FLAGS_DEBUG "-g -O0 -fprofile-instr-generate -fcoverage-mapping -w ")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fopenmp=libomp ")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fopenmp=libomp ")
endif (UNIX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-instr-generate -fcoverage-mapping -w")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-instr-generate -fcoverage-mapping -w")
#set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate -fcoverage-mapping -w")

message(INFO " dbg flags = ${CMAKE_CXX_FLAGS_DEBUG}")
message(INFO " release flags = ${CMAKE_CXX_FLAGS_RELEASE}")

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)

enable_testing()

add_subdirectory(sources)

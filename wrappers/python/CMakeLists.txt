include(FetchContent)
FetchContent_Declare(
	pybind11
	GIT_REPOSITORY https://github.com/pybind/pybind11.git
	GIT_TAG        v2.6.2
	GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(pybind11)

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Werror -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror -fvisibility=hidden")
endif (UNIX)

add_library(py2problib
  STATIC
  src/zfuncwrapper.h
  src/zfuncwrapper.cpp
  src/pyReader.h
  src/pyReader.cpp
  src/pyRunLSF.h
  src/pyRunLSF.cpp
  src/runPy2Problib.h
  src/runPy2Problib.cpp
)

target_link_libraries(py2problib PRIVATE probLib pybind11::embed)

add_executable(py2problibexe
  src/py2problib.cpp
)

target_link_libraries(py2problibexe PRIVATE py2problib probLib pybind11::embed)

if(WIN32)
  target_link_options(py2problibexe PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_executable(pyDemoRunner
  test/pyDemoRunner.cpp
)
target_link_libraries(pyDemoRunner PRIVATE testutils py2problib gtest_main pybind11::embed)
if(WIN32)
  target_link_options(pyDemoRunner PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_test(
  NAME python-wrapper-tests
  COMMAND $<TARGET_FILE:pyDemoRunner>
  WORKING_DIRECTORY ../wrappers/python/demo
)

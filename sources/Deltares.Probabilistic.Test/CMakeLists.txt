if (ENABLE_UNIT_TESTS)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
  )
  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # Prevent GoogleTest from using PThreads
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

    # adds the targers: gtest, gtest_main, gmock, gmock_main
    add_subdirectory(
      ${googletest_SOURCE_DIR}
      ${googletest_BINARY_DIR}
      )

    # Silence std::tr1 warning on MSVC
    if(MSVC)
      foreach(_tgt gtest gtest_main gmock gmock_main)
        target_compile_definitions(${_tgt}
          PRIVATE
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
          )
      endforeach()
    endif()
  endif()
endif()

file(GLOB testfiles
    "*.[ch]*"
    "*/*.[ch]*"
    "Reliability/Waarts/*.[ch]*"
)

add_executable(Deltares.Probabilistic.Test ${testfiles})

set_target_properties(Deltares.Probabilistic.Test
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Deltares.Probabilistic.Test Deltares.Probabilistic gtest_main)

if(WIN32)
  target_link_options(Deltares.Probabilistic.Test PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

get_property(CODECOV GLOBAL PROPERTY WITH_CODECOV)
# codecov tests are primary run on TeamCity, so add option for xml output that TeamCity can read.
if(CODECOV)
  add_test(
    NAME Deltares-Probabilistic-unit-tests
    COMMAND $<TARGET_FILE:Deltares.Probabilistic.Test> --gtest_output=xml:test_results.xml
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
else()
  add_test(
    NAME Deltares-Probabilistic-unit-tests
    COMMAND $<TARGET_FILE:Deltares.Probabilistic.Test>
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
endif()


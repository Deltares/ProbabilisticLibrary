if (ENABLE_UNIT_TESTS)
  include(FetchContent)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
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

add_executable(Deltares.Probabilistic.Test
    unittests.cpp
    projectBuilder.h
    projectBuilder.cpp
    Reliability/testStartPointCalculator.h
    Reliability/testStartPointCalculator.cpp
    Math/testNumericSupport.h
    Math/testNumericSupport.cpp
    Combin/hohenbichler_tests.h
    Combin/hohenbichler_tests.cpp
    Combin/intEqualElements_tests.h
    Combin/intEqualElements_tests.cpp
    Combin/upscale_tests.h
    Combin/upscale_tests.cpp
    Combin/combinElements_tests.h
    Combin/combinElements_tests.cpp
    Combin/hohenbichlerCombiner_tests.h
    Combin/hohenbichlerCombiner_tests.cpp
    Distributions/testDistributions.h
    Distributions/testDistributions.cpp
    Utils/testutils.h
    Utils/testutils.cpp
)

set_target_properties(Deltares.Probabilistic.Test
  PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(Deltares.Probabilistic.Test Deltares.Probabilistic.Kernel gtest_main)

if(WIN32)
  target_link_options(Deltares.Probabilistic.Test PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_test(
  NAME Deltares-Probabilistic-unit-tests
  COMMAND $<TARGET_FILE:Deltares.Probabilistic.Test>
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

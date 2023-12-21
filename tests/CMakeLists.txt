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

add_library(testutils
  STATIC
    utils/testdata.h
    utils/testdata.cpp
    utils/testutils.h
    utils/testutils.cpp
    combin/hohenbichler_tests.h
    combin/hohenbichler_tests.cpp
)

target_link_libraries(testutils probLib gtest_main)

add_executable(problib-method-tests
    probmethodtests.cpp
    probMethods/waarts1.h
    probMethods/waarts1.cpp
    probMethods/mhwtest.h
    probMethods/mhwtest.cpp
    rootFinder/ray_search_tests.h
    rootFinder/ray_search_tests.cpp
    rootFinder/sphere_search_tests.h
    rootFinder/sphere_search_tests.cpp
    probMethods/parseAndRunTests.h
    probMethods/parseAndRunTests.cpp
)

set_target_properties(problib-method-tests
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )

target_link_libraries(problib-method-tests probLib testutils gtest_main)

if(WIN32)
  target_link_options(problib-method-tests PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_executable(problib-unittest
    unittests.cpp
    rnd/gm_tests.h
    rnd/gm_tests.cpp
    rnd/normVectorTests.h
    rnd/normVectorTests.cpp
    math/choleski_decomp_tests.h
    math/choleski_decomp_tests.cpp
    math/matmul_tests.h
    math/matmul_tests.cpp
    correlation/gaussian_correlation_tests.h
    correlation/gaussian_correlation_tests.cpp
    correlation/testTrapeziumRule.h
    correlation/testTrapeziumRule.cpp
    math/basic_math_tests.h
    math/basic_math_tests.cpp
    math/vector1D_tests.h
    math/vector1D_tests.cpp
    distrib/distributionsTests.h
    distrib/distributionsTests.cpp
    combin/combinElements_tests.h
    combin/combinElements_tests.cpp
    combin/upscale_tests.h
    combin/upscale_tests.cpp
    combin/intEqualElements_tests.h
    combin/intEqualElements_tests.cpp
    probFuncs/probNumber_tests.h
    probFuncs/probNumber_tests.cpp
    probFuncs/conversion_tests.h
    probFuncs/conversion_tests.cpp
    rootFinder/root_finder_tests.h
    rootFinder/root_finder_tests.cpp
    rootFinder/sphere_tasks_tests.h
    rootFinder/sphere_tasks_tests.cpp
    strings/plStringTests.h
    strings/plStringTests.cpp
)

set_target_properties(problib-unittest
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(problib-unittest probLib testutils gtest_main)

if(WIN32)
  target_link_options(problib-unittest PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_executable(problib-longrangetest
    longRangeTests.cpp
)

set_target_properties(problib-longrangetest
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )

target_link_libraries(problib-longrangetest probLib testutils gtest_main)

if(WIN32)
  target_link_options(problib-longrangetest PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

add_test(
  NAME problib-unit-tests
  COMMAND $<TARGET_FILE:problib-unittest>
)

add_test(
  NAME probMethods-test
  COMMAND $<TARGET_FILE:problib-method-tests>
)

add_test(
  NAME problib-longrange-tests
  COMMAND $<TARGET_FILE:problib-longrangetest>
  WORKING_DIRECTORY ..
)

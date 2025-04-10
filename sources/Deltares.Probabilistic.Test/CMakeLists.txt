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
    unittests_body.cpp
    projectBuilder.h
    projectBuilder_body.cpp
    Reliability/testStartPointCalculator.h
    Reliability/testStartPointCalculator_body.cpp
    Reliability/testReliabilityMethods.h
    Reliability/testReliabilityMethods_body.cpp
    Math/testCholeskiDecomposition.h
    Math/testCholeskiDecomposition_body.cpp
    Math/testKMean.h
    Math/testKMean_body.cpp
    Math/testMatrixInverse.h
    Math/testMatrixInverse_body.cpp
    Math/testMatrixMultiplication.h
    Math/testMatrixMultiplication_body.cpp
    Math/testNumericSupport.h
    Math/testNumericSupport_body.cpp
    Math/testRandom.h
    Math/testRandom_body.cpp
    Math/testRootfinder.h
    Math/testRootfinder_body.cpp
    Math/testVector1D.h
    Math/testVector1D_body.cpp
    Combin/hohenbichler_tests.h
    Combin/hohenbichler_tests.cpp
    Combin/intEqualElements_tests.h
    Combin/intEqualElements_tests.cpp
    Combin/upscale_tests.h
    Combin/upscale_tests.cpp
    Combin/combinElements_tests.h
    Combin/combinElements_tests.cpp
    Combin/combiner_tests.h
    Combin/combiner_tests.cpp
    Distributions/testDistributions.h
    Distributions/testDistributions_body.cpp
    Optimization/testCobyla.h
    Optimization/testCobyla_body.cpp
    Model/TestRunModel.h
    Model/TestRunModel_body.cpp
    Statistics/testStandardNormal.h
    Statistics/testStandardNormal_body.cpp
    Sensitivity/TestSensitivity.h
    Sensitivity/TestSensitivity_body.cpp
    Utils/testutils.h
    Utils/testutils_body.cpp
)

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

add_test(
  NAME Deltares-Probabilistic-unit-tests
  COMMAND $<TARGET_FILE:Deltares.Probabilistic.Test>
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

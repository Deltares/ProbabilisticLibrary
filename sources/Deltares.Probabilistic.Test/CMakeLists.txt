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

add_executable(Deltares.Probabilistic.Test
    unittests_body.cpp
    projectBuilder.h
    projectBuilder_body.cpp
    Reliability/TestStartPointCalculator.h
    Reliability/TestStartPointCalculator_body.cpp
    Reliability/TestReliabilityMethods.h
    Reliability/TestReliabilityMethods_body.cpp
    Reliability/Waarts/TestWaarts.h
    Reliability/Waarts/TestWaarts_body.cpp
    Reliability/Waarts/TestWaartsLinearResistance.h
    Reliability/Waarts/TestWaartsLinearResistance_body.cpp
    Reliability/Waarts/TestWaartsResistanceOneQuadraticTerm.h
    Reliability/Waarts/TestWaartsResistanceOneQuadraticTerm_body.cpp
    Reliability/Waarts/TestWaartsResistanceTenQuadraticTerms.h
    Reliability/Waarts/TestWaartsResistanceTenQuadraticTerms_body.cpp
    Reliability/Waarts/TestWaartsResistance25QuadraticTerms.h
    Reliability/Waarts/TestWaartsResistance25QuadraticTerms_body.cpp
    Reliability/Waarts/TestWaartsNoisyLimitState.h
    Reliability/Waarts/TestWaartsNoisyLimitState_body.cpp
    Reliability/Waarts/TestWaartsConvexFailureDomain.h
    Reliability/Waarts/TestWaartsConvexFailureDomain_body.cpp
    Reliability/Waarts/TestWaartsOblateSpheroid.h
    Reliability/Waarts/TestWaartsOblateSpheroid_body.cpp
    Reliability/Waarts/TestWaartsSaddleSurface.h
    Reliability/Waarts/TestWaartsSaddleSurface_body.cpp
    Reliability/Waarts/TestWaartsDiscontinuousLimitState.h
    Reliability/Waarts/TestWaartsDiscontinuousLimitState_body.cpp
    Reliability/Waarts/TestWaartsTwoBranches.h
    Reliability/Waarts/TestWaartsTwoBranches_body.cpp
    Reliability/Waarts/TestWaartsConcaveFailureDomain.h
    Reliability/Waarts/TestWaartsConcaveFailureDomain_body.cpp
    Reliability/Waarts/TestWaartsSeriesSystem.h
    Reliability/Waarts/TestWaartsSeriesSystem_body.cpp
    Reliability/Waarts/TestWaartsParallelSystem.h
    Reliability/Waarts/TestWaartsParallelSystem_body.cpp
    Reliability/Waarts/TestWaartsResistance25QuadraticTermsSparse.h
    Reliability/Waarts/TestWaartsResistance25QuadraticTermsSparse_body.cpp
    Math/testCholeskiDecomposition.h
    Math/testCholeskiDecomposition_body.cpp
    Math/testKMean.h
    Math/testKMean_body.cpp
    Math/testMatrix.h
    Math/testMatrix_body.cpp
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
    Combin/UpscaleTests.h
    Combin/UpscaleTests.cpp
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
    Proxies/TestProxies.h
    Proxies/TestProxies_body.cpp
    Statistics/testStandardNormal.h
    Statistics/testStandardNormal_body.cpp
    Statistics/TestCopula.h
    Statistics/TestCopula_body.cpp
    Uncertainty/TestUncertainty.h
    Uncertainty/TestUncertainty_body.cpp
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


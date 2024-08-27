get_property(fctools GLOBAL PROPERTY fc_tools)
get_property(generalLib GLOBAL PROPERTY general_lib)

add_library(unitTestTools
  STATIC
    utils/TestTools.f90
)

if ( TARGET ftnunit )
else ()
  add_library(ftnunit
    STATIC
      ftnunit/ftnunit.f90
      ftnunit/ftnunit_hooks_teamcity.f90
  )

  if (UNIX)
    target_compile_options(ftnunit PUBLIC -cpp)
  endif (UNIX)
  target_link_libraries(ftnunit ${fctools})
endif()

add_executable(ftn_unit_test_prog
    ftn_interface_tests.f90
    ftn_unit_tests.f90
)

if (UNIX)
  target_compile_options(ftn_unit_test_prog PUBLIC -cpp)
endif (UNIX)

target_link_libraries(unitTestTools ftnunit feedback ${fctools})

target_link_libraries(ftn_unit_test_prog
  Deltares.Probabilistic.CWrapper Deltares.Probabilistic.FortranWrapper Deltares.Probabilistic.Kernel
  ${fctools} ftnunit feedback feedbackDLL unitTestTools)

add_library(FLtestProbabilistic
  STATIC
    probabilistic/initProbabilisticTests.f90
    probabilistic/incompleteGammaFunctionTests.f90
    probabilistic/basicCorrelationTests.f90
    probabilistic/distributionFunctions.f90
    probabilistic/distributionFunctionsTests.f90
    probabilistic/combineElementsTests.f90
    probabilistic/performDSTests.f90
    probabilistic/performImportanceSamplingTests.f90
    probabilistic/upscaleLengthTests.f90
    probabilistic/upscalingTests.f90
    probabilistic/HohenbichlerTests.f90
    probabilistic/probabilisticTests.f90
    probabilistic/waartsFunctions.f90
    probabilistic/waartsFunctionsTests.f90
    probabilistic/probMethodsWaartsFunctionsTests.f90
    probabilistic/toolkitTestFunctions.f90
    probabilistic/performAdaptiveMCISTests.f90
    probabilistic/sparseWaartsTestFunctions.f90
)
target_link_libraries(FLtestProbabilistic ${fctools} Deltares.Probabilistic.FortranWrapper ftnunit)

add_library(FLtestProbabilisticConversions
  STATIC
    probabilisticConversions/betaFromPQmin.f90
    probabilisticConversions/conversionFunctionsTests.f90
    probabilisticConversions/steinbrecherSeries.f90
    probabilisticConversions/Fmin.f90
)
target_link_libraries(FLtestProbabilisticConversions
  ${fctools} Deltares.Probabilistic.FortranWrapper Deltares.Probabilistic.CWrapper ftnunit)

add_executable(FLunittestprogram
    unitTests/initTestSubs.f90
    unitTests/unitTestsProgram.F90
)

target_link_libraries(FLunittestprogram
    FLtestProbabilisticConversions
    FLtestProbabilistic
    Deltares.Probabilistic.CWrapper Deltares.Probabilistic.FortranWrapper
    Deltares.Probabilistic.Kernel ${fctools} feedbackDLL ftnunit)

if (WIN32)
    target_compile_options(FLtestProbabilistic PRIVATE /Qopenmp)
    target_compile_options(FLunittestprogram PRIVATE /Qopenmp)
endif()

add_test(
  NAME fortran-interface-tests
  COMMAND $<TARGET_FILE:ftn_unit_test_prog> /s
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_test(
  NAME fortran-legacy-conversion-tests
  COMMAND $<TARGET_FILE:FLunittestprogram> conv 1 /s
)

set(LEVEL 3)
if (UNIX)
  get_property(CODECOV GLOBAL PROPERTY WITH_CODECOV)
  if (CODECOV)
    set(LEVEL 1)
  endif(CODECOV)
endif(UNIX)
message(STATUS "Level tests = ${LEVEL}")

add_test(
  NAME fortran-legacy-prob-tests
  COMMAND $<TARGET_FILE:FLunittestprogram> prob ${LEVEL} 2 /s
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/unitTests"
)


add_library(unitTestTools
  STATIC
    utils/TestTools.f90
)

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
  ${fctools} ftnunit feedback feedbackDLL unitTestTools pb_wrapper_c_old)

add_library(FLtestProbabilistic
  STATIC
    probabilistic/initProbabilisticTests.f90
    probabilistic/incompleteGammaFunctionTests.f90
    probabilistic/basicCorrelationTests.f90
    probabilistic/distributionFunctions.f90
    probabilistic/distributionFunctionsTests.f90
    probabilistic/randomGeneratorTests.f90
    probabilistic/combineElementsTests.f90
    probabilistic/sampleZfunc.f90
    probabilistic/performDSTests.f90
    probabilistic/performFORMTests.f90
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
)
target_link_libraries(FLtestProbabilistic ${fctools} Deltares.Probabilistic.FortranWrapper ftnunit)

add_library(FLtestProbabilisticData
  STATIC
    probabilisticData/probabilisticDataTests.f90
    probabilisticData/correlationModelsTests.f90
)
target_link_libraries(FLtestProbabilisticData
  ${fctools} Deltares.Probabilistic.FortranWrapper Deltares.Probabilistic.CWrapper ftnunit)

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
 FLtestProbabilisticData
 FLtestProbabilistic
 Deltares.Probabilistic.CWrapper Deltares.Probabilistic.FortranWrapper
 Deltares.Probabilistic.Kernel ${fctools} feedbackDLL ftnunit pb_wrapper_c_old)

add_test(
  NAME fortran-interface-tests
  COMMAND $<TARGET_FILE:ftn_unit_test_prog> /s
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_test(
  NAME fortran-legacy-conversion-tests
  COMMAND $<TARGET_FILE:FLunittestprogram> conv 1 /s
)

add_test(
  NAME fortran-legacy-prob-tests
  COMMAND $<TARGET_FILE:FLunittestprogram> prob 3 2 /s
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/unitTests"
)


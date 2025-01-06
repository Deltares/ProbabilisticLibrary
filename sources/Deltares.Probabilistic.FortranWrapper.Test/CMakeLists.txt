get_property(fctools GLOBAL PROPERTY fc_tools)
get_property(generalLib GLOBAL PROPERTY general_lib)

add_library(unitTestTools
  STATIC
    utils/unitTestTools.f90
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

add_executable(Deltares.Probabilistic.FortranWrapper.Test
    ftn_interface_tests.f90
    ftn_unit_tests.f90
)

if (UNIX)
  target_compile_options(Deltares.Probabilistic.FortranWrapper.Test PUBLIC -cpp)
endif (UNIX)

target_link_libraries(unitTestTools ftnunit feedback ${fctools})

target_link_libraries(Deltares.Probabilistic.FortranWrapper.Test
  Deltares.Probabilistic.CWrapper Deltares.Probabilistic.FortranWrapper Deltares.Probabilistic
  ${fctools} ftnunit feedback feedbackDLL unitTestTools)

file(GLOB testFortranProbabilisticFiles "probabilistic/*.f90")
add_library(FLtestProbabilistic STATIC ${testFortranProbabilisticFiles})
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
    Deltares.Probabilistic ${fctools} feedbackDLL ftnunit)

if (WIN32)
    target_compile_options(FLtestProbabilistic PRIVATE /Qopenmp)
    target_compile_options(FLunittestprogram PRIVATE /Qopenmp)
endif()

add_test(
  NAME fortran-interface-tests
  COMMAND $<TARGET_FILE:Deltares.Probabilistic.FortranWrapper.Test> /s
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

add_test(
  NAME fortran-conversion-tests
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
  NAME fortran-probabilistic-tests
  COMMAND $<TARGET_FILE:FLunittestprogram> prob ${LEVEL} 2 /s
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/unitTests"
)


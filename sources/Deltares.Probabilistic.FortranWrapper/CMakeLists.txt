add_library(Deltares.Probabilistic.FortranWrapper
  STATIC
    interfaces/interface_gen.f90
    interfaces/interface_distributions.f90
    interfaces/interface_randomgen.f90
    interfaces/interface_probCalc.F90
    interfaces/interface_probCalcData.f90
    interfaces/interface_convert.f90
    interfaces/interface_combin.f90
    interfaces/interface_correlation.f90
    interfaces/interface_special_funcs.f90
    interfaces/f2c_tools.f90
  )

if (UNIX)
  target_compile_options(Deltares.Probabilistic.FortranWrapper PUBLIC -cpp)
endif (UNIX)

if ( TARGET tools )
  set(fctools tools)
else ()
  set(fctools fortranCommonTools)
  add_library(fortranCommonTools
  STATIC
    tools/precision.f90
    tools/errorMessages.f90
    tools/utilities.f90
    tools/equalReals.f90
)
endif()
set_property(GLOBAL PROPERTY fc_tools ${fctools})

if ( TARGET generalLib )
  set(fcgeneral generalLib)
else()
  set(fcgeneral fortranCommonGeneral)
  add_library(fortranCommonGeneral
    STATIC
      general/realloc_check.f90
      general/vectorUtilities.f90
      general/fileUtilities.f90
  )
  target_link_libraries(fortranCommonGeneral feedback)
endif()
set_property(GLOBAL PROPERTY general_lib ${generalLib})

if ( TARGET feedback )
else ()
  add_library(feedback
    STATIC
      feedback/feedback.f90
  )
endif()

if ( TARGET feedbackDLL )
else ()
  add_library(feedbackDLL
    SHARED
      feedback_dll/feedbackDLL_implementation.f90
      feedback_dll/feedbackDLL.f90
      feedback_dll/feedback_parameters.f90
  )

  if (UNIX)
    target_compile_options(feedbackDLL PRIVATE -cpp)
  else()
    target_compile_options(feedbackDLL PRIVATE /fpp /Qopenmp)
  endif()
  target_compile_definitions(feedbackDLL PRIVATE "maxParallelThreads=96")
endif()

target_link_libraries(feedback feedbackDLL ${fctools})
target_link_libraries(feedbackDLL ${fctools})
target_link_libraries(Deltares.Probabilistic.FortranWrapper
  Deltares.Probabilistic.CWrapper Deltares.Probabilistic.Kernel
  ${fctools} ${fcgeneral} feedback feedbackDLL)
target_link_libraries(Deltares.Probabilistic.CWrapper Deltares.Probabilistic.Kernel)

if ( TARGET tools )
  set(CMAKE_Fortran_FLAGS "/libs:dll /threads /nologo /Qauto /traceback /fp:source")
  set(CMAKE_Fortran_FLAGS_DEBUG "/debug:full /Od /dbglibs /warn:all /debug-parameters:used \
                                 /check:bounds /check:stack /check:pointer /check:uninit")
  set(CMAKE_Fortran_FLAGS_RELEASE "/Qfp-speculation:safe")
endif()

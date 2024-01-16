enable_language(C)
include(FortranCInterface)
FortranCInterface_VERIFY(CXX)

add_library(pb_wrapper_c STATIC "")
target_sources(pb_wrapper_c
  PRIVATE
    stringHelper.h
    stringHelper.cpp
    probCalcF2C.cpp
    ../../../../Kernels/Deltares.Probabilistic.Kernels/CWrapper/probCalcF2Cnew.cpp
    randomgen.cpp
    fortranZfunc.h
    fortranZfunc.cpp
    calcdistributionsinv.cpp
    convert.cpp
    combin.cpp
    correlation.cpp
    special_funcs.cpp
  )

  set_target_properties(pb_wrapper_c
    PROPERTIES
      CXX_STANDARD 17
      POSITION_INDEPENDENT_CODE 1
      #COMMON_LANGUAGE_RUNTIME ""
  )

target_link_libraries(pb_wrapper_c probLibPtkA)

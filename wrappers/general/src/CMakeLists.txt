enable_language(C)
include(FortranCInterface)
FortranCInterface_VERIFY(CXX)

add_library(pb_wrapper_c STATIC "")
target_sources(pb_wrapper_c
  PRIVATE
    randomgen.cpp
    calcdistributionsinv.cpp
    convert.cpp
    combin.cpp
    correlation.cpp
  )

  set_target_properties(pb_wrapper_c
    PROPERTIES
      CXX_STANDARD 17
      POSITION_INDEPENDENT_CODE 1
      #COMMON_LANGUAGE_RUNTIME ""
  )

target_link_libraries(pb_wrapper_c Deltares.Probabilistic.Kernel)

enable_language(C)
include(FortranCInterface)
FortranCInterface_VERIFY(CXX)

add_library(pb_wrapper_c STATIC "")
file(GLOB cwrapper_files
    "*.cpp"
    "../../external/wrappers/general/src/*.cpp"
)

target_sources(pb_wrapper_c
  PRIVATE
    ${cwrapper_files}
  )

  set_target_properties(pb_wrapper_c
    PROPERTIES
      CXX_STANDARD 17
      POSITION_INDEPENDENT_CODE 1
  )

target_link_libraries(pb_wrapper_c Deltares.Probabilistic.Kernel)

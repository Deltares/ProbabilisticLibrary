file(GLOB cwrapper_files
    "*.cpp"
)

add_library(Deltares.Probabilistic.CWrapper
  STATIC
    ${cwrapper_files}
)

set_target_properties(Deltares.Probabilistic.CWrapper
  PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE 1
)

target_link_libraries(Deltares.Probabilistic.CWrapper Deltares.Probabilistic.Kernel)

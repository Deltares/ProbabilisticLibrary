add_library(pb_wrapper_c_old STATIC "")
file(GLOB cwrapper_files_old
    "*.cpp"
)

target_sources(pb_wrapper_c_old
  PRIVATE
    ${cwrapper_files_old}
  )

  set_target_properties(pb_wrapper_c_old
    PROPERTIES
      CXX_STANDARD 17
      POSITION_INDEPENDENT_CODE 1
  )

target_link_libraries(pb_wrapper_c_old Deltares.Probabilistic.Kernel)

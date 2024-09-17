if(WIN32)
  file(GLOB ptkcli_files
      "Model/*.cpp"
      "Reliability/*.cpp"
      "Statistics/*.cpp"
      "Sensitivity/*.cpp"
      "Utils/*.cpp"
  )

  add_library(Deltares.Probabilistics.Wrappers
    SHARED
      ${ptkcli_files}
  )
  target_link_options(Deltares.Probabilistics.Wrappers PRIVATE "$<$<CONFIG:Debug>:/Debug>")
  set_target_properties(Deltares.Probabilistics.Wrappers
    PROPERTIES
      CXX_STANDARD 20
      POSITION_INDEPENDENT_CODE 1
      COMMON_LANGUAGE_RUNTIME ""
    )
  target_link_libraries(Deltares.Probabilistics.Wrappers Deltares.Probabilistic)
endif()


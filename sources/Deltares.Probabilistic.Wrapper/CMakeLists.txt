if(WIN32)
  file(GLOB ptkcli_files
      "Model/*.cpp"
      "Reliability/*.cpp"
      "Statistics/*.cpp"
      "Sensitivity/*.cpp"
      "Utils/*.cpp"
  )

  add_library(Deltares.Probabilistic.Wrapper
    SHARED
      ${ptkcli_files}
  )
  target_link_options(Deltares.Probabilistic.Wrapper PRIVATE "$<$<CONFIG:Debug>:/Debug>")
  set_target_properties(Deltares.Probabilistic.Wrapper
    PROPERTIES
      CXX_STANDARD 20
      POSITION_INDEPENDENT_CODE 1
      COMMON_LANGUAGE_RUNTIME ""
    )
  target_link_libraries(Deltares.Probabilistic.Wrapper Deltares.Probabilistic)
endif()


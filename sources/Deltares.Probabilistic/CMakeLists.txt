file(GLOB ptkcpp_files
    "Combine/*.cpp"
    "Math/*.cpp"
    "Math/ASA/*.cpp"
    "Math/Randomizers/*.cpp"
    "Math/RootFinders/*.cpp"
    "Model/*.cpp"
    "Reliability/*.cpp"
    "Sensitivity/*.cpp"
    "Statistics/*.cpp"
    "Statistics/Distributions/*.cpp"
    "Optimization/*.cpp"
    "Utils/*.cpp"
)

add_library(Deltares.Probabilistic.Kernel
    SHARED
    ${ptkcpp_files}
)
if (WIN32)
    target_link_options(Deltares.Probabilistic.Kernel PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()
set_target_properties(Deltares.Probabilistic.Kernel
  PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE 1
    COMMON_LANGUAGE_RUNTIME ""
  )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(Deltares.Probabilistic.Kernel PUBLIC OpenMP::OpenMP_CXX)
endif()

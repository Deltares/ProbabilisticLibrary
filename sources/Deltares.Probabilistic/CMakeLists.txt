file(GLOB ptkcpp_files
    "Math/*.cpp"
    "Math/ASA/*.cpp"
    "Math/Randomizers/*.cpp"
    "Math/RootFinders/*.cpp"
    "Model/*.cpp"
    "Reliability/*.cpp"
    "Statistics/*.cpp"
    "Statistics/Distributions/*.cpp"
    "Utils/*.cpp"
)

add_library(Deltares.Probabilistic.Kernel
  STATIC
    ${ptkcpp_files}
)
target_link_options(Deltares.Probabilistic.Kernel PRIVATE "$<$<CONFIG:Debug>:/Debug>")
set_target_properties(Deltares.Probabilistic.Kernel
  PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE 1
  )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(Deltares.Probabilistic.Kernel PUBLIC OpenMP::OpenMP_CXX)
endif()

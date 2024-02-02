
file(GLOB ptkcpp_files
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/ASA/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/Randomizers/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/RootFinders/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Statistics/Distributions/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Utils/*.cpp"
)
file(GLOB ptkcli_files
    "../../Kernels/Deltares.Probabilistics.Wrappers/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Utils/*.cpp"
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

if(WIN32)
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
  target_link_libraries(Deltares.Probabilistics.Wrappers Deltares.Probabilistic.Kernel)
endif()



file(GLOB ptkcpp_files
    "../../Kernels/Deltares.Probabilistic/Math/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Math/ASA/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Math/Randomizers/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Math/RootFinders/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Statistics/Distributions/*.cpp"
    "../../Kernels/Deltares.Probabilistic/Utils/*.cpp"
)
file(GLOB ptkcli_files
    "../../Kernels/Deltares.Probabilistic.Wrapper/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Wrapper/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Wrapper/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Wrapper/Utils/*.cpp"
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


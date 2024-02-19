
file(GLOB ptkcpp_files
    "../../sources/Deltares.Probabilistic/Math/*.cpp"
    "../../sources/Deltares.Probabilistic/Math/ASA/*.cpp"
    "../../sources/Deltares.Probabilistic/Math/Randomizers/*.cpp"
    "../../sources/Deltares.Probabilistic/Math/RootFinders/*.cpp"
    "../../sources/Deltares.Probabilistic/Model/*.cpp"
    "../../sources/Deltares.Probabilistic/Reliability/*.cpp"
    "../../sources/Deltares.Probabilistic/Statistics/*.cpp"
    "../../sources/Deltares.Probabilistic/Statistics/Distributions/*.cpp"
    "../../sources/Deltares.Probabilistic/Utils/*.cpp"
)
file(GLOB ptkcli_files
    "../../sources/Deltares.Probabilistic.Wrapper/Model/*.cpp"
    "../../sources/Deltares.Probabilistic.Wrapper/Reliability/*.cpp"
    "../../sources/Deltares.Probabilistic.Wrapper/Statistics/*.cpp"
    "../../sources/Deltares.Probabilistic.Wrapper/Utils/*.cpp"
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


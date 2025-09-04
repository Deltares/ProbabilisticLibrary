file(GLOB probabilistic_files
    "Combine/*.cpp"
    "Math/*.cpp"
    "Math/ASA/*.cpp"
    "Math/Randomizers/*.cpp"
    "Math/RootFinders/*.cpp"
    "Model/*.cpp"
    "Reliability/*.cpp"
    "Sensitivity/*.cpp"
    "Uncertainty/*.cpp"
    "Statistics/*.cpp"
    "Statistics/Distributions/*.cpp"
    "Optimization/*.cpp"
    "Server/*.cpp"
    "Utils/*.cpp"
    "Combine/*.h"
    "Math/*.h"
    "Math/ASA/*.h"
    "Math/Randomizers/*.h"
    "Math/RootFinders/*.h"
    "Model/*.h"
    "Reliability/*.h"
    "Uncertainty/*.h"
    "Sensitivity/*.h"
    "Statistics/*.h"
    "Statistics/Distributions/*.h"
    "Optimization/*.h"
    "Server/*.h"
    "Utils/*.h"
)

add_library(Deltares.Probabilistic
    STATIC
    ${probabilistic_files}
)
if (WIN32)
    target_link_options(Deltares.Probabilistic PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()
set_target_properties(Deltares.Probabilistic
  PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE 1
  )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(Deltares.Probabilistic PUBLIC OpenMP::OpenMP_CXX)
endif()

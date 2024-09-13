file(GLOB kernel_files
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

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_number.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_product_version.cmake")
set_version_numbers("../version_number.ini")
generate_product_version(
   VersionFilesOutputVariable
   NAME "Streams kernel"
   ICON "${CMAKE_CURRENT_SOURCE_DIR}/../streams.ico"
   COMPANY_NAME "Deltares"
   VERSION_MAJOR ${ver_major}
   VERSION_MINOR ${ver_minor}
   VERSION_PATCH ${ver_patch}
   ORIGINAL_FILENAME ${ver_git_commit_hash}
)

add_library(Deltares.Probabilistic.Kernel
    SHARED
    ${kernel_files}
    ${VersionFilesOutputVariable}
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

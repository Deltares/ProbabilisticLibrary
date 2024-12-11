file(GLOB cwrapper_files
    "*.cpp"
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_number.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate_product_version.cmake")
set_version_numbers("../version_number.ini")
generate_product_version(
   VersionFilesOutputVariable
   NAME "Probabilistic Library c-wrapper"
   ICON "${CMAKE_CURRENT_SOURCE_DIR}/../problib.ico"
   COMPANY_NAME "Deltares"
   VERSION_MAJOR ${ver_major}
   VERSION_MINOR ${ver_minor}
   VERSION_PATCH ${ver_patch}
   ORIGINAL_FILENAME ${ver_git_commit_hash}
)

add_library(Deltares.Probabilistic.CWrapper
  SHARED
    ${cwrapper_files}
    ${VersionFilesOutputVariable}
)

set_target_properties(Deltares.Probabilistic.CWrapper
  PROPERTIES
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE 1
)

target_link_libraries(Deltares.Probabilistic.CWrapper Deltares.Probabilistic)

if(WIN32)
    target_link_options(Deltares.Probabilistic.CWrapper PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()



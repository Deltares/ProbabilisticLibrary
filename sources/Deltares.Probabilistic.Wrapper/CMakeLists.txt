if(WIN32)
  file(GLOB problibcli_files
      "Model/*.cpp"
      "Reliability/*.cpp"
      "Statistics/*.cpp"
      "Sensitivity/*.cpp"
      "Server/*.cpp"
      "Utils/*.cpp"
      "Model/*.h"
      "Reliability/*.h"
      "Statistics/*.h"
      "Sensitivity/*.h"
      "Server/*.h"
      "Utils/*.h"
  )

  include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake_vn/version_number.cmake")
  include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake_vn/generate_product_version.cmake")
set_version_numbers("../version_number.ini")
generate_product_version(
	VersionFilesOutputVariableDotNet
	NAME "Probabilistic Library CLI-wrapper"
   ICON "${CMAKE_CURRENT_SOURCE_DIR}/../problib.ico"
   COMPANY_NAME "Deltares"
   VERSION_MAJOR ${ver_major}
   VERSION_MINOR ${ver_minor}
   VERSION_PATCH ${ver_patch}
   ORIGINAL_FILENAME ${ver_git_commit_hash}
)

  add_library(Deltares.Probabilistic.Wrapper
    SHARED
      ${problibcli_files}
      ${VersionFilesOutputVariableDotNet}
  )
  target_link_options(Deltares.Probabilistic.Wrapper PRIVATE "$<$<CONFIG:Debug>:/Debug>")
  target_compile_options(Deltares.Probabilistic.Wrapper PUBLIC "/bigobj")
  set_target_properties(Deltares.Probabilistic.Wrapper
    PROPERTIES
      CXX_STANDARD 20
      POSITION_INDEPENDENT_CODE 1
      COMMON_LANGUAGE_RUNTIME ""
    )
  target_link_libraries(Deltares.Probabilistic.Wrapper Deltares.Probabilistic)
endif()


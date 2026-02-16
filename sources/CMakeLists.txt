add_subdirectory(Deltares.Probabilistic)

add_subdirectory(Deltares.Probabilistic.CWrapper)

get_property(FORSONAR GLOBAL PROPERTY FOR_SONAR)

if ( NOT TARGET tools)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if ( NOT FORSONAR)
      add_subdirectory(Deltares.Probabilistic.Net)
    endif()
  endif()
endif()

add_subdirectory(Deltares.Probabilistic.Test)


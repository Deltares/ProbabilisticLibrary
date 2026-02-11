add_subdirectory(Deltares.Probabilistic)

add_subdirectory(Deltares.Probabilistic.CWrapper)

if ( NOT TARGET tools)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_subdirectory(Deltares.Probabilistic.Net)
  endif()
endif()

add_subdirectory(Deltares.Probabilistic.Test)


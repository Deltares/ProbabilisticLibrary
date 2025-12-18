add_subdirectory(Deltares.Probabilistic)

add_subdirectory(Deltares.Probabilistic.CWrapper)

if(WIN32)
  if ( NOT TARGET tools)
    add_subdirectory(Deltares.Probabilistic.Wrapper)
  endif()
endif(WIN32)

add_subdirectory(Deltares.Probabilistic.Test)

if(ENABLE_Fortran)
    add_subdirectory(Deltares.Probabilistic.FortranWrapper)
endif()

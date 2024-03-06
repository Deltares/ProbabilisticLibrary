add_subdirectory(Deltares.Probabilistic)

add_subdirectory(Deltares.Probabilistic.CWrapper)

if(WIN32)
  add_subdirectory(Deltares.Probabilistic.Wrapper)
endif(WIN32)

add_subdirectory(Deltares.Probabilistic.FortranWrapper)

add_subdirectory(Deltares.Probabilistic.FortranWrapper.Test)

add_subdirectory(Deltares.Probabilistic.Test)

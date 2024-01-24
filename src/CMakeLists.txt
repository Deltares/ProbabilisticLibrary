set (extern_files
    extern/asa063.hpp
    extern/asa063.cpp
    extern/asa109.hpp
    extern/asa109.cpp
    extern/asa239.hpp
    extern/asa239.cpp
    extern/asa241.hpp
    extern/asa241.cpp
    extern/dgammaDs.h
    extern/dgammaDs.cpp
)

set (rnd_files
    rnd/rndDefs.h
    rnd/rndDefs.cpp
    rnd/randomForSampling.h
    rnd/randomForSampling.cpp
    rnd/defaultRandomDouble.h
    rnd/defaultRandomDouble.cpp
    rnd/mersenne_twister.h
    rnd/mersenne_twister.cpp
    rnd/george_marsaglia.h
    rnd/george_marsaglia.cpp
)

set (utils_files
    utils/basic_math.h
    utils/vector1D.h
    utils/vector1D.cpp
    utils/matrix.h
    utils/matrix.cpp
    utils/cholesky_decomp.cpp
    utils/probLibException.h
    utils/probLibString.h
    utils/probLibString.cpp
    utils/xyPair.h
)

set (distributions_files
    distributions/distrib.h
    distributions/distrib.cpp
    distributions/normalDist.h
    distributions/normalDist.cpp
    distributions/logNormalDist.h
    distributions/logNormalDist.cpp
    distributions/uniformDist.h
    distributions/mtable.h
    distributions/mtable.cpp
    distributions/combine_x1x2_mtable.h
    distributions/combine_x1x2_mtable.cpp
    distributions/truncatedNormal.h
    distributions/truncatedNormal.cpp
    distributions/distrib.h
    distributions/gumble.h
    distributions/gumble.cpp
    distributions/weibull.h
    distributions/weibull.cpp
    distributions/distBeta.h
    distributions/distBeta.cpp
    distributions/triangleDist.h
    distributions/triangleDist.cpp
    distributions/paretoDist.h
    distributions/paretoDist.cpp
    distributions/rayleighDist.h
    distributions/rayleighDist.cpp
    distributions/exponentialDist.h
    distributions/exponentialDist.cpp
    distributions/parseDistribution.h
    distributions/parseDistribution.cpp
)

set (probFuncs_files
    probFuncs/alphaBeta.h
    probFuncs/conversions.h
    probFuncs/conversions.cpp
    probFuncs/stochSettings.h
    probFuncs/stochSettings.cpp
    probFuncs/listDistribs.h
    probFuncs/listDistribs.cpp
    probFuncs/probDraw.h
    probFuncs/probDraw.cpp
    probFuncs/probNumber.h
    probFuncs/probNumber.cpp
)

set (rootFinders_files
    rootFinders/root_finder.h
    rootFinders/root_finder.cpp
    rootFinders/linear_loop.h
    rootFinders/linear_loop.cpp
    rootFinders/bisection.h
    rootFinders/bisection.cpp
    rootFinders/regula_falsi.h
    rootFinders/regula_falsi.cpp
    rootFinders/Anderson_Bjork.h
    rootFinders/Anderson_Bjork.cpp
)

set (correlation_files
    correlation/correlation.h
    correlation/identity.h
    correlation/identity.cpp
    correlation/gaussianCorrelation.h
    correlation/gaussianCorrelation.cpp
    correlation/gaussianCorrelationRobust.h
    correlation/gaussianCorrelationRobust.cpp
    correlation/HRcorrelation.h
    correlation/HRcorrelation.cpp
    correlation/trapeziumRule.h
    correlation/trapeziumRule.cpp
)

set (combine_files
    combine/Hohenbichler.h
    combine/Hohenbichler.cpp
    combine/intEqualElements.h
    combine/combineElements.h
    combine/combineElements.cpp
    combine/upscaling.h
    combine/upscaling.cpp
)

set (probMethods_files
    probMethods/lsfResult.h
    probMethods/lsfResult.cpp
    probMethods/lsf.h
    probMethods/lsf.cpp
    probMethods/dpoptions.h
    probMethods/form.h
    probMethods/form.cpp
    probMethods/formWithTrialLoops.h
    probMethods/formWithTrialLoops.cpp
    probMethods/form_init.h
    probMethods/form_init.cpp
    probMethods/ray_search.h
    probMethods/ray_search.cpp
    probMethods/sphere_search.h
    probMethods/sphere_search.cpp
    probMethods/sphere_tasks.h
    probMethods/sphere_tasks.cpp
    probMethods/dsSettings.h
    probMethods/dsRootFinder.h
    probMethods/dsRootFinder.cpp
    probMethods/dsRootFinderRobust.h
    probMethods/dsRootFinderRobust.cpp
    probMethods/directionalSampling.h
    probMethods/directionalSampling.cpp
    probMethods/crudeMonteCarlo.h
    probMethods/crudeMonteCarlo.cpp
    probMethods/crudeMonteCarloForm.h
    probMethods/crudeMonteCarloForm.cpp
    probMethods/importanceSampling.h
    probMethods/importanceSampling.cpp
    probMethods/importanceSamplingSeq.h
    probMethods/importanceSamplingSeq.cpp
    probMethods/adaptiveImpSampling.h
    probMethods/adaptiveImpSampling.cpp
    probMethods/importanceSamplingForm.h
    probMethods/importanceSamplingForm.cpp
    probMethods/numint.h
    probMethods/numint.cpp
    probMethods/fdir.h
    probMethods/fdir.cpp
    probMethods/dsfi.h
    probMethods/dsfi.cpp
    probMethods/formDsfi.h
    probMethods/formDsfi.cpp
    probMethods/zmodel.h
    probMethods/zmodel.cpp
    probMethods/parseMethod.h
    probMethods/parseMethod.cpp
    probMethods/parseAndRunMethod.h
    probMethods/parseAndRunMethod.cpp
    probMethods/fillStartVector.h
    probMethods/fillStartVector.cpp
    probMethods/progress.h
    probMethods/progress.cpp
)

file(GLOB ptkcpp_files
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/ASA/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/Randomizers/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Math/RootFinders/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/Statistics/Distributions/*.cpp"
    "../../Kernels/Deltares.Probabilistic.Kernels/CWrapper/*.cpp"
)
file(GLOB ptkcli_files
    "../../Kernels/Deltares.Probabilistics.Wrappers/Model/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Reliability/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Statistics/*.cpp"
    "../../Kernels/Deltares.Probabilistics.Wrappers/Utils/*.cpp"
)

add_library(probLib
  SHARED
    ${extern_files}
    ${rnd_files}
    ${utils_files}
    ${distributions_files}
    ${probFuncs_files}
    ${probMethods_files}
    ${rootFinders_files}
    ${correlation_files}
    ${combine_files}
)

source_group("extern" FILES ${extern_files})
source_group("rnd" FILES ${rnd_files})
source_group("utils" FILES ${utils_files})
source_group("distributions" FILES ${distributions_files})
source_group("probFuncs" FILES ${probFuncs_files})
source_group("probMethods" FILES ${probMethods_files})
source_group("rootFinders" FILES ${rootFinders_files})
source_group("correlation" FILES ${correlation_files})
source_group("combine" FILES ${combine_files})

set_target_properties(probLib
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
  )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  target_link_libraries(probLib PUBLIC OpenMP::OpenMP_CXX)
endif()

if(WIN32)
  target_link_options(probLib PRIVATE "$<$<CONFIG:Debug>:/Debug>")
endif()

if(WIN32)
  add_library(probLibKernel
    SHARED
      ${ptkcpp_files}
      ${ptkcli_files}
  )
  target_link_options(probLibKernel PRIVATE "$<$<CONFIG:Debug>:/Debug>")
  set_target_properties(probLibKernel
    PROPERTIES
      CXX_STANDARD 20
      POSITION_INDEPENDENT_CODE 1
      COMMON_LANGUAGE_RUNTIME ""
    )
endif()


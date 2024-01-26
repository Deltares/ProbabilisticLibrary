set(CMAKE_CSharp_FLAGS "/langversion:10")

file (GLOB reliabilityTestFiles
    "../../Kernels/Deltares.Probabilistics.Wrappers.Test/*.cs"
)

add_library(reliabilityTest
    SHARED
        ${reliabilityTestFiles}
)

add_executable(testRunnerMC
    RunMonteCarloTests.cs
)

add_executable(testRunnerDS
    RunDirectionalSamplingTests.cs
)

add_executable(testRunnerDistributions
    RunDistributionsTests.cs
)

target_link_libraries(reliabilityTest Deltares.Probabilistics.Wrappers )
target_link_libraries(testRunnerMC reliabilityTest )
target_link_libraries(testRunnerDS reliabilityTest )
target_link_libraries(testRunnerDistributions reliabilityTest )

set(dotnet_nunit_version "45")
set(nunit_framework "${CMAKE_CURRENT_SOURCE_DIR}/../packages/NUnit.3.13.3/lib/net${dotnet_nunit_version}/nunit.framework.dll")
message("nunit = ${nunit_framework}")

set_target_properties(reliabilityTest PROPERTIES
    VS_DOTNET_REFERENCE_nunit.framework ${nunit_framework}
)

#add_test(
#  NAME dotnet-mc-tests
#  COMMAND $<TARGET_FILE:testRunnerMC>
#)

#add_test(
#  NAME dotnet-ds-tests
#  COMMAND $<TARGET_FILE:testRunnerDS>
#)

#add_test(
#  NAME dotnet-distributions-tests
#  COMMAND $<TARGET_FILE:testRunnerDistributions>
#)



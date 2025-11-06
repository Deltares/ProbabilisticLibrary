add_executable(demo_dotnet
    dotnet/demo.cs
)

target_link_libraries(demo_dotnet Deltares.ProbabilisticLibrary.Interface)


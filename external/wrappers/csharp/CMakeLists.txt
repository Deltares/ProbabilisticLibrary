add_library(pb_wrapper_csharp SHARED
    src/plconversions.cs
    src/plcombin.cs
    src/distributions.cs
    src/probCalc.cs
    src/correlations.cs
)

add_executable(cs_demo
    demo/demo.cs
    test/comparer.cs
    test/TestUpscaling.cs
    test/ProbLibWrapperTests.cs
    test/TestCorrelation.cs
    test/TestCombineElements.cs
)

target_link_libraries(cs_demo pb_wrapper_csharp)

add_test(
  NAME csharp-interface-tests
  COMMAND $<TARGET_FILE:cs_demo>
)


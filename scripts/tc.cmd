set LLVM_PROFILE_FILE=ProbLibTest.profraw
set CC=clang-cl
set CXX=clang-cl

rem "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat" && ^
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" -arch=amd64 && ^
cmake -G Ninja -H"." -B"build/Debug" ^
 -DCMAKE_UNITY_BUILD="OFF" && ^
cmake --build build/Debug && ^
build\Debug\bin\Deltares.Probabilistic.Test.exe --gtest_filter="*Copula*"

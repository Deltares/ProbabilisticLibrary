call "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" -arch=amd64

set CC=clang-cl
set CXX=clang-cl

cmake -G Ninja ..

cmake --build .

bin\Deltares.Probabilistic.Test.exe --gtest_filter="*Copula*"

rem llvm-profdata merge -sparse default.profraw -o default.profdata

rem llvm-cov show -inst-profile default.profdata -show-directory-coverage -show-branches=count bin\Deltares.Probabilistic.CWrapper.dll -format=html -output-dir=coverage
rem
rem llvm-profdata merge -sparse foo.profraw -o default.profdata    
rem llvm-cov show bin\Deltares.Probabilistic.Test.exe -instr-profile=default.profdata
rem llvm-cov report bin\Deltares.Probabilistic.Test.exe -instr-profile=default.profdata > report.out

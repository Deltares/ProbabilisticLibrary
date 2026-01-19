call "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" -arch=amd64

set CC=clang-cl
set CXX=clang-cl
rem set CC=cl
rem set CXX=cl
rem set CMAKE_CXX_FLAGS=-fprofile-instr-generate -fcoverage-mapping -w
rem set CMAKE_C_FLAGS=-fprofile-instr-generate -fcoverage-mapping -w

rem cmake .. -DCMAKE_UNITY_BUILD=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
rem cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug  ^
rem -DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping -w" -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping -w" ^

cmake -G Ninja -D CMAKE_CXX_FLAGS=--coverage -D CMAKE_EXE_LINKER_FLAGS=--coverage -D CMAKE_MODULE_LINKER_FLAGS=--coverage -D CMAKE_SHARED_LINKER_FLAGS=--coverage ..

rem -DCMAKE_EXE_LINKER_FLAGS="-fprofile-instr-generate -fcoverage-mapping -w" 
cmake --build .

rem bin\Deltares.Probabilistic.Test.exe --gtest_filter="*Copula*"

rem llvm-profdata merge -sparse default.profraw -o default.profdata

rem llvm-cov show -inst-profile default.profdata -show-directory-coverage -show-branches=count bin\Deltares.Probabilistic.CWrapper.dll -format=html -output-dir=coverage

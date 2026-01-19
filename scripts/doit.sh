#!/bin/sh

export CC=clang-19
export CXX=clang++-19
# set CC=cl
# set CXX=cl
# export CMAKE_CXX_FLAGS=-fprofile-instr-generate -fcoverage-mapping -w
# export CMAKE_C_FLAGS=-fprofile-instr-generate -fcoverage-mapping -w

# cmake .. -DCMAKE_UNITY_BUILD=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 
cmake ..  -DCMAKE_BUILD_TYPE=Debug  
#-DCMAKE_C_FLAGS="-fprofile-instr-generate -fcoverage-mapping -w" -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping -w" 
cmake --build .

#bin\Deltares.Probabilistic.Test.exe --gtest_filter="*Copula*"

#llvm-profdata merge -sparse default.profraw -o default.profdata

#llvm-cov show -inst-profile default.profdata -show-directory-coverage -show-branches=count bin\Deltares.Probabilistic.CWrapper.dll -format=html -output-dir=coverage

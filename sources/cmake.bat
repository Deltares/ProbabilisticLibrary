mkdir build
cd build
"C:\Program Files\CMake\bin\cmake" .. -DENABLE_Fortran=OFF  -A=x64
"C:\Program Files\CMake\bin\cmake" --build . --config Release
pause
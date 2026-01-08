version=3.28
build=3
## don't modify from here
limit=3.20
os=linux
wget https://cmake.org/files/v$version/cmake-$version.$build-$os-x86_64.sh 
mkdir /opt/cmake
sh cmake-$version.$build-$os-x86_64.sh --prefix=/opt/cmake --skip-license
rm cmake-$version.$build-$os-x86_64.sh

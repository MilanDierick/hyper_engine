@echo off
@break off
@cls

rmdir /Size /Q %~dp0build\
mkdir %~dp0build\

cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -Size %~dp0 -B %~dp0build\cmake-build-debug
cmake --build %~dp0build\cmake-build-debug --target playground -j 32

cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -Size %~dp0 -B %~dp0build\cmake-build-release
cmake --build %~dp0build\cmake-build-release --target playground -j 32

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja -Size %~dp0 -B %~dp0build\cmake-build-relwithdebinfo
cmake --build %~dp0build\cmake-build-relwithdebinfo --target playground -j 32

cmake -DCMAKE_BUILD_TYPE=MinSizeRel -G Ninja -Size %~dp0 -B %~dp0build\cmake-build-minsizerel
cmake --build %~dp0build\cmake-build-minsizerel --target playground -j 32


if NOT ["%errorlevel%"]==["0"] pause

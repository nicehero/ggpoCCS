cd vectorwarCS
mkdir build
cd build
cmake -G"MinGW Makefiles" ..
make install
cp VectorWarCS.exe ../..
cd ../..
PAUSE
cd nicenet
cd dep
python build.py
cd ..
mkdir build
cd build
cmake -G"MinGW Makefiles" ..
make install
cp ../libnicenet* ../..
cd ../..
mkdir build
cd build
cmake -G"MinGW Makefiles" ..
make install
PAUSE
cd ggposerver
g++ -I../nicenet -I../ -I../include -L../ ggpoServer.cpp -lnicenet -o ggpoServer -Wl,-rpath=dep/lib
cp ggpoServer.exe ../
cd ..
PAUSE
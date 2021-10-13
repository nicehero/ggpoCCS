# GGPO
(简体中文版[这里](README.cn.md))

####Introduction

Ggpo network SDK was founded in 2009 and took the lead in using rollback network in point-to-point games. 

It is specifically designed to hide network delays in fast-paced, twitching games that require very precise input and frame perfect execution.

See for details https://ggpo.net

My forked version features:

Cross platform ggpo branch, mainly to support mobile platforms

Transform to a version that supports client server

####Installation tutorial

Android NDK so library creation

1. CMake 3.18.1 / NDK 20.1.5948944 or CMake 3.21.3 + /NDK 23

2. mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh",rename "mingw32-make.exe" to "make.exe"

3. need "wget.exe","tar.exe" to environment PATH

4. need "python" and set environment PATH

5. Set the environment variable NDK_ROOT

6. run buildNDK.bat

7. Get libggpo.so and libnicenet.so

Simple test installation based on mingw-w64 Windows version:

1. Install mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh",rename "mingw32-make.exe" to "make.exe"

2. need "wget.exe","tar.exe" to environment PATH

3. need "python" and set environment PATH

4. buildLibs.bat

5. buildServer.bat

6. buildTest.bat

7. buildTestCS.bat

8. Test P2P UDP version: test1.bat, test2.bat; Test CS version: ggposerver.exe, testcs1.bat, testcs2.bat

####Instructions for use

Simply put, it is to set the number of players, set the save,load,nextFrame,etcs for state machine to callback functions to ggposession and call the corresponding API to create, and then run the state machine normally

See the example vectorwar for details


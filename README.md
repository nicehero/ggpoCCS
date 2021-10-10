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

1. CMake 3.18.1

2. NDK 20.1.5948944

3. Set the environment variable Android_ NDK_ HOME

4. buildNDK.bat

5. Get libggpo.so

Simple test installation based on mingw-w64 Windows version:

1. Install mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh"

2. buildLibs.bat

3. buildServer.bat

4. buildTest.bat

5. buildTestCS.bat

6. Test P2P UDP version: test1.bat, test2.bat; Test CS version: ggposerver.exe, testcs1.bat, testcs2.bat

####Instructions for use

Simply put, it is to set the number of players, set the save,load,nextFrame,etcs for state machine to callback functions to ggposession and call the corresponding API to create, and then run the state machine normally

See the example vectorwar for details


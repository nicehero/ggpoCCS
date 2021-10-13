# GGPO

#### 介绍
GGPO 网络 SDK 创建于 2009 年，率先在点对点游戏中使用回滚网络。它专门设计用于在需要非常精确的输入和帧完美执行的快节奏、抽搐式游戏中隐藏网络延迟。 

具体见https://ggpo.net

我的分叉版本功能:

跨平台ggpo分支，主要使其支持移动平台

改造成一个支持ClientServer的版本

#### 安装教程

Android NDK so 库创建
1. "CMake 3.18.1" & "NDK 20" or "CMake 3.21.3" & "NDK 23"
2. mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh",修改文件名 mingw32-make.exe为make.exe
3. 安装python放到环境变量 PATH 目录中
4. 需要 wget.exe,tar.exe 放到环境变量 PATH 目录中
5. 设置环境变量 NDK_ROOT 为NDK所在目录
6. 运行buildNDK.bat
7. 最终获得NDK库文件 libggpo.so 和 libnicenet.so

基于Mingw-w64 Windows版本 简单测试安装：
1.  安装Mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh",修改文件名 mingw32-make.exe为make.exe
2.  需要 wget.exe,tar.exe 放到环境变量 PATH 目录中
3.  需要安装python放到并将其环境变量 PATH 目录中
4.  buildLibs.bat
5.  buildServer.bat
6.  buildTest.bat
7.  buildTestCS.bat
8.  测试P2PUDP版本:test1.bat,test2.bat;测试CS版本:ggpoServer.exe,testCS1.bat,testCS2.bat

#### 使用说明

简单来说就是设置好多少player，设置状态机保存，状态机加载，状态机下一帧等回调函数给ggpoSession并调用相应api创建，然后正常跑状态机即可

具体可以看例子vectorwar
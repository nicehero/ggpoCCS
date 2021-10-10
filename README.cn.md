# GGPO

#### 介绍
GGPO 网络 SDK 创建于 2009 年，率先在点对点游戏中使用回滚网络。它专门设计用于在需要非常精确的输入和帧完美执行的快节奏、抽搐式游戏中隐藏网络延迟。 

具体见https://ggpo.net

我的分叉版本功能:

跨平台ggpo分支，主要使其支持移动平台

改造成一个支持ClientServer的版本

#### 安装教程

Android NDK so 库创建
1.  CMake 3.18.1
2.  NDK 20.1.5948944
3.  设置环境变量 ANDROID_NDK_HOME
4.  buildNDK.bat
5.  获得 libggpo.so

基于Mingw-w64 Windows版本 简单测试安装：
1.  安装Mingw-w64 8.1.0 arch: "x86_64",  Threads: "posix",  Exception: "seh"
2.  buildLibs.bat
3.  buildServer.bat
4.  buildTest.bat
5.  buildTestCS.bat
6.  测试P2PUDP版本:test1.bat,test2.bat;测试CS版本:ggpoServer.exe,testCS1.bat,testCS2.bat

#### 使用说明

简单来说就是设置好多少player，设置状态机保存，状态机加载，状态机下一帧等回调函数给ggpoSession并调用相应api创建，然后正常跑状态机即可

具体可以看例子vectorwar
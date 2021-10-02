# ggpo

#### 介绍
ggpo功能参见https://ggpo.net

跨平台ggpo支持分支，主要使其支持移动平台

并准备制作成一个支持ClientServer版本

#### 安装教程

Android
1.  CMake 3.18.1
2.  NDK 20.1.5948944
3.  设置环境变量 ANDROID_NDK_HOME
4.  cd ndkBuild && build.bat
5.  获得 libggpo.so

#### 使用说明

  先看ggpo自己的例子vectorwar:https://github.com/pond3r/ggpo

  大概就是设置好多少player，设置好状态机保存，状态机加载，状态机下一帧等回调函数给ggpoSession并创建，然后正常跑状态机即可


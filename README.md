# PHOENIXEngine

开源的室内机器人及物联网开发引擎

open soruce cross platform system for robots/iOT

版本：测试版 0.01

主要核心功能：

    功能完备的3D场景仿真渲染，UI组件等

    基于Windows和Liunx室内机器人自主定位建图，路径规划0.01版
    
快速编译测试：

    在Windows平台，下载安装VisualStudio，下载引擎及依赖库解压，编译即可运行

网址website: www.manykit.com

商城market: https://shop194048616.taobao.com

![avatar](Documentation/ShowImagesGitHub/img/phoenixframe.png)

## 简述

	欢迎来到PHOENIXEngine，这是一个可以用来开发游戏，机器人，物联网项目的开发引擎。集成了HectorSlam，用来做为机器人室内导航。
	引擎已经支持Windows上，ARM，Linux，LinuxARM，iOS平台，使用简洁高效的Lua作为主要逻辑开发，使用C++/C作为底层构建语言，通过跨平台编译，可以将项目运行在所需要的平台上。
	借助Arduino，ESP8266/32等芯片，使用PHOENIXFrameWireWall，引擎可以支持一般的物联设备开发。
	当需要更高级的嵌入式方案时，我们可以选择LinuxArm作为开发环境，打造高级的物联网系统。
	PHOENIXEngine只选择了最基本的两种语言C/C++和Lua，降低使用者的门槛。引擎致力于打造容易实用，典雅优美的开发体验。

## 版权信息/LICENCE

	ManyKit开源协议：
	PHOENIXEngine是一个开放开源的引擎，拥有免费版本和商业版本，开源版本之需要遵循较少的规则就可以免费用于商业开发，商业版本包含相关的技术服务。协议声明如下：
	1.任何人，组织都可以自由修改引擎，免费应用于商业开发。
	2.使用PHOENIXEngine免费版本，必须在醒目位置标准PHOENIXEngine的商标，并且在软件中出现PHOENIXEngine开源协议。
	3.用户开发的扩展包，可以选择开源或者避源，对自己开发的开发包享有完全的自主控制权利。
	4.许多米科技保留对PHONEIXEngine，PHOENIXFrame的商标权。
	5.许多米科技目前保留对引擎的教育培训领域的完全使用权，第三方如想要使用PHOENIXEngine作为培训教育，许多米可以进行相关培训，并且颁发认证。
	6.许多米科技目前保留对引擎技术社区，包含项目交流社区的独家开发使用权。
	7.任何捐赠第三方必须首先提出对引擎修改意见和技术支持的具体需求。

##  编译/Compile

### 依赖库

ThirdPartiesLibs.zip：[http://url.cn/49EuKAU](http://url.cn/49EuKAU)

下载放置PHOENIXEngine\Phoenix目录下，解压。

如需编译编辑器NIRVANA2，预先编译ThirdPartiesLibs/wxWidgets-3.0.2/build/msw/wx_vc12.sln

### Windows

使用visualstudio打开PHOENIXEngine/PHOENIX.sln编译

### Linux

**1.安装依赖库**

- 	拷贝FMOD:

	进入目录 PHOENIX/ThirdPartiesLibs/

	sudo cp -fr FMOD /usr/local/lib/ 

	sudo chmod 777 /usr/local/lib/FMOD
	
	进入目录 /usr/local/lib/FMOD/linux/lib/x86
	如果是64位系统环境进入 /usr/local/lib/FMOD/linux/lib/x86_64目录

	sudo cp -f libfmod.so.9 /usr/local/lib/

- 	安装OpenGL:

	sudo apt-get install freeglut3-dev

**2.编译引擎库libPX2Engine.so:**

	在目录PHOENIXEngine/Phoenix中执行:make CFG=ReleaseDynamic -f makefile.px2

**3.安装引擎库:**

	在目录PHOENIXEngine/Phoenix中执行:sudo make CFG=ReleaseDynamic -f makefile.px2 install

**4.如果具体的项目是动态链接库，需要编译c++源文件，例如如果项目名称为Sample编译项目库为Sample.so
如果想要运行引擎，进行快速查看，可以不需要编译，跳过此项

	在目录PHOENIXEngine/PHOENIX/Projects/Client/Sample中执行:make CFG=ReleaseDynamic -f makefile.px2

**5.运行最终的可执行程序**
	
	在PHOENIXEngine/PHOENIX/Bin目录执行 ./AppPlayer.ReleaseDynamic
	
### Linux Raspberry

**1.安装依赖库**

- 拷贝FMOD:

	进入目录 PHOENIX/ThirdPartiesLibs/

	sudo cp -fr FMOD /usr/local/lib/ 

	sudo chmod 777 /usr/local/lib/FMOD
	
	进入目录 /usr/local/lib/FMOD/linux/lib/armhf

	sudo cp -f libfmod.so.9 /usr/local/lib/

- 	安装OpenGLES
	
	sudo apt-get install libgles2-mesa-dev

**2.编译引擎库libARMPX2Engine.so:**

	在目录PHEONIXEngine/Phoenix中执行:make CFG=ReleaseARMDynamic -f makefile.px2

**3.安装引擎库:**

	在目录PHEONIXEngine/Phoenix中执行:sudo make CFG=ReleaseARMDynamic -f makefile.px2 install

**4.如果具体的项目是动态链接库，需要编译c++源文件，例如如果项目名称为Sample编译项目库为Sample.so
如果想要运行引擎，进行快速查看，可以不需要编译，跳过此项

	在目录PHOENIXEngine/PHOENIX/Projects/Client/Sample中执行:make CFG=ReleaseARMDynamic -f makefile.px2

**5.执行**
	
	在PHEONIXEngine/PHOENIX/Bin目录执行 AppPlayer.ReleaseARMDynamic

### Android

目前我们使用的还是旧的Android编译方法，使用Crygwin进行编译。

	下载Cygwin:
	安装Cygwin:
	使用Cygwin进行编译:
	使用Ecplise打开项目进行发布:
	导出Ecplise Android项目:

### IOS

	使用XCode打开项目PHOENIX/Phoenix.xcworkspace编译项目

## 使用方法/General Use

- Bin/Data/Sample 是一个空项目

	Data下的boost.xml决定从哪个项目启动
	boost.lua可以对启动项二次修改
	
	项目运行前，执行，Sample/lua/start.lua中的prestart
	紧接着运行，执行，Sample/lua/start.lua中的start
	用户的项目逻辑在presstart和start函数中写就可以
	
## 更多内容请查看GitHub Wiki

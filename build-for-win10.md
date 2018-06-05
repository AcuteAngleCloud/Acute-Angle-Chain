环境准备：
安装cygWin，选择LLVM版本为4.0，boost版本选择1.66

1、运行cygWin，使用git clone https://github.com/AcuteAngleCloud/Acute-Angle-Chain
2、cd Acute-Angle-Chain，mkdir build
3、cd build
4、cmake ..
5、这时候会生成CMakeCache.txt文件
6、根据提示修改缺失的文件
7、再次到build下边cmake ..
8、提示需要修改C:\cygwin64\home\userName\Acute-Angle-Chain\libraries\wasm-jit\Source\Platform\POSIX.cpp
----具体做法是添加一个 #define __linux__
----30行注释掉
----153行改为throw "pthread_getattr_np is Unrealized by XXX";当有异常时直接抛出异常信息就是了

9、编译nodaac（在build文件夹下运行make nodaac）
10、编译kaacd (在build文件夹下运行make kaacd)
11、编译claac (在build文件夹下运行make claac)
12、其它模块不用编译，智能合约可以和Ubuntu下编译的共用，直接将在Ubuntu下编译好的智能合约拷贝到build文件夹下的contracts中即可（原有文件删掉）

# Memory Leak Checker
## 软件介绍
 Memory Leak Checker是使用malloc的hook机制来检查软件内存泄漏问题，并且使用了libunwind来获取泄漏地址的堆栈信息。</br>
 该软件支持malloc/new,并且支持多线程(valgrind 在多线程环境下会有比较大的问题），使用时只需要将mem_leak_checker头文件包含在需要监控的文件内即可，但是机器上必须安装libunwind库。
## API介绍

>void mem_leak_checker_init();</br>

初始化底层数据结构，在main函数的启动前调用。为了方便代码往c语言移植，我们并没有使用c++的面向对象特性来编写代码，所以移植到c语言非常方便。

>pthread_mutex_t* get_lock(); </br>
>void start_hook();</br>
>void stop_hook();</br>

对于大型的项目，底层会有大量的malloc调用，由于malloc_hook底层使用了libuwind来获取堆栈信息，所以会大幅度降低程序性能.
因此Memory Leak Checker支持按照一定区块来监控，示例代码见main文件。


>void mem_leak_checker_detail();</br>

在main函数末尾，获取泄漏的内存信息

>void mem_leak_checker_clear();</br>

显式清理泄漏的内存

## 编译
如果你使用blade，我也为你写好了BUILD文件。</br>
你也可以使用g++来编译: g++ hash.cpp main.cpp mem_leak_checker.cpp stack_trace.cpp -lpthread  -lunwind -o main

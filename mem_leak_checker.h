//
// Created by lintong on 2018/4/11.
//

#ifndef MEMORY_LEAK_CHECKER_MEM_LEAK_CHECKER_H
#define MEMORY_LEAK_CHECKER_MEM_LEAK_CHECKER_H



#ifdef __cplusplus
extern "C" {
#endif

#include<pthread.h>


    pthread_mutex_t* get_lock();
    void start_hook();
    void stop_hook();
    void mem_leak_checker_init();
    void mem_leak_checker_detail();
    void mem_leak_checker_clear();

#ifdef __cplusplus
}
#endif


#endif //MEMORY_LEAK_CHECKER_MEM_LEAK_CHECKER_H

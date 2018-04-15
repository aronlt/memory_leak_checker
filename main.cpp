#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <string.h>
#include "mem_leak_checker.h"



void* malloc_and_free_thread(void*);
void* malloc_without_free_thread(void*);

int main(int argc, char** argv) {
    pthread_t tid1, tid2;
    fprintf(stderr, "enter main\n");
    mem_leak_checker_init();
    int rc2 = pthread_create(&tid2, NULL, malloc_and_free_thread, NULL);
    if(rc2 != 0)
        fprintf(stderr, "%s: %d\n",__func__, strerror(rc2));

    int rc1 = pthread_create(&tid1, NULL, malloc_without_free_thread, NULL);
    if(rc1 != 0)
        printf("%s: %d\n",__func__, strerror(rc1));

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    mem_leak_checker_detail();
    mem_leak_checker_clear();
    fprintf(stderr, "leave main\n");
    exit(0);
}


void malloc_and_free() {
    char* p = (char*)malloc(1);
    free(p);
}

void malloc_deep_2() {
    char* p = (char*)malloc(1);
}
void malloc_deep() {
    malloc_deep_2();
}

void malloc_without_free() {
    malloc_deep();
}

void* malloc_and_free_thread(void* arg) {
    fprintf(stderr, "malloc and free thread id is %u, start to malloc and free\n", (unsigned int)pthread_self());
    for (int i= 0; i < 1000; ++i) {
        malloc_and_free();
    }
    fprintf(stderr, "malloc and free thread id is %u, malloc and free success\n", (unsigned int)pthread_self());
    pthread_exit(0);
}


void* malloc_without_free_thread(void* arg) {
    fprintf(stderr, "malloc without free thread id is %u, start to malloc\n", (unsigned int)pthread_self());
    pthread_mutex_t* lock = get_lock();
    pthread_mutex_lock(lock);
    start_hook();
    for (int i = 0; i < 3; ++i) {
        malloc_without_free();
    }
    stop_hook();
    pthread_mutex_unlock(lock);
    fprintf(stderr, "malloc and free thread id is %u, malloc and free success\n", (unsigned int)pthread_self());
    pthread_exit(0);
}


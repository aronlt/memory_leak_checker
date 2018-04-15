/* Prototypes for __malloc_hook, __free_hook */
#include <iostream>
#include "mem_leak_checker.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "stack_trace.h"
#include "hash.h"


#define MAX_STACK_LEN 50

volatile int hook_flag = 0;
pthread_mutex_t mlock;

#define save_hook(old_malloc_hook, old_free_hook) \
    do { (old_malloc_hook)=reinterpret_cast<void* (*) (size_t, const void*)>(__malloc_hook); \
        (old_free_hook)=reinterpret_cast<void (*) (void *, const void *)>(__free_hook); \
    } while(0)

#define set_hook(old_malloc_hook, old_free_hook) \
    do { __malloc_hook=reinterpret_cast<void* (*) (size_t, const void*)>(old_malloc_hook); \
        __free_hook=reinterpret_cast<void (*) (void *, const void *)>(old_free_hook); \
    } while(0)

void* (*malloc_hook_backup) (size_t size, const void *caller);
void (*free_hook_backup) (void *ptr, const void *caller);

/* Prototypes for our hooks.  */
static void *malloc_hook (size_t, const void *);
static void free_hook (void*, const void *);


pthread_mutex_t* get_lock() {
    return &mlock;
}
void start_hook() {
    hook_flag = 1;
}
void stop_hook() {
    hook_flag = 0;
}

void mem_leak_checker_detail() {
    DEBUG_PRINT("start to print detail..\n");
    pthread_mutex_lock(&mlock);
    print_unclear_memory();
    pthread_mutex_unlock(&mlock);
}

void mem_leak_checker_init() {
    DEBUG_PRINT("start to init mem leak checker\n");
    save_hook(malloc_hook_backup, free_hook_backup);
    set_hook(malloc_hook, free_hook);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&mlock,&attr);
    init_hash_table();
}

void mem_leak_checker_clear() {
    DEBUG_PRINT("start to clear hash table\n");
    pthread_mutex_lock(&mlock);
    clear_all_hash_table();
    pthread_mutex_unlock(&mlock);
    pthread_mutex_destroy(&mlock);
}

static void *malloc_hook(size_t size, const void *caller) {
    void* result = NULL;
    int max_stack_len = MAX_STACK_LEN;

    pthread_mutex_lock(&mlock);
    set_hook(malloc_hook_backup, free_hook_backup);

    if (hook_flag == 0) {
        result =  malloc(size);
    } else {
        DEBUG_PRINT("in malloc hook, start to alloc stack array\n");
        char** stacks = (char**)malloc(sizeof(char*) * max_stack_len);
        if (stacks == NULL) {
            DEBUG_PRINT("in malloc hook, alloc stack array fail\n");
            pthread_mutex_unlock(&mlock);
            return NULL;
        }
        int nptrs = backtrace(stacks, max_stack_len);
        DEBUG_PRINT("in malloc hook, backtrace stack success\n");

        result = malloc (size);

        if (result != NULL) {
            insert_hash_table((size_t) result, nptrs, stacks);
        }
    }
    save_hook(malloc_hook_backup, free_hook_backup);
    set_hook(malloc_hook, free_hook);
    pthread_mutex_unlock(&mlock);
    return result;
}

static void free_hook (void *ptr, const void *caller) {
    pthread_mutex_lock(&mlock);
    set_hook(malloc_hook_backup, free_hook_backup);
    if (hook_flag == 0) {
        free(ptr);
    } else {
        if (ptr != NULL) {
            DEBUG_PRINT("in free hook, start to clear hash table\n");
            clear_hash_table((size_t) ptr);
        }
        free (ptr);
    }
    save_hook(malloc_hook_backup, free_hook_backup);
    set_hook(malloc_hook, free_hook);
    pthread_mutex_unlock(&mlock);
}



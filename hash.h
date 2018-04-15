//
// Created by lintong on 2018/4/11.
//

#ifndef MEMORY_LEAK_CHECKER_HASH_H
#define MEMORY_LEAK_CHECKER_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include "define.h"

#define TABLE_SIZE 5009993
#define step_forward(pos) (((pos) + 1) % TABLE_SIZE)

    typedef struct Node {
        char** stack_frames;
        int frame_number;
        size_t alloc_address;
    } Node;

    typedef struct Table {
        struct Node table[TABLE_SIZE];
    } Table;

    int clear_hash_table(size_t address);
    void clear_all_hash_table();
    void init_hash_table();
    void print_unclear_memory();
    int insert_hash_table(size_t address, int nptrs, char** stacks);

#ifdef __cplusplus
}
#endif

#endif //MEMORY_LEAK_CHECKER_HASH_H

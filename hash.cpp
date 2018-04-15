//
// Created by lintong on 2018/4/11.
//

#include "hash.h"

static Table hash_table;

static size_t hash(size_t address);
static int find(size_t address, size_t* found_pos) {
    size_t pos = hash(address);
    size_t check_pos = pos;
    int found = -1;
    while (hash_table.table[pos].frame_number != -1) {
        if (hash_table.table[pos].alloc_address == address) {
            found = 1;
            break;
        }
        pos = step_forward(pos);
        if (pos == check_pos) {
            break;
        }
    }
    DEBUG_PRINT("find address:%lu pos:%lu, is found:%d\n", address, pos, found);
    *found_pos = pos;
    return found;
}

void print_unclear_memory() {
    DEBUG_PRINT("start to print leak memory stacks\n");
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        int frame_number = hash_table.table[i].frame_number;
        if (frame_number != -1) {
            fprintf(stderr, "find leak memory!\n");
            for (size_t j = 0; j < frame_number; j++) {
                fprintf(stderr, "%s\n", hash_table.table[i].stack_frames[j]);
            }
        }
    }
}

int clear_hash_table(size_t address) {
    DEBUG_PRINT("clear address:%lu\n", address);
    size_t pos = 0;
    int found = find(address, &pos);
    int i = 0;
    if (found < 0) {
        DEBUG_PRINT("clear address:%lu, not found in hash table\n", address);
        return -1;
    }
    DEBUG_PRINT("clear address:%lu, function stacks: \n", address);
    for (i = 0; i < hash_table.table[pos].frame_number; i++) {
        DEBUG_PRINT("%s\n", hash_table.table[pos].stack_frames[i]);
    }

    free(hash_table.table[pos].stack_frames);
    hash_table.table[pos].alloc_address = 0;
    hash_table.table[pos].frame_number = -1;
    hash_table.table[pos].stack_frames = NULL;

    DEBUG_PRINT("clear address:%lu success\n", address);
    return 0;
}

void init_hash_table() {
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        hash_table.table[i].stack_frames = NULL;
        hash_table.table[i].frame_number = -1;
        hash_table.table[i].alloc_address = 0;
    }
}

void clear_all_hash_table() {
    DEBUG_PRINT("clear all table\n");
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        int frame_number = hash_table.table[i].frame_number;
        if (frame_number != -1) {
            free(hash_table.table[i].stack_frames);
        }
        hash_table.table[i].stack_frames = NULL;
        hash_table.table[i].frame_number = -1;
        hash_table.table[i].alloc_address = 0;
    }
}

int insert_hash_table(size_t address, int nptrs, char** stacks) {
    size_t pos = 0;
    DEBUG_PRINT("insert address:%lu\n", address);
    int found = find(address, &pos);
    if (found > 0) {
        DEBUG_PRINT("insert repeat\n");
        return -1;
    }
    // todo:这里需要进行rehash或者重新分配地址
    if (hash_table.table[pos].frame_number != -1) {
        DEBUG_PRINT("hash table is full!\n");
        return -1;
    }
    hash_table.table[pos].frame_number = nptrs;
    hash_table.table[pos].alloc_address = address;
    hash_table.table[pos].stack_frames = stacks;

    DEBUG_PRINT("insert address:%lu success, pos:%lu, frame_number:%d\n", address, pos, nptrs);
    for (int i = 0; i < nptrs; i++) {
        DEBUG_PRINT("%s\n", stacks[i]);
    }
    return 0;
}

// todo:地址需要重新散列，不然冲突比较严重
static size_t hash(size_t address) {
    return  (size_t)(address % TABLE_SIZE);
}

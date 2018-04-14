//
// Created by lintong on 2018/4/11.
//

#ifndef MEMORY_LEAK_CHECKER_STACK_TRACE_H
#define MEMORY_LEAK_CHECKER_STACK_TRACE_H

int __attribute__((noinline)) backtrace(char* stacks[], int stack_len);

#endif //MEMORY_LEAK_CHECKER_STACK_TRACE_H

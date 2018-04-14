//
// Created by lintong on 2018/4/11.
//

#ifndef MEMORY_LEAK_CHECKER_DEFINE_H
#define MEMORY_LEAK_CHECKER_DEFINE_H

#define DEBUG 3


#if defined(DEBUG) && DEBUG > 0
 #define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, \
    __FILE__, __LINE__, __func__, ##args)
#else
 #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
#endif

#endif //MEMORY_LEAK_CHECKER_DEFINE_H

cmake_minimum_required(VERSION 3.9)
project(memory_leak_checker)

add_executable(memory_leak_checker
        main.cpp
        define.h
        hash.cpp
        hash.h
        mem_leak_checker.cpp
        mem_leak_checker.h
        stack_trace.cpp
        stack_trace.h
        )

TARGET_LINK_LIBRARIES(mem_leak_checker pthread)

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <cxxabi.h>

#include <stdio.h>
#include <stdlib.h>

#define RESERVE_SIZE 256

int __attribute__((noinline)) backtrace(char* stacks[], int stack_len) {
  unw_cursor_t cursor;
  unw_context_t context;

  unw_getcontext(&context);
  unw_init_local(&cursor, &context);

  int n=0;
  while ( unw_step(&cursor) && n < stack_len) {
    unw_word_t off;

    char symbol[256] = {"<unknown>"};
    char *name = symbol;

    if ( !unw_get_proc_name(&cursor, symbol, sizeof(symbol), &off) ) {
      int status;
      if ( (name = abi::__cxa_demangle(symbol, NULL, NULL, &status)) == 0 )
        name = symbol;
    }

    char* p = (char*)malloc(sizeof(char*) * RESERVE_SIZE * 2);
    if (p == NULL) {
        return n;
    }
    sprintf(p, "#%-2d" PRIxPTR "%s + 0x%" PRIxPTR "\n",
        n,
        name,
        static_cast<uintptr_t>(off));
    stacks[n++] = p;

    if ( name != symbol ) {
      free(name);
    }
  }
  return n;
}

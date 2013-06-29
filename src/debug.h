#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define debug_print(fmt, ...) \
  do { if (DEBUG_TEST) fprintf(stderr, "%s:%d:%s(): " fmt "\n", __FILE__, __LINE__, \
      __func__, __VA_ARGS__); } while (0)

#endif

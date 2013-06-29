#ifndef DOCK_H
#define DOCK_H

#include <string>

/* bah.. */
typedef enum {
  DOCK_TOP = 0,
  DOCK_BOTTOM,
  DOCK_LAST
} dock_position_t;

extern std::string dock_names[DOCK_LAST];

#endif

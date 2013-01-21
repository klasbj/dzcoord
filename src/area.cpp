#include "area.h"

std::string float_names[FLOAT_LAST] = {
  "LEFT",
  "CENTER",
  "RIGHT",
};

bool operator<(const area_t & a, const area_t & b) {
  if (a.fl == b.fl)
    return a.weight > b.weight;
  return a.fl < b.fl;
}

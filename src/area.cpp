#include "area.h"

std::string float_names[FLOAT_LAST] = {
  "LEFT",
  "CENTER",
  "RIGHT",
};


bool area_t_lt::operator()(const area_t * l, const area_t * r) {
  if (l->fl == r->fl)
    return l->weight < r->weight;
  return l->fl < r->fl;
}

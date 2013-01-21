#ifndef AREA_H
#define AREA_H

#include <string>
#include <unordered_map>

typedef enum {
  FLOAT_LEFT = 0,
  FLOAT_CENTER,
  FLOAT_RIGHT,
  FLOAT_LAST
} float_e;

extern std::string float_names[FLOAT_LAST];

class area_t {
  public:
  std::string id;
  int weight;
  std::string text;
  std::string prints;
  float_e fl;
  size_t width;

  area_t() : id(""),weight(0),text(""),prints(""),fl(FLOAT_LAST),width(0) { }
};

bool operator<(const area_t & a, const area_t & b);

//map<size_t,area_t> areas;
extern std::unordered_map<std::string,area_t> areas;

#endif

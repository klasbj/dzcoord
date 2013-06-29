#ifndef SCREEN_H
#define SCREEN_H

#include "area.h"
#include "dzen.h"
#include "dock.h"
#include <string>
#include <set>
#include <vector>

class screen_t {
  private:
  typedef std::multiset<area_t*, area_t_lt> aset;
  aset atop, abottom;
  dzen_t top, bottom;
  bool show_top, show_bottom;
  bool screen_active;

  void bartext(const aset & d, std::string & left, std::string & right,
               size_t & right_pos) const;

  public:
  int id;
  int x, y, width, height;
  bool dirty;

  screen_t(int id = 0, int w = 0, int h = 0) 
    : top(id,DOCK_TOP),bottom(id,DOCK_BOTTOM),show_top(false),
      show_bottom(false), screen_active(false),
      id(id),x(0),y(0),width(w),height(h),dirty(true) { }

  void add_area(area_t * area, dock_position_t position);

  void set_screen_size(int x, int y, int w, int h);
  void set_screen_size(const screen_t & s);

  void update();

};

extern std::vector<screen_t> screens;

#endif

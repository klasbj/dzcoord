#include "options.h"
#include "screen.h"
#include <string>
#include <cstdio>

std::string dock_names[DOCK_LAST] = {
  "TOP",
  "BOTTOM",
};

std::vector<screen_t> screens;

void screen_t::add_area(area_t * area, dock_position_t position) {
  this->dirty = true;
  switch (position) {
    case DOCK_TOP:
      show_top = true;
      atop.insert(area);
      break;
    case DOCK_BOTTOM:
      show_bottom = true;
      abottom.insert(area);
      break;
    default:
      break;
  }
}

void screen_t::set_screen_size(int x, int y, int w, int h) {
  this->x = x;
  this->y = y;
  this->width = w;
  this->height = h;
  this->dirty = true;
  this->screen_active = true;
}
void screen_t::set_screen_size(const screen_t & s) {
  this->x = s.x;
  this->y = s.y;
  this->width = s.width;
  this->height = s.height;
  this->dirty = true;
  this->screen_active = true;
}

void screen_t::bartext(const aset & d, std::string & left, std::string & right,
                       size_t & right_pos) const {
  bool center_added = false;
  int left_pos = 0;

  left = "";
  right = "";
  right_pos = this->width;
  
  for (auto it = d.begin(); it != d.end(); ++it) {
    if ((*it)->fl == FLOAT_LEFT) {
      left_pos += (*it)->width;
      left += (*it)->prints;
      if ((*it)->width > 0) {
        left += SEPARATOR;
        left_pos += SEPARATOR_WIDTH;
      }
    } else if ((*it)->fl == FLOAT_CENTER) {
      if (center_added) {
        if ((*it)->width > 0) {
          left += SEPARATOR;
          left_pos += SEPARATOR_WIDTH;
        }
      } else {
        center_added = (*it)->width > 0;
      }

      left_pos += (*it)->width;
      left += (*it)->prints;

    } else if ((*it)->fl == FLOAT_RIGHT) {
      if ((*it)->width > 0) {
        right += SEPARATOR;
        right_pos -= SEPARATOR_WIDTH;
      }
      right_pos -= (*it)->width;
      right += (*it)->prints;
    }
  }
}

void screen_t::update() {
  if (!this->screen_active) return;

  std::string left, right;
  size_t right_pos;
  this->bartext(atop, left, right, right_pos);
  if (this->dirty) {
    this->top.start_dzen();
    this->top.set_size(this->width, this->height);
  }
  this->top.write(left, right, right_pos);

  this->bartext(abottom, left, right, right_pos);
  if (this->dirty) {
    this->bottom.start_dzen();
    this->bottom.set_size(this->width, this->height);
  }
  this->bottom.write(left, right, right_pos);

  this->dirty = false;
}

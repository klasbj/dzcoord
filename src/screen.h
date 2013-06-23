#ifndef SCREEN_H
#define SCREEN_H

class screen_t {
  public:
  int id;
  int x, y, width, height;
  bool dirty;

  screen_t() : id(0),x(0),y(0),width(0),height(0),dirty(true) { }
};

#endif

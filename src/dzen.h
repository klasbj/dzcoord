#ifndef DZEN_H
#define DZEN_H

#include <cstdio>
#include <string>

#include "dock.h"

class dzen_t {
  public:

  dzen_t(const int screen, const dock_position_t position);
  virtual ~dzen_t();

  void set_size(size_t width, size_t height);

  void write(const std::string & left, const std::string & right,
             const size_t right_len) const;

  void start_dzen();
  void stop_dzen();


  private:
  int screen;
  size_t width, height;
  dock_position_t position;
  bool dirty;

  /* child process */
  FILE * pipe;
};

#endif

#include "dzen.h"

#include <cstdio>
#include <string>

dzen_t::dzen_t(const int screen, const dock_position_t position) :
  screen(screen), width(0), height(0), position(position), dirty(true), pipe(NULL) { }


dzen_t::~dzen_t() {
  stop_dzen();
}

void dzen_t::set_size(size_t width, size_t height) {
  this->width = width;
  this->height = height;
  if (this->pipe) {
    fprintf(this->pipe, "^screen()\n");
    fflush(this->pipe);
  }
}

void dzen_t::write(const std::string & left, const std::string & right,
               const size_t right_pos) const {
  fprintf(this->pipe, "%s^pa(%zu)^fg(black)^r(%zux15)^fg(#e0ffff)^pa(%zu)%s\n",
      left.c_str(), right_pos, width - right_pos, right_pos, right.c_str());
  fflush(this->pipe);
}
  
void dzen_t::start_dzen() {
  char cmdbuf[1024];
  if (this->pipe != NULL)
    return;

  sprintf(cmdbuf, "/usr/bin/dzen2 -xs \"%d\" -y \"%d\" -h \"%d\" -fg \"%s\" -bg \"%s\" -ta l -fn '%s' -e 'onstart=lower'",
      this->screen + 1, this->position == DOCK_TOP ? 0 : -15, 15, "#e0ffff", "black",
      "-*-terminus-*-*-*-*-12-*-*-*-*-*-*-");
  //fprintf(stderr, "run: %s\n", cmdbuf);
  this->pipe = popen(cmdbuf, "w");
  if (this->pipe == NULL)
    perror("unable to open dzen");
}
void dzen_t::stop_dzen() {
  if (this->pipe != NULL)
    if (pclose(this->pipe))
      perror("unable to close dzen");
  this->pipe = NULL;
}

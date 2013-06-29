#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <set>
#include "area.h"
#include "parser.h"
#include "options.h"
#include "dzen.h"

using namespace std;

#define MAX_SCREENS 5

const size_t LINE_BUF_SIZE = 1024*1024;

char line_buf[LINE_BUF_SIZE];

extern multiset<area_t*, area_t_lt> areas;

screen_t screen;
dzen_t dzen_right(1, DOCK_TOP);

int main() {
  for (int i = 0; i < MAX_SCREENS; ++i)
    screens.emplace_back(i);

//  screens[0].set_screen_size(0,0,SCREEN_WIDTH, 100);

  while (fgets(line_buf, LINE_BUF_SIZE, stdin)) {
    if (parse(line_buf)) {
      cerr << "Unable to parse line.\n";
    }
    for (int i = 0; i < MAX_SCREENS; ++i)
      screens[i].update();
  }

  return EXIT_SUCCESS;
}

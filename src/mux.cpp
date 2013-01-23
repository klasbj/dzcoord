#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <set>
#include "area.h"
#include "parser.h"
#include "options.h"

using namespace std;

const size_t LINE_BUF_SIZE = 1024*1024;

char line_buf[LINE_BUF_SIZE];

extern multiset<area_t*, area_t_lt> areas;

void print() {
  bool center_added = false;
  int left_pos = 0;
  int right_pos = SCREEN_WIDTH;

  string left = "", right = "";

  for (auto it = areas.begin(); it != areas.end(); ++it) {
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

  cout << left;
  cout << "^pa(" << right_pos << ")^fg(black)^r(" << SCREEN_WIDTH-right_pos <<
    "x15)^fg(#e0ffff)^pa(" << right_pos << ")";
  cout << right;

  cout << endl;
  cout.flush();
}

int main() {
  while (fgets(line_buf, LINE_BUF_SIZE, stdin)) {
    if (parse(line_buf)) {
      cerr << "Unable to parse line.\n";
    }
    
    print();
  }
}

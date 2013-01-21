#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <set>
#include "area.h"
#include "parser.h"

using namespace std;

extern const size_t SEPARATOR_WIDTH;
extern const string SEPARATOR;

const size_t LINE_BUF_SIZE = 1024*1024;

char line_buf[LINE_BUF_SIZE];

extern multiset<area_t*, area_t_lt> areas;

void print() {
  int left_pos = 0;
  int right_pos = 1366;

  string left = "", right = "";

  for (auto it = areas.begin(); it != areas.end(); ++it) {
    if ((*it)->fl == FLOAT_LEFT) {
      left_pos += (*it)->width;
      left += (*it)->prints;
      if ((*it)->width > 0) {
        left += SEPARATOR;
        left_pos += SEPARATOR_WIDTH;
      }
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
  cout << "^pa(" << right_pos << ")^fg(black)^r(" << 1366-right_pos <<
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

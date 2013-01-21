#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include "parser.h"
#include "area.h"

using namespace std;

extern unordered_map<string, area_t> areas;

const int
  RET_SUCCESS   = 0,
  RET_FAIL      = 1;


static const size_t CHAR_WIDTH = 6;
static const size_t SCREEN_WIDTH = 1366;

#define _FGNORMAL_  "^fg(#e0ffff)"
#define _FGFOCUS_   "^fg(#1994d1)"
#define _SPACE_     "^p(4)"
#define _SEP_CHAR_  "|"

static const size_t SPACE_WIDTH = 4;
static const string SPACE = _SPACE_;

static const size_t SEPARATOR_WIDTH = (CHAR_WIDTH+2*SPACE_WIDTH);
static const string SEPARATOR = _SPACE_ _FGFOCUS_ _SEP_CHAR_ _FGNORMAL_ _SPACE_;

typedef enum {
  LINE_ADD_AREA = 0,
  LINE_NEW_TEXT,
  LINE_LAST 
} line_t;

static string LINE_WORDS[LINE_LAST] = {
  "add_area",
  "text"
};

/*
 * Parses string and executes what is necessary to alter the areas struct.
 */
int parse(const char * str) {
  /* check what the first word is */
  size_t f = 0, t = 0;
  const char * s = str;
  while (isspace(str[t])) { ++t; }
  f = t;
  while (!isspace(str[t])) { ++t; }
  string word(&str[f], &str[t]);
  /* compare to the known types */
  line_t l = LINE_LAST;
  for (int i = 0; i < (int)LINE_LAST; ++i)
    if (word == LINE_WORDS[i])
      l = (line_t)i;

  /* find the start of the next word */
  while (isspace(str[t])) { ++t; }

  s = &str[t];

  if (l == LINE_ADD_AREA) {
    area_t new_area;

    if (parse_add_area(s, new_area)) {
      cerr << "Unable to parse add_area.\n";
      return RET_FAIL;
    } else {
      areas[new_area.id] = new_area;
    }
  } else if (l == LINE_NEW_TEXT) {
    size_t width = 0;
    string id;
    string res;

    if (parse_new_text(s, id, width, res)) {
      cerr << "Unable to parse text.\n";
      return RET_FAIL;
    } else {
      auto it = areas.find(id);
      if (it != areas.end()) {
        it->second.width = width;
        it->second.prints = res;
      } else {
        cerr << "No such area: '" << id << "'.\n";
        return RET_FAIL;
      }
    }
  } else {
    cerr << "Unknown line type: '" << word << "'.\n";
    return RET_FAIL;
  }

  return RET_SUCCESS;
}

/*
 * add_area id weight LEFT|RIGHT
 */
int parse_add_area(const char *& str, area_t & a) {
  size_t f = 0, t = 0;
  if (str[t] == '\0') return RET_FAIL;

  /* find the ID */
  while (!isspace(str[t])) { ++t; }
  a.id = string(&str[f], &str[t]);

  /* advance past the next word */
  while (isspace(str[t])) { ++t; }
  f = t;
  if (str[t] == '\0') return RET_FAIL;
  while (!isspace(str[t])) { ++t; }

  /* find the weight */
  string w(&str[f], &str[t]);
  try {
    a.weight = stoi(w);
  } catch (invalid_argument e) {
    cerr << "Unable to parse weight: '" << w << "'\n";
    return RET_FAIL;
  }

  /* advance past the next word */
  while (isspace(str[t])) { ++t; }
  f = t;
  if (str[t] == '\0') return RET_FAIL;
  while (!isspace(str[t])) { ++t; }

  /* find the float direction */
  string fl(&str[f], &str[t]);
  a.fl = FLOAT_LAST;
  for (int i = 0; i < (int)FLOAT_LAST; ++i)
    if (fl == float_names[i])
      a.fl = (float_e) i;

  if (a.fl == FLOAT_LAST) {
    cerr << "Unknown float value: '" << fl << "'.\n";
    return RET_FAIL;
  }

  if (a.fl == FLOAT_LEFT) {
    a.weight = -a.weight;
  }

  return RET_SUCCESS;
}

int parse_new_text(const char *& str, string & id, size_t & width, string & res) {
  size_t f = 0, t = 0;
  if (str[t] == '\0' || str[t] == '\n') return RET_FAIL;

  /* find the ID */
  while (!isspace(str[t])) { ++t; }
  id = string(&str[f], &str[t]);

  /* find the first char of the output */
  while (isspace(str[t])) { ++t; }
  str = &str[t];

  while (*str != '\n' && *str != '\0') {
    if (*str == '^') {
      ++str;
      if (parse_cmd(str, width, res))
        return RET_FAIL;
    } else {
      if (parse_text(str, width, res))
        return RET_FAIL;
    }
  }
  return RET_SUCCESS;
}

int parse_text(const char *& str, size_t & width, string & res) {
  const char * begin = str;
  while (*str != '^' && *str != '\n') { ++str; }
  res += string(begin, str);
  width += CHAR_WIDTH * (str - begin);
  return RET_SUCCESS;
}


typedef enum {
  CMD_IMG = 0,
  CMD_HASH,
  CMD_PIPE,
  CMD_LAST
} cmd_e;

static string CMD_WORDS[CMD_LAST] = {
  "img",
  "#",
  "|"
};

static int (*CMD_FCNS[CMD_LAST])(const char *&, size_t&, string&) {
  &parse_img,
  &parse_hash,
  &parse_pipe
};

/*
 * Commands:
 *  ^img(...)
 *  ^#(...)
 *  ^|(...)
 */
int parse_cmd(const char *& str, size_t & width, string & res) {
  const char * begin = str;
  while (*++str != '(') ;
  string scmd(begin, str);
  cmd_e cmd = CMD_LAST;
  for (int i = 0; i < (int)CMD_LAST; ++i)
    if (scmd == CMD_WORDS[i])
      cmd = (cmd_e) i;

  if (cmd == CMD_LAST) {
    cerr << "Unkown command: '" << scmd << "'.\n";
    return RET_FAIL;
  }

  if (CMD_FCNS[cmd](str, width, res))
    return RET_FAIL;

  return RET_SUCCESS;
}

/*
 * ^img(filename,width)
 */
int parse_img(const char *& str, size_t & width, string & res) {
  /* get filename */
  const char * begin = ++str;
  while (*++str != ',') ;
  string file(begin, str);
  
  /* get width */
  begin = ++str;
  while (*++str != ')') ;
  string swidth(begin, str);
  try {
    width += stoul(swidth);
  } catch (invalid_argument e) {
    cerr << "Unable to parse image width.\n";
    return RET_FAIL;
  }
  ++str;

  res += "^i(" + file + ")";
  return RET_SUCCESS;
}

int parse_hash(const char *& str, size_t & width, string & res) {
  ++str;
  if (*str != ')') {
    cerr << "Unkown argument(s) to ^#.\n";
    return RET_FAIL;
  }
  ++str;
  
  res += SPACE;
  width += SPACE_WIDTH;

  return RET_SUCCESS;
}

int parse_pipe(const char *& str, size_t & width, string & res) {
  ++str;
  if (*str != ')') {
    cerr << "Unkown argument(s) to ^|.\n";
    return RET_FAIL;
  }
  ++str;

  res += SEPARATOR;
  width += SEPARATOR_WIDTH;

  return RET_SUCCESS;
}
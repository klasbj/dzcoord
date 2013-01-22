#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <set>
#include "parser.h"
#include "area.h"

using namespace std;

static unordered_map<string, area_t> area_map;
multiset<area_t*, area_t_lt> areas;

const int
  RET_SUCCESS   = 0,
  RET_FAIL      = 1;


static const size_t CHAR_WIDTH = 6;
static const size_t SCREEN_WIDTH = 1366;

#define _FGLOW_      "^fg(#2e3436)"
#define _FGNORMAL_  "^fg(#e0ffff)"
#define _FGFOCUS_   "^fg(#1994d1)"
#define _SPACE_     "^p(4)"
#define _SEP_CHAR_  "|"

static const size_t SPACE_WIDTH = 4;
static const string SPACE = _SPACE_;

extern const size_t SEPARATOR_WIDTH = (CHAR_WIDTH+2*SPACE_WIDTH);
extern const string SEPARATOR = _SPACE_ _FGFOCUS_ _SEP_CHAR_ _FGNORMAL_ _SPACE_;

typedef enum {
  LINE_ADD_AREA = 0,
  LINE_NEW_TEXT,
  LINE_LAST 
} line_t;

static string LINE_WORDS[LINE_LAST] = {
  "add_area",
  "text"
};

static inline bool iscmd(char ch) {
  return (
      ch == '^'   ||
      ch == '#'   ||
      ch == '|'   ||
      false
      );
}

static inline bool istext(char ch) {
  return !(
      ch == '\n'  ||
      ch == '\0'  ||
      iscmd(ch)   ||
      false
      );
}

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
      area_map[new_area.id] = new_area;
      areas.insert(&area_map[new_area.id]);
    }
  } else if (l == LINE_NEW_TEXT) {
    size_t width = 0;
    string id;
    string res;

    if (parse_new_text(s, id, width, res)) {
      cerr << "Unable to parse text.\n";
      return RET_FAIL;
    } else {
      auto it = area_map.find(id);
      if (it != area_map.end()) {
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
    if (iscmd(*str)) {
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
  while (istext(*++str)) ;
  res += string(begin, str);
  width += CHAR_WIDTH * (str - begin);
  return RET_SUCCESS;
}


typedef enum {
  CMD_IMG = 0,
  CMD_FOCUS,
  CMD_NORM,
  CMD_LOW,
  CMD_LAST
} cmd_e;

static string CMD_WORDS[CMD_LAST] = {
  "i",
  "focus",
  "norm",
  "low",
};

static int (*CMD_FCNS[CMD_LAST + 1])(const char *&, size_t&, string&) {
  &parse_img,
  &parse_focus,
  &parse_norm,
  &parse_low,
  &parse_unkown_command,
};

/*
 * Commands:
 *  ^xxx(...)
 *  #
 *  |
 */
int parse_cmd(const char *& str, size_t & width, string & res) {
  char ch = *str++;
  if (*str == ch) {
    /* doubled, print the character */
    ++str;
    res += ch;
    width += CHAR_WIDTH;

    return RET_SUCCESS;
  }
  switch (ch) {
    case '^':
      return parse_long_cmd(str, width, res);
      break;
    case '#':
      return parse_hash(str, width, res);
      break;
    case '|':
      return parse_pipe(str, width, res);
      break;
    default:
      cerr << "Unkown command char: '" << ch << "'.\n";
      return RET_FAIL;
      break;
  }
  return RET_SUCCESS;
}

/*
 * Commands:
 *  ^i(...)
 *  ^focus(...)
 *  ^norm(...)
 *  ^low(...)
 */
int parse_long_cmd(const char *& str, size_t & width, string & res) {
  const char * begin = str;
  while (*++str != '(') ;
  string scmd(begin, str);
  cmd_e cmd = CMD_LAST;
  for (int i = 0; i < (int)CMD_LAST; ++i)
    if (scmd == CMD_WORDS[i])
      cmd = (cmd_e) i;

  return CMD_FCNS[cmd](str, width, res);
}

/*
 * ^i(filename,width)
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

/*
 * ^i(filename,width)
 */
int parse_unkown_command(const char *& str, size_t & width, string & res) {
  /* find the whole argument */
  const char * begin = str;
  while (*--begin != '^') ;
  while (*++str != ')') ;
  string cmd(begin, str);

  /* add the command */
  /* unkown commands have zero width */
  res += cmd;

  return RET_SUCCESS;
}

/*
 * simple commands
 */
int parse_hash(const char *& str, size_t & width, string & res) {
  res += SPACE;
  width += SPACE_WIDTH;

  return RET_SUCCESS;
}

int parse_pipe(const char *& str, size_t & width, string & res) {
  res += SEPARATOR;
  width += SEPARATOR_WIDTH;

  return RET_SUCCESS;
}

int parse_focus(const char *& str, size_t & width, std::string & res) {
  if (*++str != ')') {
    cerr << "Unkown argument to ^focus()." << endl;
    return RET_FAIL;
  }
  ++str;
  res += _FGFOCUS_;
  return RET_SUCCESS;
}

int parse_norm(const char *& str, size_t & width, std::string & res) {
  if (*++str != ')') {
    cerr << "Unkown argument to ^norm()." << endl;
    return RET_FAIL;
  }
  ++str;
  res += _FGNORMAL_;
  return RET_SUCCESS;
}

int parse_low(const char *& str, size_t & width, std::string & res) {
  if (*++str != ')') {
    cerr << "Unkown argument to ^low()." << endl;
    return RET_FAIL;
  }
  ++str;
  res += _FGLOW_;
  return RET_SUCCESS;
}

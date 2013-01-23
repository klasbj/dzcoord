#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define FG(x) "^fg(" x ")"
#define BG(x) "^bg(" x ")"
#define P(x)  "^p(" x ")"

#define _DEFAULT_SCREEN_WIDTH_  1366
#define _DEFAULT_CHAR_WIDTH_    6
#define _DEFAULT_SPACE_WIDTH_   4

#define _DEFAULT_COLOR_LOW_    "#2e3436"
#define _DEFAULT_COLOR_NORMAL_ "#e0ffff"
#define _DEFAULT_COLOR_FOCUS_  "#1994d1"
#define _DEFAULT_BG_COLOR_     "black"

#define _DEFAULT_FGLOW_       FG( _DEFAULT_COLOR_LOW_        )
#define _DEFAULT_FGNORMAL_    FG( _DEFAULT_COLOR_NORMAL_     )
#define _DEFAULT_FGFOCUS_     FG( _DEFAULT_COLOR_FOCUS_      )
#define _DEFAULT_SPACE_        P( STR(_DEFAULT_SPACE_WIDTH_) )
#define _DEFAULT_SEP_CHAR_    "|"

extern size_t SCREEN_WIDTH;
extern size_t CHAR_WIDTH;
extern size_t SPACE_WIDTH;
extern size_t SEPARATOR_WIDTH;

extern std::string SPACE;
extern std::string SEPARATOR_CHARS;
extern std::string SEPARATOR;

extern std::string COLOR_LOW;
extern std::string COLOR_NORMAL;
extern std::string COLOR_FOCUS;
extern std::string COLOR_BG;

extern std::string FG_LOW;
extern std::string FG_NORMAL;
extern std::string FG_FOCUS;
extern std::string BG;

#endif

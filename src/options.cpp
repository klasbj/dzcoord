#include "options.h"

size_t SCREEN_WIDTH     = _DEFAULT_SCREEN_WIDTH_;
size_t CHAR_WIDTH       = _DEFAULT_CHAR_WIDTH_;
size_t SPACE_WIDTH      = _DEFAULT_SPACE_WIDTH_;
size_t SEPARATOR_WIDTH  = CHAR_WIDTH + 2*SPACE_WIDTH;

size_t LINE_HEIGHT      = _DEFAULT_LINE_HEIGHT_;

std::string FONT    = _DEFAULT_FONT_;

std::string SPACE   = _DEFAULT_SPACE_;
std::string SEPARATOR_CHARS = _DEFAULT_SEP_CHAR_;
std::string SEPARATOR =
  _DEFAULT_SPACE_
  _DEFAULT_FGFOCUS_
  _DEFAULT_SEP_CHAR_
  _DEFAULT_FGNORMAL_
  _DEFAULT_SPACE_;

std::string COLOR_LOW     = _DEFAULT_COLOR_LOW_;
std::string COLOR_NORMAL  = _DEFAULT_COLOR_NORMAL_;
std::string COLOR_FOCUS   = _DEFAULT_COLOR_FOCUS_;
std::string COLOR_BG      = _DEFAULT_BG_COLOR_;

std::string FG_LOW    = FG( _DEFAULT_COLOR_LOW_     );
std::string FG_NORMAL = FG( _DEFAULT_COLOR_NORMAL_  );
std::string FG_FOCUS  = FG( _DEFAULT_COLOR_FOCUS_   );
std::string BG        = BG( _DEFAULT_BG_COLOR_      );

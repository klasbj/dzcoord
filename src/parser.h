#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "area.h"

/*
 * text^cmd(args)text
 */

int parse(const char * str);
int parse_add_area(const char *& str, area_t & a);
int parse_rm_area(const char *& str, std::string & id);
int parse_new_text(const char *& str, std::string & id, size_t & width, std::string & res);
int parse_text(const char *& str, size_t & width, std::string & res);
int parse_cmd(const char *& str, size_t & width, std::string & res);
int parse_long_cmd(const char *& str, size_t & width, std::string & res);
int parse_unkown_command(const char *& str, size_t & width, std::string & res);
int parse_img(const char *& str, size_t & width, std::string & res);
int parse_focus(const char *& str, size_t & width, std::string & res);
int parse_norm(const char *& str, size_t & width, std::string & res);
int parse_low(const char *& str, size_t & width, std::string & res);
int parse_hash(const char *& str, size_t & width, std::string & res);
int parse_pipe(const char *& str, size_t & width, std::string & res);


#endif

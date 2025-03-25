#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include <string>

std::string get_sys_time(const std::string& req_format);
std::string parse_response(std::string res);

#endif // !RESPONSE_PARSER_H

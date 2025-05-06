#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include <string>
#include <vector>

std::string getCity(std::string lat, std::string lon);
std::vector<std::string> parse_response(const std::string& res, const std::string lat, const std::string lon, const std::string req_time);

#endif // !RESPONSE_PARSER_H

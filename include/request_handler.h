#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
bool get_coords(const std::string city, std::string& lon, std::string& lat);
std::string request_data(std::string latitude, std::string longitude, const std::string& req_time);

#endif // !REQUEST_HANDLER_H

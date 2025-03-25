#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
bool get_coords(const std::string city, double& lon, double& lat);
std::string request_data(std::string latitude, std::string longitude);

#endif // !REQUEST_HANDLER_H

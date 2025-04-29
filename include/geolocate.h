#ifndef GEOLOCATE_H
#define GEOLOCATE_H

#include <string>

void geolocate_user(std::string& lat, std::string& lon);
void parse_loc_str(std::string loc_str, std::string& lat, std::string& lon);

#endif // !GEOLOCATE_H

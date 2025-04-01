#ifndef CACHE_H
#define CACHE_H

#include "pch.h"

void write_to_cache(const std::string& filename, std::vector<std::string> write_data);
nlohmann::json read_from_cache(const std::string& filename);
void clear_cache(const std::string& filename);

#endif // !CACHE_H

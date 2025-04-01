#include "../include/pch.h"
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

json read_from_cache(const std::string& filename){
  json data;

  std::ifstream file(filename);

  if(file.is_open()){
    file >> data;
    file.close();
  }

  return data;
}

void write_to_cache(const std::string& filename, std::vector<std::string> write_data){
  json data;

  std::vector<std::string> favorite_cities = read_from_cache("../cache/user_cache.json")["favorite_cities"];

  for(int i = 0; i < write_data.size(); ++i){
    if(std::find(favorite_cities.begin(), favorite_cities.end(), write_data[i]) != favorite_cities.end()){
      continue;
    }
    else{
      favorite_cities.push_back(write_data[i]);
    }
  }

  data["favorite_cities"] = favorite_cities;

  std::ofstream file(filename);

  if(file.is_open()){
    file << data.dump(4);
    file.close();
  }

  read_from_cache("../cache/user_cache.json");
}

void clear_cache(const std::string& filename){
  json data;

  data["favorite_cities"] = json::array();

  std::ofstream file(filename);

  if(file.is_open()){
    file << data.dump(4);
    file.close();
    std::cout << "Cache Cleared..." << std::endl;
  }
}


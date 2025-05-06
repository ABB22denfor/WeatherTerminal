#include "../include/pch.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>

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

void write_to_cache(const std::string& filename, std::vector<std::string> write_data, std::vector<std::string> coords){
  json data;
  std::vector<std::string> favorite_cities = read_from_cache("./cache/user_cache.json")["favorite_cities"];
  std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> data_map = read_from_cache("./cache/user_cache.json")["data"];

  if(coords.empty()){
    for(int i = 0; i < write_data.size(); ++i){
      if(std::find(favorite_cities.begin(), favorite_cities.end(), write_data[i]) != favorite_cities.end()){
        // Skip if city already saved in cache
        continue;
      }
      else{
        favorite_cities.push_back(write_data[i]);
      }
    }
  }else{
    std::string coords_str = coords[0] + " " + coords[1];

    if(write_data.size() == 1){
      data_map[coords_str]["now"] = write_data;
    }
    else if(write_data.size() == 24){
      data_map[coords_str]["day"] = write_data;
    }
    else if(write_data.size() == 168){
      data_map[coords_str]["week"] = write_data;
    }
  }
  
  data["favorite_cities"] = favorite_cities;
  data["data"] = data_map;


  std::ofstream file(filename);

  if(file.is_open()){
    file << data.dump(4);
    file.close();
  }
}

void clear_cache(const std::string& filename){
  json data;

  // Set to empty array
  data["favorite_cities"] = json::array();
  data["data"] = json::object();

  std::ofstream file(filename);

  if(file.is_open()){
    file << data.dump(4);
    file.close();
    std::cout << "Cache Cleared..." << std::endl;
  }
}


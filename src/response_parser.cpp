#include <iostream>
#include <ctime>

#include <nlohmann/json.hpp>

#include "../include/response_parser.h"

std::string_view parse_response(std::string_view res){
  nlohmann::json json_res = nlohmann::json::parse(res);

  std::vector<std::string> time_array = json_res["hourly"]["time"].get<std::vector<std::string>>();
  std::vector<int> temp_array = json_res["hourly"]["temperature_2m"].get<std::vector<int>>();

  if(time_array.size() != temp_array.size()){
    std::cerr << "ERR: Invalid Fetch";
  }

  for(size_t i = 0; i < time_array.size(); ++i) {
      std::ostringstream oss; //For outputing string with different variable types

      oss << time_array[i] << '\t' << temp_array[i] << "°C";
      
      std::cout << oss.str() << '\n';
  }

  return res;
}

#include <iostream>
#include <ctime>

#include <nlohmann/json.hpp>

#include "response_parser.h"


std::string get_sys_time(const std::string& req_format){
  time_t now = time(nullptr);
  tm* local_time = localtime(&now);

  char buffer[80];
  strftime(buffer, sizeof(buffer), req_format.c_str(), local_time);

  return std::string(buffer);
}

std::string parse_response(std::string res){
  std::string curr_hour = get_sys_time("%H");
  nlohmann::json json_res = nlohmann::json::parse(res);

  std::vector<std::string> time_array = json_res["hourly"]["time"].get<std::vector<std::string>>();
  std::vector<int> temp_array = json_res["hourly"]["temperature_2m"].get<std::vector<int>>();

  if(time_array.size() != temp_array.size()){
    std::cout << "ERR: Invalid Fetch";
  }

  for(size_t i = 0; i < time_array.size(); ++i) {
      std::ostringstream oss; //For outputing string with different variable types

      oss << time_array[i] << '\t' << temp_array[i] << "°C";
      
      std::cout << oss.str() << '\n';
  }

  return res;
}

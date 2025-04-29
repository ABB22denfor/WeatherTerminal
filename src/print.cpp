#include "../include/pch.h"

#include "../include/cache.h"
#include "../include/request_handler.h"
#include "../include/response_parser.h"

using json = nlohmann::json;

void print_favorites(){
  std::vector<std::string> favorite_cities = read_from_cache("../cache/user_cache.json")["favorite_cities"];

  std::string longitude, latitude;

  for(int i = 0; i < favorite_cities.size(); ++i){
    get_coords(favorite_cities[i], longitude, latitude);
    std::cout << favorite_cities[i] << '\t';
    parse_response(request_data(latitude, longitude, "now"), latitude, longitude, "now");
  }  
}

void print_data(const std::string& data_str, int index){
  if(((index + 1) % 2) == 0 && index != 0){
    std::cout << data_str << '\n';
  }
  else if(index == 9999){
    std::cout << data_str << '\n';
  }
  else{
    std::cout << data_str << '\t' << '\t';
  }

  if(((index + 1) % 24) == 0){
    std::cout << '\n';
  }
}

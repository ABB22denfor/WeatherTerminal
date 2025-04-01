//  Standard libraries:
#include "../include/pch.h"
#include <cstring>

// Header files:
#include "../include/response_parser.h"
#include "../include/request_handler.h"
#include "../include/cache.h"
#include "../include/print.h"

std::vector<std::string> favorite_cities;

bool isFloat(const std::string& s) {
  char* end = nullptr;
  std::strtof(s.c_str(), &end);
  return end != s.c_str() && *end == '\0';
}


void execute_args(int argc, char* argv[]){
  const std::unordered_set<std::string> validTimePeriods = {"now", "day", "week"};
  std::string longitude, latitude;

  std::string response;
  std::string req_time = "day";

  bool verbose = false;


  for(int i = 1; i < argc; ++i){
    std::string flag = argv[i];

    if(flag == "-p" || flag == "--position"){
      if((i+2 < argc) && isFloat(argv[i+1]) && isFloat(argv[i+2])){
        // If input is coordinates:
        latitude = argv[i+1];
        longitude = argv[i+2];
      }
      else if(i+1 < argc){
        // If input is city:
        get_coords(argv[i+1], longitude, latitude);
      }
      else{
        std::cerr << "Options for flag omitted. Aborting..." << '\n';
        return;
      }
    }
    else if(flag == "-t" || flag == "--time"){
      if(i+1 < argc && (validTimePeriods.find(argv[i+1]) != validTimePeriods.end())){
        req_time = argv[i+1];
      }
      else if(i+1 == argc){
        std::cerr << "Options for flag omitted. Aborting..." << '\n';
        return;
      }
      else{
        std::cerr << "Option invalid. See documentation..." << '\n';
        return;
      }
    }
    else if(flag == "-f" || flag == "--favorite"){
      for(int j = 1; i+j < argc; ++j){
        if(strchr(argv[i+j], '-') != nullptr){
          break;
        }
        favorite_cities.push_back(argv[i+j]);
      }
      write_to_cache("../cache/user_cache.json", favorite_cities);
    }
    else if(flag == "-c" || flag == "--clear"){
      clear_cache("../cache/user_cache.json");
    }
    else if(flag == "-v" || flag == "--verbose"){
      if(i+1 < argc){
        verbose = ((strcmp(argv[i+1], "true")) == 0) ? true : false;
      } 
    }
  }

  if(!latitude.empty() && !longitude.empty()){
    response = request_data(latitude, longitude, req_time);
    parse_response(response); 
  }
  else{
    print_favorites();
  }
}

int main(int argc, char* argv[]){
  execute_args(argc, argv);

  
  return 0;
}

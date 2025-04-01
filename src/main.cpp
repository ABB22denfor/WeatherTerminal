//  Standard libraries:
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

// Header files:
#include "../include/response_parser.h"
#include "../include/request_handler.h"


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
        std::cerr << "Options for flag omitted. Aborti, req_timeng..." << '\n';
        return;
      }
      else{
        std::cerr << "Option invalid. See documentation..." << '\n';
        return;
      }

    }
  }

  response = request_data(latitude, longitude, req_time);
  parse_response(response); 
}

int main(int argc, char* argv[]){
  execute_args(argc, argv);
  
  return 0;
}

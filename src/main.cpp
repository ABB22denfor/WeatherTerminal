//  Standard libraries:
#include <iostream>

// Header files:
#include "response_parser.h"
#include "request_handler.h"


bool isFloat(const std::string& s) {
    char* end = nullptr;
    std::strtof(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}


void parse_args(int argc, char* argv[]){
  for(int i = 1; i < argc; ++i){
    std::string flag = argv[i];

    if(flag == "-p" || flag == "--position"){
      if((i+2 < argc) && isFloat(argv[i+1]) && isFloat(argv[i+2])){
        std::cout << "LONG AND LAT";
      }
      else if(i+1 < argc){
        std::cout << "CITY";
      }
    }
  }

}

int main(int argc, char* argv[]){
  std::string response_data;
  std::string city;

  parse_args(argc, argv);
  
  
  double longitude, latitude;
  if(get_coords(city, longitude, latitude)){
    std::cout << city << std::endl;
  }
  response_data = request_data(std::to_string(latitude), std::to_string(longitude));

  parse_response(response_data);

  
  return 0;
}

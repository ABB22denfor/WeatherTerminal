//  Standard libraries:
#include <vector>
#include <string>

// Header files:
#include "../include/response_parser.h"
#include "../include/request_handler.h"


bool isFloat(const std::string& s) {
    char* end = nullptr;
    std::strtof(s.c_str(), &end);
    return end != s.c_str() && *end == '\0';
}


void execute_args(int argc, char* argv[]){
  std::vector<char*> parsed_args;
  for(int i = 1; i < argc; ++i){
    std::string flag = argv[i];

    if(flag == "-p" || flag == "--position"){
      if((i+2 < argc) && isFloat(argv[i+1]) && isFloat(argv[i+2])){
        // If input is coordinates:
        parse_response(request_data(argv[i+1], argv[i+2])); 
      }
      else if(i+1 < argc){
        // If input is city:
        double longitude, latitude;
        get_coords(argv[i+1], longitude, latitude);
        parse_response(request_data(std::to_string(latitude), std::to_string(longitude)));
      }
    }
    else if(flag == "-t"){
      if(i+1 < argc){
        parsed_args.push_back(argv[i+1]);
      }
    }
  }
  
}

int main(int argc, char* argv[]){
  std::string response_data;
  std::string city;

  execute_args(argc, argv);

  /*
  
  double longitude, latitude;
  if(get_coords(city, longitude, latitude)){
    std::cout << city << std::endl;
  }
  response_data = request_data(std::to_string(latitude), std::to_string(longitude));

  parse_response(response_data);*/

  
  return 0;
}

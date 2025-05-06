//  Standard libraries:
#include <cstring>

// Header files:
#include "../include/pch.h"
#include "../include/response_parser.h"
#include "../include/request_handler.h"
#include "../include/cache.h"
#include "../include/print.h"
#include "../include/geolocate.h"
#include "../include/notify.h"

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
  bool added_favorites = false;

  std::vector<std::string> formatted_data;

  std::string notif_delay;
  bool send_notif;


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
        std::cout << "Option invalid. Use ./WeatherApp -h for more info...\n";
        return;
      }
    }
    else if(flag == "-t" || flag == "--time"){
      if(i+1 < argc && (validTimePeriods.find(argv[i+1]) != validTimePeriods.end())){
        req_time = argv[i+1];
      }
      else{
        std::cout << "Option invalid. Use ./WeatherApp -h for more info...\n";
        return;
      }
    }
    else if(flag == "-f" || flag == "--favorite"){
      if(i+1 == argc){
        std::cout << "Option invalid. Use ./WeatherApp -h for more info...\n";
        return;
      }
      for(int j = 1; i+j < argc; ++j){
        if(strchr(argv[i+j], '-') != nullptr){
          break;
        }
        favorite_cities.push_back(argv[i+j]);
      }
      write_to_cache("../cache/user_cache.json", favorite_cities, {});

      added_favorites = true;
    }
    else if(flag == "-c" || flag == "--clear"){
      std::cout << "Work In Progress... Will break application\n";
      return;
    }
    else if(flag == "-n" || flag == "--notification"){
      if(i+1 < argc){
        notif_delay = argv[i+1];
        send_notif = true;
      }
      else{
        std::cout << "Option invalid. Use ./WeatherApp -h for more info...\n";
        return;
      }
    }
    else if(flag == "-h" || flag == "--help"){
      print_help();
      return;
    }
  }

  if(!latitude.empty() && !longitude.empty()){
    if(send_notif){
      send_notification(notif_delay, latitude, longitude);
      return;
    }

    response = request_data(latitude, longitude, req_time);
    formatted_data = parse_response(response, latitude, longitude, req_time); 

    if(formatted_data[0] == response){
      return;
    }
    write_to_cache("../cache/user_cache.json", formatted_data, {latitude, longitude});
  }
  else if (added_favorites){
    return;
  }
  else{
    if(!print_favorites()){
      geolocate_user(latitude, longitude);
      req_time = "now";
      response = request_data(latitude, longitude, req_time);
      formatted_data = parse_response(response, latitude, longitude, req_time); 
      if(formatted_data[0] == response){
        return;
      }
      write_to_cache("../cache/user_cache.json", formatted_data, {latitude, longitude});
    }
  }
}

int main(int argc, char* argv[]){
  execute_args(argc, argv);

  
  return 0;
}

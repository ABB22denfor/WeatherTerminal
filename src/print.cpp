#include "../include/pch.h"

#include "../include/cache.h"
#include "../include/request_handler.h"
#include "../include/response_parser.h"

using json = nlohmann::json;

bool print_favorites(){
  std::vector<std::string> favorite_cities = read_from_cache("./cache/user_cache.json")["favorite_cities"];

  std::string longitude, latitude;

  for(int i = 0; i < favorite_cities.size(); ++i){
    get_coords(favorite_cities[i], longitude, latitude);
    std::cout << favorite_cities[i] << '\t';
    parse_response(request_data(latitude, longitude, "now"), latitude, longitude, "now"); 
  }

  if(favorite_cities.size() == 0){
    return false;
  }
  else{
    return true;
  }
}

void print_help(){
  std::string help_str = R"(Usage: ./WeatherApp [<options>]

Flags:
- -p | --position :  
    Position of which the app will base it's forecast. Permitted options are: "city" or "lat lon".  
(default: "None")
- -t | --time :  
    Time period of which the app will forecast. Permitted options are "now", "day", "week" or "month".  
(default: "now")
- -f | --favorite :  
    Favorite city or location that the app will save in database. No limit to amount of favorite cities.  
(default: "None")
- -c | --clear:  
    Clears the cache of all saved data.  
(default: "None")
- -n | --notification:
    Sets interval for which to send notifications of current data. Minimum of 15 minutes. 
(default: "None")
)";
  std::cout << help_str;
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

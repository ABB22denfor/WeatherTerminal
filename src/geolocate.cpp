#include <curl/curl.h>

#include "nlohmann/json.hpp"
#include "../include/request_handler.h"


void parse_loc_string(std::string loc_str, std::string& lat, std::string& lon){
  size_t comma_pos = loc_str.find(",");

  if(comma_pos != std::string::npos){
    lat = loc_str.substr(0, comma_pos);
    lon = loc_str.substr(comma_pos + 1);
  }
}

void geolocate_user(std::string& lat, std::string& lon) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://ipinfo.io/json"); // Example geolocation API
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        std::string loc_str = nlohmann::json::parse(response)["loc"];

        parse_loc_string(loc_str, lat, lon);
    }
}



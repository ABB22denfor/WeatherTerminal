#include "../include/pch.h"

#include "../include/request_handler.h"
#include "../include/cache.h"
#include "../include/print.h"

#include <unordered_map>
#include <cstdlib>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output){
  size_t totalSize = size * nmemb;
  output->append((char*)contents, totalSize);
  return totalSize;
}

std::string get_sys_time(std::string format){
  std::time_t now = std::time(nullptr);

  // Convert to GMT+2
  now += 2 * 60 * 60;

  std::tm* utc_tm = std::gmtime(&now);

  std::ostringstream oss;
  oss << std::put_time(utc_tm, format.c_str());

  return oss.str();
}

bool check_time_difference(const std::string& curr_time, const std::string& stored_time, const int diff){
  int curr_h, curr_m, stored_h, stored_m;

  sscanf(curr_time.c_str(), "%d:%d", &curr_h, &curr_m);
  sscanf(stored_time.c_str(), "%d:%d", &stored_h, &stored_m);

  int curr_minutes = curr_h * 60 + curr_m;
  int stored_minutes = stored_h * 60 + stored_m;

  int difference = std::abs(curr_minutes - stored_minutes);

  return difference >= diff;
}

bool get_coords(const std::string city, std::string& lon, std::string& lat){
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    std::string response;
    std::string url = "https://nominatim.openstreetmap.org/search?q=" + city + "&format=json&limit=1";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return false;

    try {
        auto jsonData = nlohmann::json::parse(response);
        if (!jsonData.empty()) {
            lat = jsonData[0]["lat"].get<std::string>();
            lon = jsonData[0]["lon"].get<std::string>();
            return true;
        }
    } catch (...) {
        return false;
    }

    return false;
}

std::string request_data(std::string latitude, std::string longitude, const std::string& req_time){
  CURL* curl;
  CURLcode res;

  bool new_req = false;

  std::string req_url;
  std::string time_format;

  std::string coord_str = latitude + " " + longitude;

  std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> data_map = read_from_cache("../cache/user_cache.json")["data"];

  
  if(req_time == "day"){
    req_url = std::format("https://api.open-meteo.com/v1/forecast?latitude={}&longitude={}&hourly=temperature_2m,wind_speed_10m,precipitation,weather_code&timezone=Europe%2FBerlin&forecast_days=1&wind_speed_unit=ms", latitude, longitude); 
  }
  else if(req_time == "now"){
    req_url = std::format("https://api.open-meteo.com/v1/forecast?latitude={}&longitude={}&current=temperature_2m,wind_speed_10m,precipitation,weather_code&timezone=Europe%2FBerlin&wind_speed_unit=ms", latitude, longitude);
  }
  else if(req_time == "week"){
    req_url = std::format("https://api.open-meteo.com/v1/forecast?latitude={}&longitude={}&hourly=temperature_2m,wind_speed_10m,precipitation,weather_code&timezone=Europe%2FBerlin&forecast_days=7&wind_speed_unit=ms", latitude, longitude);
  }

  if(data_map.find(coord_str) != data_map.end()){
    if(data_map[coord_str].find(req_time) != data_map[coord_str].end()){
      std::string empty_str = " ";

      std::string i_data_str = data_map[coord_str][req_time][0];
      
      std::size_t colon_pos = i_data_str.find(":");

      std::string stored_date = i_data_str.substr(0, colon_pos);
      std::string stored_rest = i_data_str.substr(colon_pos, -1);

      std::istringstream iss(stored_rest);

      std::string colon, stored_time;
      iss >> colon >> stored_time;

      if(stored_date == get_sys_time("%B %d %Y")){
        if(check_time_difference(get_sys_time("%H:%M"), stored_time, 15) && req_time == "now"){
          new_req = true;
        }
      }
      else{
        new_req = true;
      }
      if(!new_req){
        std::cout << "OUTPUT FROM CACHE \n";
        for(int i = 0; i < data_map[coord_str][req_time].size(); ++i){
          if(i == 0 && i+1 == data_map[coord_str][req_time].size()){
            print_data(data_map[coord_str][req_time][i], 9999);
          }
          else{
            print_data(data_map[coord_str][req_time][i], i);
          }
        }
        return data_map[coord_str]["now"][0];
      }
    }
  }

  
  std::string response_data;
  
  curl = curl_easy_init();
  if(curl){
    curl_easy_setopt(curl, CURLOPT_URL, req_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK){
      std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
  }

  return std::move(response_data);
}

#include "../include/pch.h"

#include "../include/response_parser.h"
#include "../include/request_handler.h"
#include "../include/print.h"

#include <sstream>
#include <cmath>
#include "../include/date/date.h"

std::string formatTime(const std::string& iso8601Time){
  std::istringstream ss(iso8601Time);
  date::sys_seconds tp;

  ss >> date::parse("%FT%R", tp);

  if(ss.fail()){
    return "Invalid ISO 8601 date";
  }

  std::ostringstream out;

  out << date::format("%B %d %Y: %H:%M", tp);

  return out.str();
}


std::string getCity(std::string lat, std::string lon){
    CURL* curl = curl_easy_init();
    if (!curl) return "ERROR, CURL NOT FOUND!";

    std::string response;
    std::string url = std::format("https://nominatim.openstreetmap.org/reverse?lat={}&lon={}&zoom=10&format=json&limit=1", stod(lat), stod(lon));

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return "ERROR, CURL NOT WORKING \n";

    try {
        auto jsonData = nlohmann::json::parse(response);
        if (!jsonData.empty()) {
            std::string city = jsonData["name"].get<std::string>();
            return city;
        }
    } catch (...) {
        return "ERROR IN PARSING \n";
    }

    return "ERROR, JSONDATA EMPTY \n";

}

std::string parse_weather_code(const int& weather_code){
    static const std::unordered_map<int, std::string> wmoDesc = {
        {0, "Clear sky"},
        {1, "Mainly clear"},
        {2, "Partly cloudy"},
        {3, "Overcast"},
        {45, "Fog"},
        {48, "Depositing rime fog"},
        {51, "Light drizzle"},
        {53, "Moderate drizzle"},
        {55, "Dense drizzle"},
        {56, "Light freezing drizzle"},
        {57, "Dense freezing drizzle"},
        {61, "Slight rain"},
        {63, "Moderate rain"},
        {65, "Heavy rain"},
        {66, "Light freezing rain"},
        {67, "Heavy freezing rain"},
        {71, "Slight snow fall"},
        {73, "Moderate snow fall"},
        {75, "Heavy snow fall"},
        {77, "Snow grains"},
        {80, "Slight rain showers"},
        {81, "Moderate rain showers"},
        {82, "Violent rain showers"},
        {85, "Slight snow showers"},
        {86, "Heavy snow showers"},
        {95, "Thunderstorm"},
        {96, "Thunderstorm with slight hail"},
        {99, "Thunderstorm with heavy hail"}
    };

  auto it = wmoDesc.find(weather_code);
  if(it != wmoDesc.end()){
    return it->second;
  }
  else{
    return "Unknown weather code";
  }
}


std::vector<std::string> parse_response(std::string_view res, const std::string lat, const std::string lon, const std::string req_time){
  nlohmann::json json_res;
  try{
    json_res = nlohmann::json::parse(res);
  }
  catch(const nlohmann::json::parse_error& e){
    return {"ERROR"};
  }


  std::vector<std::string> joined_array = {};

  if(req_time == "now"){
    const std::string time_str = json_res["current"]["time"].get<std::string>();
    const double temp = json_res["current"]["temperature_2m"].get<double>();
    const double prec = json_res["current"]["precipitation"].get<double>();
    const int weather_code = json_res["current"]["weather_code"].get<int>();
    const double wind_speed = json_res["current"]["wind_speed_10m"].get<double>();

    std::ostringstream oss;
    oss << formatTime(time_str) << '\t' << temp << "°C\t" << std::round(wind_speed * 10.0) / 10.0 << "m/s\t" << static_cast<int>(std::round(prec)) << "mm\t" << parse_weather_code(weather_code) << "\t";
    print_data(oss.str(), 9999);
    joined_array.push_back(oss.str());
  }
  else{
    const std::vector<std::string> time_array = json_res["hourly"]["time"].get<std::vector<std::string>>();
    const std::vector<double> temp_array = json_res["hourly"]["temperature_2m"].get<std::vector<double>>();
    const std::vector<double> prec_arr = json_res["hourly"]["precipitation"].get<std::vector<double>>();
    const std::vector<int> weather_code_arr = json_res["hourly"]["weather_code"].get<std::vector<int>>();
    const std::vector<double> wind_speed_arr = json_res["hourly"]["wind_speed_10m"].get<std::vector<double>>();


    if(time_array.size() != temp_array.size()){
      std::cerr << "ERR: Invalid Fetch";
    }

    for(size_t i = 0; i < time_array.size(); ++i) {
      std::ostringstream oss;
      oss << formatTime(time_array[i]) << '\t' << temp_array[i] << "°C\t" << std::round(wind_speed_arr[i] * 10.0) / 10.0 << "m/s\t" << static_cast<int>(std::round(prec_arr[i])) << "mm\t" << parse_weather_code(weather_code_arr[i]);
      print_data(oss.str(), i);
      joined_array.push_back(oss.str());
    }
  }


  return joined_array;
}

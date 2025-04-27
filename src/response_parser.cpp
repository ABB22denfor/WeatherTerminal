#include "../include/pch.h"

#include "../include/response_parser.h"
#include "../include/request_handler.h"
#include "../include/print.h"

#include <sstream>
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
    const int temp = json_res["current"]["temperature_2m"].get<int>();

    std::ostringstream oss;
    oss << formatTime(time_str) << '\t' << temp << "°C";
    print_data(oss.str(), 0);
    joined_array.push_back(oss.str());
  }
  else{
    const std::vector<std::string> time_array = json_res["hourly"]["time"].get<std::vector<std::string>>();
    const std::vector<int> temp_array = json_res["hourly"]["temperature_2m"].get<std::vector<int>>();

    if(time_array.size() != temp_array.size()){
      std::cerr << "ERR: Invalid Fetch";
    }

    for(size_t i = 0; i < time_array.size(); ++i) {
      std::ostringstream oss;
      oss << formatTime(time_array[i]) << '\t' << temp_array[i] << "°C";
      print_data(oss.str(), i);
      joined_array.push_back(oss.str());
    }
  }


  return joined_array;
}

//  Standard libraries:
#include <iostream>
#include <string>
#include <format>

//  Installed libraries:
#include <curl/curl.h>
#include <curl/easy.h>

// Other modules:
#include "response_parser.cpp"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output){
  size_t totalSize = size * nmemb;
  output->append((char*)contents, totalSize);
  return totalSize;
}

bool get_coords(const std::string city, double& lon, double& lat){
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
            lat = stod(jsonData[0]["lat"].get<std::string>());
            lon = stod(jsonData[0]["lon"].get<std::string>());
            return true;
        }
    } catch (...) {
        return false;
    }

    return false;
}

std::string request_data(std::string latitude, std::string longitude){
  CURL* curl;
  CURLcode res;

  std::string req_url = std::format("https://api.open-meteo.com/v1/forecast?latitude={}&longitude={}&hourly=temperature_2m&timezone=Europe%2FBerlin&forecast_days=1", latitude, longitude);
  
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

  return response_data;
}

int main(int argc, char* argv[]){
  std::string response_data;
  std::string city;

  for(int i = 1; i < argc; ++i){
    std::string arg = argv[i];

    if(arg == "-p" || arg == "--position"){
      if(i + 1 < argc){
        city = argv[++i];
      }
    }
  }
  
  double longitude, latitude;
  if(get_coords(city, longitude, latitude)){
    std::cout << city << std::endl;
  }
  response_data = request_data(std::to_string(latitude), std::to_string(longitude));

  parse_response(response_data);

  
  return 0;
}

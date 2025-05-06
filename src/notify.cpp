#include <cstdlib>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>
#include <fstream>

#include "../include/pch.h"
#include "../include/response_parser.h"
#include "../include/request_handler.h"

std::atomic<bool> keepRunning(true);

void signalHandler(int signum){
  keepRunning = false;
}


std::string get_data(std::string lat, std::string lon, std::string req_time){
  std::string response = request_data(lat, lon, req_time);
  std::vector<std::string> formatted_data = parse_response(response, lat, lon, req_time); 
  std::size_t split_pos = formatted_data[0].find('\t');
  std::string data = formatted_data[0].substr(split_pos, -1).erase(0,1);

  return data;
}


void send_notification(std::string delay, std::string lat, std::string lon){
  if(std::stoi(delay) >= 15){
    std::signal(SIGINT, signalHandler);
    while(keepRunning){
      std::string data = get_data(lat, lon, "now");
      std::string message = std::format("wsl-notify-send.exe --appId \"Weather CLI\" --category \"Weather Update\" \"{}\"", data);
      system(message.c_str());
      std::this_thread::sleep_for(std::chrono::minutes(std::stoi(delay)));
    }
  }
}

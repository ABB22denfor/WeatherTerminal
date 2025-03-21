# About
This is a Weather App implemented in the terminal. The user should provide requested city or coordinates aswell as the period for their forecast.
Default behaviour is to display current (hourly) weather in Västerås.


# Dependencies
These libraries are required to compile successfully:
- nlohmann/json
- libcurl

#### Installation:
```bash
sudo apt update
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev
```

# Usage
```bash
./WeatherApp -p <city|lon lat> -t <time_period>
```

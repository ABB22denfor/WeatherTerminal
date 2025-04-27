# Weather App CLI
This is a Weather App implemented in the terminal built with C++.

## Plan
The user should provide requested city or coordinates aswell as the period for their forecast.
Default behaviour is to display current (hourly) weather in the favorite location if city or time period is omitted. The data will be printed with ncurses,
showing weather and windspeed, with ASCII art. If user has multiple favorite locations the app will show a table that compares the weather in every location.
Notifications will also be implemented to show weather warnings, the user should also be able to customize how often they get the warnings, every hour or every three hours,
for example.


## Dependencies
These libraries are required to compile successfully:
- nlohmann/json
- libcurl
- ncurses

#### Installation:
```bash
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev libncurses5-dev libncursesw5-dev
```

## Usage

```bash
./WeatherApp [<options>]
```
#### Flags:
- -p | --position :  
    Position of which the app will base it's forecast. Permitted options are: "city" or "lon lat".  
*(default: "None")*
- -t | --time :  
    Time period of which the app will forecast. Permitted options are "now", "day", "week" or "month".  
*(default: "now")*
- -f | --favorite :  
    Favorite city or location that the app will save in database. No limit to amount of favorite cities.  
*(default: "None")*
- -v | --verbose :
    Enable verbose mode, where program outputs using ncurses instead of regular command-line.  
*(default: false)*
- -c | --clear:  
    Clears the cache of all saved data.  
*(default: "None")*

#### Example:
```bash
./WeatherApp -p 59.6162 16.5528 -t week -f Stockholm Vasteras Lidkoping
```

#### WARNING!
If options are omitted and the user hasn't set a favorite city this app might use geolocation to access the user's current location.   
  
##### Temp for compilation:
```bash
g++-13 -std=c++20 main.cpp request_handler.cpp response_parser.cpp -o main -lcurl
```

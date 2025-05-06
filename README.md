# Weather App CLI
This is a Weather App implemented in the terminal built with C++.

## About
The user should provide requested city or coordinates aswell as the period for their forecast.
Default behaviour is to display current (hourly) weather in the favorite location if city or time period is omitted. The data will be printed in the command line
showing weather, windspeed and precipitation. If the user has multiple favorite locations the app will show a table that compares the weather in every location.
Notifications will also be implemented to show current weather at a set interval determined by the user, minimum of 15 minutes.

## Dependencies
These libraries are required to compile successfully:
- nlohmann/json
- libcurl

#### Installation:
```bash
sudo apt install libcurl4-openssl-dev nlohmann-json3-dev
```

## Usage

```bash
./WeatherApp [<options>]
```
#### Flags:
- -p | --position :  
    Position of which the app will base it's forecast. Permitted options are: "city" or "lat lon".  
*(default: "None")*
- -t | --time :  
    Time period of which the app will forecast. Permitted options are "now", "day", "week" or "month".  
*(default: "now")*
- -f | --favorite :  
    Favorite city or location that the app will save in database. No limit to amount of favorite cities.  
*(default: "None")*
- -c | --clear:  
    Clears the cache of all saved data.  
*(default: "None")*
- -n | --notification:
    Sets interval for which to send notifications of current data. Minimum of 15 minutes. 
*(default: "None")*

#### Example:
```bash
./WeatherApp -p 59.6162 16.5528 -t week -f Stockholm Vasteras Lidkoping -n 15
```

#### WARNING!
If options are omitted and the user hasn't set a favorite city this app will use geolocation to access the user's current location.  

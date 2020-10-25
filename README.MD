# The Weather Lamp
An IoT project using NodeMCU. The lamp (5050 SMD RGB Led Strip) changes light pattern depending to the weather forecast.


## Getting Started

- Download the app:

```bash
  git clone git@github.com:ugobriasco/weather-lamp.git && cd ./weather-lamp
  cp ./src/nodemcu-lamp/config.h.template ./src/nodemcu-lamp/config.h
```
- Get an access token from https://openweathermap.org/api

- Edit the config file:

```bash
  vi ./src/nodemcu-lamp/config.h

  # Add API token to the HOSTNAME
  # Update SSID and PASSWORD
```

- Compile and flash the NodeMCU.
- Check the serial port under 115200 baud and get the IP assigned by wour WAN.
- Open your browser and GET that IP.


### Type of weather supported
*Weather conditions*

| Tag           | OpenWeather ID range      | Light behaviour                             | Example |
| ------------- | ------------------------- | ------------------------------------------- | ------- |
| Clear         | 800                       | Constant                                    | ☀️       |
| Partly-cloudy | 801, 802                  | Intermittent sinusoidal wave, low pitch     | 🌤       |
| Overcast      | 701->762, 803, 804        | Constant sinusoidal wave, low pitch         | ☁️️ 🌫     |
| Patchy-rain   | 500 -> 504                | Intermittent white noise burst              | 🌦       |
| light-snow    | 600, 611, 612, 615        | Intermittent white noise burst              | 🌦       |
| Rain          | 520 -> 531                | White noise                                 | 🌧       |
| Snow          | 511, 601, 602, 616 -> 622 | Intermittent white noise burst              | 🌨       |
| Storm         | 200 -> 321, 771, 781      | White noise convolute with hard noise burst | ⛈       |

source: https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2

*Temperatures*

| Tag       | Light behaviour | Example    |
| --------- | --------------- | --------- |
| very-hot  | Red             | > 30 °C   |
| hot       | Yellow          | 24-29 °C |
| mild      | Green           | 13-23 °C |
| cold      | Light Blue      | 5-12 °C |
| very cold | Deep Blue       | < 4 °C |


## Operating modes
This application can be used in different ways.

### Standalone mode
The lamp behaves autonomously, querying Open Weather API. This is the default mode, thus be sure to use the hostame and url defined in config.h.
In this mode there is a pretty basic http client to handle basic functions via web-interface

### Slave mode
The lamp queries a proxy server which returns the weather data. This allows the installation of multiple weather applications, centralising calls to the Open Weather API.

#### Proxy weather data
Change the Hostname in the config.h and ensure that whenever tha lamp GET it, it returns the following body:

```
{
    "lat": 52.51,
    "lon": 13.46,
    "timezone": "Europe/Berlin",
    "timezone_offset": 7200,
    "current": {...}
    "hourly": [  ## IMPORTANT!
        {},
        {}, ## The position in the houlry array should match config::FORECAST_IN_HOURS
        {
            "dt": 1603400400,
            "temp": 15.57,
            "feels_like": 12.58,
            "pressure": 1012,
            "humidity": 76,
            "dew_point": 11.36,
            "clouds": 78,
            "visibility": 10000,
            "wind_speed": 4.88,
            "wind_deg": 220,
            "weather": [ ## IMPORTANT!
                {
                    "id": 803,  ## IMPORTANT!
                    "main": "Clouds",
                    "description": "broken clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0.14
        },
        {

```

Furtrhermore, it is possible to control the lamp from the proxy (e.g. a custom app toglges the lamp via proxy server). The API of the lamp:

```
  GET /ledOn
  --> Turns the lamp on

  GET /ledOff
  --> Turns the lamp off

```

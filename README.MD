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


## Type of weather supported
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

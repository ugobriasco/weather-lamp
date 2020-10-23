# The Weather Lamp
An IoT project using NodeMCU. The lamp (5050 SMD RGB Led Strip) changes light pattern depending to the weather forecast.


## Getting Started

- Download the app:

```bash
  git clone git@github.com:ugobriasco/weather-lamp.git && cd ./weather-lamp
  cp ./src/nodemcu-lamp/config.h.template ./src/nodemcu-lamp/config.h
```

- Edit the config file:

```bash
  vi ./src/nodemcu-lamp/config.h
```

- Compile and flash the NodeMCU.
- Check the serial port under 115200 baud and get the IP assigned by wour WAN.
- Open your browser and GET that IP.


## Type of weather supported
https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2

*Weather conditions*

| Tag           | Light behaviour                             | Example |
| ------------- | ------------------------------------------- | ------- |
| clear         | Constant                                    | ☀️       |
| partly-cloudy | intermittent sinusoidal wave                | 🌤       |
| overcast      | constant sinusoidal wave                    | ☁️️ 🌫     |
| patchy-rain   | intermittent white noise burst              | 🌦       |
| rain          | white noise                                 | 🌧 - 🌨   |
| storm         | white noise convolute with hard noise burst | ⛈       |

*Temperatures*

| Tag       | Light behaviour | Example    |
| --------- | --------------- | --------- |
| very-hot  | Red             | > 30 °C   |
| hot       | Yellow          | 20-29 °C  |
| mild      | Green           | 10-20 °C  |
| cold      | Light Blue      | 0 - 10 °C |
| very cold | Deep Blue       | < 0 °C    |


## API Integration
https://openweathermap.org/api/one-call-api

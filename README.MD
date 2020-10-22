# The Weather Lamp





## Getting Started


If directly connecting to the ESP:
192.168.4.1/status


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

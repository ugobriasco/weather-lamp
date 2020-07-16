# The Weather Lamp





## Getting Started


If directly connecting to the ESP:
192.168.4.1/status


## Type of weather supported

*Weather conditions*

| Tag           | Light behaviour                             | Example   |
| ------------- | ------------------------------------------- | ----------|
| clear         | Constant                                    | ☀️         |
| partly-cloudy | intermittent sinusoidal wave                | 🌤        |
| overcast      | constant sinusoidal wave                    | ☁️️ 🌫      |
| patchy-rain   | intermittent white noise burst              | 🌦        |
| rain          | white noise                                 | 🌧 - 🌨   |
| storm         | white noise convolute with hard noise burst | ⛈        |

*Temperatures*

| Tag       | Light behaviour | Example   |
| --------- | --------------- | --------- |
| very-hot  | Red             | > 26  °C  |
| hot       | Yellow          | 21-26 °C  |
| mild      | Green           | 14-20 °C  |
| cold      | Light Blue      | 5-13  °C  |
| very cold | Deep Blue       | < 5   °C  |

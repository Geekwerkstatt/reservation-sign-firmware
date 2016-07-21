# Reservation sign firmware
The reservation sign is a custom built hardware, containing some leds, buttons and switches controlled by a teensy 3.1 micro controller.

# hardware
## Input
* 4 buttons (pin 9 to 12)
* 2 switches (pin 7 and 8)
* 1 potentiometer (pin A0, analog), values are between 0 and around 770.
* battery level indicator (pin A2, analog), values below 1000 indicates low battery.

## Output
* 4 RGB LEDs (WS2812B aka. NeoPixel), controlled via SPI bus on pin 6

# development environment
The current version of the firmware was developed using the following tools
* Arduino IDE 1.6.9 [Download-Website](https://www.arduino.cc/en/Main/Software)
* Teensyduino 1.29 [Download-Website](https://www.pjrc.com/teensy/td_download.html)
* Library: FastLED 3.1.1 (installed via the package manager integrated in the arduino IDE)
* Library: Encoder 1.4.1 (installed via the package manager integrated in the arduino IDE)

# other
The extend the battery life of the reservation sign the teensy is underclocked to 24 Mhz using the profile '24 Mhz optimized'.

# license
The firmware in this repository is licensed under the MIT License.

//
//  reservation-sign-firmware.ino
//  reservation-sign-firmware
//
//  Created by @1024kilobyte on 7/21/16.
//  Copyright (c) 2016 Geekwerkstatt. All rights reserved.
//

// helper library
#include <Encoder.h>

// library used controlling the rgb leds over spi
#include "FastLED.h"

// paramter definitions

// leds
#define LED_COUNT 4
#define LED_SPI_OUTPUT_PIN 6

CRGB leds[LED_COUNT];

// switches
#define SWITCH_1_INT_PIN 7
#define SWITCH_2_INT_PIN 8

// buttons
#define BUTTON_1_INT_PIN 9
#define BUTTON_2_INT_PIN 10
#define BUTTON_3_INT_PIN 11
#define BUTTON_4_INT_PIN 12

int currentColorIndex1 = 0;
int currentColorIndex2 = 0;
int currentColorIndex3 = 0;
int currentColorIndex4 = 0;

int delayDuration = 1000;
int lastChangedColorIndex = 0;

// encoder
Encoder knobLeft(4, 5);

// define a gradient to allow color fading
DEFINE_GRADIENT_PALETTE( colors_gp ) {
  0,   255,  0,  0,
 85,   0,  255,  0,
170,   0,    0,255,  
255,   255,  0,  0 };  

CRGBPalette16 colorGradient = colors_gp;

// interrupt service routines
void button1Pressed() {
  leds[1] = CRGB(0, 255, 0);
  leds[0] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 0, 0);
  
  FastLED.show();

  delay(2000);
}

void button2Pressed() {
  leds[1] = CRGB(0, 0, 0);
  leds[0] = CRGB(0, 255, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 0, 0);
  
  FastLED.show();

  delay(2000);
}

void button3Pressed() {
  leds[1] = CRGB(0, 0, 0);
  leds[0] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 255, 0);
  leds[3] = CRGB(0, 0, 0);
  
  FastLED.show();

  delay(2000);
}

void button4Pressed() {
  leds[1] = CRGB(0, 0, 0);
  leds[0] = CRGB(0, 0, 0);
  leds[2] = CRGB(0, 0, 0);
  leds[3] = CRGB(0, 255, 0);
  
  FastLED.show();

  delay(2000);
}

void switch1Changed() {
  int randomInt = random(12);

  int counter = 0;
  
  while (counter < randomInt) {
    leds[(counter+1) % 4] = CRGB(0, 0, 0);
    leds[(counter+2) % 4] = CRGB(0, 0, 0);
    leds[(counter+3) % 4] = CRGB(0, 0, 0);
    leds[counter % 4] = CRGB(0, 255, 0);
    FastLED.show();
    delay(200);
    counter++;
  }

  delay(2000);
}

void switch2Changed() {
  // not implemented yet
}

void batteryLow() {
  // Reset values
  FastLED.setBrightness(255);
  for (int clearCounter = 0; clearCounter < LED_COUNT; clearCounter++) leds[clearCounter] = CRGB::Black;
  FastLED.show();
  
  while(true) {
    for(int counter = 0; counter < LED_COUNT; counter++) {
        leds[counter] = CRGB::Red;
        FastLED.show();
        delay(1000);

        leds[counter] = CRGB::Black;
        FastLED.show();
        delay(1000);
    }
  }   
}

// arduino setup function
void setup() {
  // Serial.begin(38400);
  
  // setup leds
  FastLED.addLeds<WS2812B, LED_SPI_OUTPUT_PIN, GRB>(leds, LED_COUNT);
  FastLED.setDither( 0 );
  FastLED.setBrightness(255);
  // now clear the leds manually - FastLED.clear() is not always reliable
  for (int clearCounter = 0; clearCounter < LED_COUNT; clearCounter++) leds[clearCounter] = CRGB::Black;
  FastLED.show();

  // set pullup
  pinMode(BUTTON_1_INT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_INT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_INT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_INT_PIN, INPUT_PULLUP);

  pinMode(SWITCH_1_INT_PIN, INPUT_PULLUP);
  pinMode(SWITCH_2_INT_PIN, INPUT_PULLUP);

  // Battery level input
  pinMode(A2, INPUT_PULLUP);

  // POTI Input
  pinMode(A0, INPUT_PULLUP);

  // Encoder
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  // setup interrupts
  // attachInterrupt(SWITCH_1_INT_PIN, switch1Changed, CHANGE); //LOW,RISING, FALLING,CHANGE
  attachInterrupt(SWITCH_2_INT_PIN, switch2Changed, CHANGE);

  attachInterrupt(BUTTON_1_INT_PIN, button1Pressed, CHANGE);
  attachInterrupt(BUTTON_2_INT_PIN, button2Pressed, CHANGE);
  attachInterrupt(BUTTON_3_INT_PIN, button3Pressed, CHANGE);
  attachInterrupt(BUTTON_4_INT_PIN, button4Pressed, CHANGE);

  // setup startColors for light mode 0
  currentColorIndex1 = random8();
  currentColorIndex2 = random8();
  currentColorIndex3 = random8();
  currentColorIndex4 = random8();
}

// arduino run loop
void loop() {
  // light mode
  int lightMode = digitalRead(SWITCH_1_INT_PIN);

  // read value from poti
  int val = analogRead(A0);
  float relativeValue = (float)val / (float)760;
  float targetValue = (int)((float)relativeValue * (float)255);
  if (targetValue > 255) targetValue = 255;
  
  if (lightMode == 0) {
    int switchColorCount = (int)((float)targetValue / (float)64) + 1;

    while(switchColorCount > 0) {
      switch (lastChangedColorIndex) {
        case 0:
          currentColorIndex2 = random8();
          leds[1] = ColorFromPalette((CRGBPalette16)colors_gp, currentColorIndex2);
          break;
        case 1:
          currentColorIndex3 = random8();
          leds[2] = ColorFromPalette((CRGBPalette16)colors_gp, currentColorIndex3);
          break;
        case 2:
          currentColorIndex4 = random8();
          leds[3] = ColorFromPalette((CRGBPalette16)colors_gp, currentColorIndex4);
          break;
        case 3:
          currentColorIndex1 = random8();
          leds[0] = ColorFromPalette((CRGBPalette16)colors_gp, currentColorIndex1);
          break;
      }

      lastChangedColorIndex = ((lastChangedColorIndex + 1) % 4);
      switchColorCount--;
    }

    // calculate speed of color change / delay duration
    long encoderValue = knobLeft.read();
    if (encoderValue < -180) {
      encoderValue = -180;
      knobLeft.write(-180);
    } else if (encoderValue > 200) {
      encoderValue = 200;
      knobLeft.write(200);
    }
      
    int actualDuration = delayDuration + ((int)encoderValue * 5);
    
    delay(actualDuration);
  } else {
    // set brightness based on value from potentiometer
    FastLED.setBrightness(targetValue);
  
    // read encoder value - set color
    long encoderValue = knobLeft.read();
    uint8_t colorindex = (uint8_t)((int)((float)encoderValue / (float)2) % 255);
  
    CRGB currentColor = ColorFromPalette((CRGBPalette16)colors_gp, colorindex);
  
    // set randow colors for all leds
    for (int ledCounter = 0; ledCounter < LED_COUNT; ledCounter++)
    {
      leds[ledCounter] = currentColor;
    }
  }

  // finally show new color
  FastLED.show();

  // Check battery state
  int batteryStateValue = analogRead(A2); 

  if (batteryStateValue < 1000) {
    batteryLow();
  }
}


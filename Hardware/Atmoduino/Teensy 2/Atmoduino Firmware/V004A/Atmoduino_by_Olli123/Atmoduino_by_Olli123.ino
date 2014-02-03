//#define FORCE_SOFTWARE_SPI
//#define FORCE_SOFTWARE_PINS
#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS 50

// Data pin that led data will be written out over
#define DATA_PIN 2

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 13

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

void setup(){
  
  //sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
   
  //Change this to match your led strip
  
      // Uncomment one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastSPI_LED2.addLeds<WS2811, DATA_PIN, GRB>(leds+18, NUM_LEDS/3);
      // FastLED.addLeds<WS2811, 8, RGB>(leds + 225, NUM_LEDS/4);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);

       FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);

      //FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  
  Serial.begin(230400);
}

int readByte(){
  while(Serial.available()==0){
  }
  return Serial.read();
}

void loop(){
  if(readByte() == 0xFF){
    if(readByte() == 0x00){
      if(readByte() == 0x00){
        int channels = readByte();
        for(int channel=0; channel<channels; channel++){
          leds[channel].r = readByte();
          leds[channel].b = readByte();
          leds[channel].g = readByte();
        }
        FastSPI_LED.show();
      }
    }
  }
}

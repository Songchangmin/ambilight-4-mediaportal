// Uncomment this line if you have any interrupts that are changing pins - this causes the library to be a little bit more cautious
// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1

// Uncomment this line to force always using software, instead of hardware, SPI (why?)
// #define FORCE_SOFTWARE_SPI 1

// Uncomment this line if you want to talk to DMX controllers
// #define FASTSPI_USE_DMX_SIMPLE 1

#include "FastLED.h"

//Set the number of leds in the strip.
#define NUM_LEDS 50


// Data pin that led data will be written out over
#define DATA_PIN 2

// Clock pin only needed for SPI based chipsets when not using hardware SPI
//#define CLOCK_PIN 8

// Sometimes chipsets wire in a backwards sort of way
struct CRGB leds[NUM_LEDS]; 

// If no serial data is received for a while, the LEDs are shut off
// automatically.  This avoids the annoying "stuck pixel" look when
// quitting LED display programs on the host computer.
//static const unsigned long serialTimeout = 8000; // 15 seconds 15000

void setup() 
{
  Serial.begin(115200);	
  	// sanity check delay - allows reprogramming if accidently blowing power w/leds
   	//delay(2000);

   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
       //LEDS.setBrightness(64);

      // Uncomment one of the following lines for your leds arrangement.
      // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
      //FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);

      FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
      
      // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
      
      // Put ws2801 strip on the hardware SPI pins with a BGR ordering of rgb and limited to a 1Mhz data rate
      // LEDS.addLeds<WS2801, DATA_PIN, CLOCK_PIN, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);

      // LEDS.addLeds<LPD8806, DATA_PIN, CLOCK_PIN>(leds, NUM_LEDS);
      // LEDS.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
      // LEDS.addLeds<LPD8806, BGR>(leds, NUM_LEDS);

WhiteLeds();
//delay(1); // One millisecond pause = latch
//FadeinFadeOut();
//delay(1); // One millisecond pause = latch
//clearLeds();

}


/*------------------  TESTING LED when board start only  -------------------- */


void WhiteLeds()
{   // Move a single white led 
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) {
      // Turn our current led on to white, then show the leds
      leds[whiteLed] = CRGB::White;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(8);

      // Turn our current led back to black for the next loop around
      leds[whiteLed] = CRGB::Black;
   }
 }

 // Fade in/fade out
void FadeinFadeOut()
 {
  for(int i = 0; i < 3; i++)  
  {
        for(int i = 0; i < NUM_LEDS; i++) 
   {
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));    
    for(int k = 0; k < 256; k++) 
    {
      {
        switch(i) 
        {
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastLED.show();
      delay(1);
    }
    for(int k = 255; k >= 0; k--) 
    {
      for(int i = 0; i < NUM_LEDS; i++ ) 
      {
        switch(i) 
        { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastLED.show();
      //delay(1);
    }
   }
  }
 }

int readByte()
    {
  while(Serial.available()==0) 
  {
       // clearLeds();
  }
  FastLED.show();
  return Serial.read();
    }

void clearLeds()
{
  for(int tmpChannel=0; tmpChannel<NUM_LEDS; tmpChannel++)
  {
    leds[tmpChannel].r = 0;
    leds[tmpChannel].g = 0;
    leds[tmpChannel].b = 0;  
  }
  FastLED.show();
}

void loop()
{
  if(readByte() == 0xFF)
  {
    if(readByte() == 0x00)
    {
      if(readByte() == 0x00)
      {
        int channels = readByte();
        for(int channel=0; channel<channels; channel++)
        {
          leds[channel].r = readByte(); 
          leds[channel].g = readByte(); // was b old
          leds[channel].b = readByte(); // was g old
        }
        FastLED.show();
      }
    }
  }
}


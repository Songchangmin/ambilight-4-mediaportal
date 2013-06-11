#include "FastSPI_LED2.h"

//Set the number of leds in the strip.
#define NUM_LEDS 150

// Sometimes chipsets wire in a backwards sort of way
//struct CRGB { unsigned char b;   unsigned char r;   unsigned char g; };
//struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
//struct CRGB { unsigned char r;   unsigned char b;   unsigned char g; };
struct CRGB leds[NUM_LEDS];

// If no serial data is received for a while, the LEDs are shut off
// automatically.  This avoids the annoying "stuck pixel" look when
// quitting LED display programs on the host computer.
//static const unsigned long serialTimeout = 8000; // 15 seconds 15000

void setup() 
{
  //Serial.begin(9600);	
  
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
   	delay(2000);

   	// For safety (to prevent too high of a power draw), the test case defaults to
   	// setting brightness to 25% brightness
   	//LEDS.setBrightness(64);

   	// LEDS.addLeds<WS2811, 13>(leds, NUM_LEDS);
   	// LEDS.addLeds<TM1809, 13>(leds, NUM_LEDS);
   	// LEDS.addLeds<UCS1903, 13>(leds, NUM_LEDS);
   	// LEDS.addLeds<TM1803, 13>(leds, NUM_LEDS);

   	//LEDS.addLeds<LPD8806>(leds, NUM_LEDS)->clearLeds(300);
	 LEDS.addLeds<WS2801>(leds, NUM_LEDS);
   	// LEDS.addLeds<SM16716>(leds, NUM_LEDS);

   	// LEDS.addLeds<WS2811, 11>(leds, NUM_LEDS);

	// Put ws2801 strip on the hardware SPI pins with a BGR ordering of rgb and limited to a 1Mhz data rate
	// LEDS.addLeds<WS2801, 11, 13, BGR, DATA_RATE_MHZ(1)>(leds, NUM_LEDS);

   	// LEDS.addLeds<LPD8806, 10, 11>(leds, NUM_LEDS);
   	// LEDS.addLeds<WS2811, 13, BRG>(leds, NUM_LEDS);
   	// LEDS.addLeds<LPD8806, BGR>(leds, NUM_LEDS);
    
  //Change datarate to match your led strip as well
  //FastSPI_LED.setDataRate(2); // Data Rate set to (2) for Adafruit ID 322 12mm bullet because is 5V led strip

  //If non-default SPI pins have been used change this.
  //FastSPI_LED.setPin(PIN,2,1); // (datapin, datapin, clockpin)

}
 // TESTING LED when board start only
 // Fade in/fade out
 void testleds() {
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
      LEDS.show();
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
      LEDS.show();
      delay(1);
    }
   }
  }

testleds();
clearLeds();
delay(1); // One millisecond pause = latch
  
}
int readByte()
{
  while(Serial.available()==0)
  {
    clearLeds();
  }
  LEDS.show();
  return Serial.read();
}

void clearLeds()
{
  for(int tmpChannel=0; tmpChannel<NUM_LEDS; tmpChannel++)
  {
    leds[tmpChannel].r = 0;
    leds[tmpChannel].b = 0;
    leds[tmpChannel].g = 0;  
  };
  LEDS.show();
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
          leds[channel].b = readByte(); // try with .g
          leds[channel].g = readByte(); // try with .b
        }
        LEDS.show();
      }
    }
  }
  else
  {
    clearLeds(); 
    //FastSPI_LED.stop(); // descativer a l'origine
  }
}


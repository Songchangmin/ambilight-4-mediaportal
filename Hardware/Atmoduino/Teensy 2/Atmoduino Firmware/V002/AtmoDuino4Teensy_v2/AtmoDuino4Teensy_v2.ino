#include <FastSPI_LED.h>

//Set the number of leds in the strip.
#define NUM_LEDS 150

// Sometimes chipsets wire in a backwards sort of way
//struct CRGB { unsigned char b;   unsigned char r;   unsigned char g; };
//struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB { unsigned char r;   unsigned char b;   unsigned char g; };
struct CRGB *leds;

#define PIN 2

// If no serial data is received for a while, the LEDs are shut off
// automatically.  This avoids the annoying "stuck pixel" look when
// quitting LED display programs on the host computer.
//static const unsigned long serialTimeout = 8000; // 15 seconds 15000

void setup()
{
  

  Serial.begin(115200); // Teensy/32u4 disregards baud rate; is OK!
  FastSPI_LED.setLeds(NUM_LEDS);
  //Change this to match your led strip
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_SM16716);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_HL1606);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_595);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);
  
  //Change datarate to match your led strip as well
  FastSPI_LED.setDataRate(2); // Data Rate set to (2) for Adafruit ID 322 12mm bullet because is 5V led strip

  //If non-default SPI pins have been used change this.
  //FastSPI_LED.setPin(PIN,2,1); // (datapin, datapin, clockpin)

  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData();
 
 // TESTING LED
 // Fade in/fade out
  for(int j = 0; j < 3; j++ ) 
  { 
    memset(leds, 0, NUM_LEDS * 3);
    
    for(int k = 0; k < 256; k++) 
    { 
      for(int i = 0; i < NUM_LEDS; i++ ) 
      {
        switch(j) 
        { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(1);
    }
    for(int k = 255; k >= 0; k--) 
    { 
      for(int i = 0; i < NUM_LEDS; i++ ) 
      {
        switch(j) 
        { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(1);
    }

    }
  

clearLeds();
delay(1); // One millisecond pause = latch
  }

int readByte(){
  while(Serial.available()==0){
    FastSPI_LED.stop();
  }
  FastSPI_LED.start();
  return Serial.read();
}

void clearLeds(){
  for(int tmpChannel=0; tmpChannel<NUM_LEDS; tmpChannel++){
    leds[tmpChannel].r = 0;
    leds[tmpChannel].b = 0;
    leds[tmpChannel].g = 0;  
  };
  FastSPI_LED.show();
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
  else
  {
    clearLeds(); // desactiver a lorigine
    FastSPI_LED.stop(); // descativer a l'origine
  }
}


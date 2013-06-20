#include <FastSPI_LED.h>

//Set the number of leds in the strip.
#define NUM_LEDS 120

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
  unsigned char b; 
  unsigned char r; 
  unsigned char g; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 4
void setup(){
  Serial.begin(115200);
  FastSPI_LED.setLeds(NUM_LEDS);

  //Change this to match your led strip
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);
  
  //Change datarate to match your led strip as well
  FastSPI_LED.setDataRate(0);

  //If non-default SPI pins have been used change this.
  FastSPI_LED.setPin(PIN,11,13);

  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData();
  clearLeds();
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
    //clearLeds();
    //FastSPI_LED.stop();
  }
}


#include <FastSPI_LED.h>

//number of LEDs in the chain
#define NumLEDs 50

// re-order r g b if you chip wiring is weird
struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 2
  
void setup()
{
  Serial.begin(115200);
  FastSPI_LED.setLeds(NumLEDs);
  //Set chipset type 
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

  //For WS2801 set datarate // play with this if flickering
  FastSPI_LED.setDataRate(2);
  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

int incomingatmo[256];// increase this if you have over 85 LEDS or implement a buffering mechanism.

int gammatable[]={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,3,
3,3,4,4,4,5,5,6,6,6,7,7,8,8,9,10,10,11,12,12,13,14,14,15,16,17,18,19,
19,20,21,22,23,24,26,27,28,29,30,31,33,34,35,36,38,39,41,42,44,45,47,48,
50,52,53,55,57,58,60,62,64,66,68,70,72,74,76,78,80,82,85,87,89,92,94,96,99,
101,104,106,109,112,114,117,120,123,125,128,131,134,137,140,143,146,149,153,
156,159,162,166,169,172,176,179,183,187,190,194,198,201,205,209,213,217,221,
225,229,233,237,241,246,250,254,258,263,267,272,276,281,286,290,295,300,305,
310,314,319,324,329,335,340,345,350,355,361,366,372,377,383,388,394,400,405,
411,417,423,429,435,441,447,453,459,465,472,478,484,491,497,504,510,517,524,
530,537,544,551,558,565,572,579,586,593,601,608,615,623,630,638,645,653,661,
668,676,684,692,700,708,716,724,732,740,749,757,765,774,782,791,800,808,817,
826,835,844,853,862,871,880,889,898,908,917,926,936,945,955,965,974,984,994,
1004,1014,1023};


void outputpwm(int curchannel)
{
  // start at incomingatmo[1] because we left the "Channel Count" byte in the array
  int red   = gammatable[incomingatmo[(curchannel*3)+1]]; 
  int green = gammatable[incomingatmo[(curchannel*3)+2]];  
  int blue  = gammatable[incomingatmo[(curchannel*3)+3]];
      
  leds[curchannel].r = red;
  leds[curchannel].g = green;  
  leds[curchannel].b = blue;  
 }

int buffer=0;

void loop()
{
    if(Serial.available())
    {
      buffer = Serial.read();
      if(buffer==0xff)
      {
      while(!Serial.available())
      {
         //Wait for data 
      }
      buffer=Serial.read();
      if(buffer==0x00)
      {
        while(!Serial.available())
        { 
          //wait for data
        }
        buffer=Serial.read();
        if(buffer==0x00)
        {
           for(int i=0;i<=NumLEDs*3;i++)
           {
              while(!Serial.available())
              { 
                //wait for data
              }
              incomingatmo[i]=Serial.read();   
           }
           for(int channel=0;channel<NumLEDs;channel++)
           {           
              outputpwm(channel);
           }
           FastSPI_LED.show();
         }
       }    
     }
   }
   else
   {
     FastSPI_LED.show();
     delay(5);
   }
}

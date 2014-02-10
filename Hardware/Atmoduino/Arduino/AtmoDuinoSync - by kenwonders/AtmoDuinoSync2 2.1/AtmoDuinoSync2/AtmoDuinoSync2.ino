/*

 AtmoDuino custom driver
 This is based on original AtmoDuino source, but I wanted to add a delay to sync with my TV
 ****************************
 
 KF 2014 
 2.1 updated to FastLED
 2.0 removed channel check or usage. it is not used in the same way between clients
 1.5 branched into v2 to attempt optimisations
 1.4 added millis rollover detection
 1.3 fixed serial clear line detection
 1.2 added serial clear line detection
 1.1 added documentation,
 1.0 fixed delay, cleaned code
 0.9 works but does not delay
 0.8 first change from multi array based setup, lights flicker strangely
 */

// includes
#include <FastLED.h>
#include <QueueArray.h>

/////////////////////////////////////////////////////////////////////
// settings - change these to suit
const int NUM_LEDS = 50; // number of LEDs in set
const unsigned long SERIAL_TIMEOUT = 60000; // milliseconds delay after last contact to clear LEDs
const unsigned long STREAM_DELAY = 150; // milliseconds delay in stream to LEDs
/*
 see the FastSPI_LED2 library for details about how to change the addLeds call below to suit your LED set
 */
/////////////////////////////////////////////////////////////////////

// variables
//main led set
struct CRGB leds[NUM_LEDS] = {
  0};
// optimised structure that avoids everything but rgb;
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b; 
};
//structure to hold a stream and a timestamp
struct LEDdata
{
  struct RGB ledarr[NUM_LEDS];
  unsigned long timestamp;
};
//the holding ground for LED data
QueueArray <LEDdata> LEDqueue;
//the point where the data no longer came
unsigned long  serialLineClearStarted = 0;
//whether the line is clear right now
boolean serialLineClear = true;

// setup
void setup(){
  //open serial port
  Serial.begin(115200);
  //add LEDs to FastSPI (find the appropriate driver for you)
  FastLED.addLeds<WS2801, 11, 13, RGB, 20>(leds, NUM_LEDS);
  //blank out the LEDs
  setupLEDs();
}

// main loop
void loop(){
  //try to read a stream to be stored in the queue
  readData();
  //see if there is any data to show yet
  showLEDs(); 
}

// shows white LEDs for 500ms then blanks out ready to go
void setupLEDs()
{
  memset(leds,255, sizeof(leds));
  FastSPI_LED.show();
  delay(500);  
  memset(leds,0, sizeof(leds));
  FastSPI_LED.show();
}

void clearLEDs() {
  memset(leds,0, sizeof(leds));
  FastSPI_LED.show();
}

// read a stream of data
void readData() {
  // try to load a stream of data 
  if((readByte() == 0xFF) && (readByte() == 0x00) && (readByte() == 0x00) && loadLEDs()){
    //we have complete data
    updateSerialStatus(true);
  }
  else
  {
    updateSerialStatus(false);      
  }
}

// show the LED set
void showLEDs() {
  //ensure there is data in the queue and that the length of time in the STREAM_DELAY value has passed since the LED data was set
  //also ensure millis hasn't reset back to zero
  if(!LEDqueue.isEmpty()) {
    unsigned long LEDpeek = LEDqueue.peek().timestamp;
    unsigned long now = millis();
    checkMillis(LEDpeek, now);
    if(LEDpeek + STREAM_DELAY < now) {
      //pop from queue, load into leds and show
      LEDdata templed = LEDqueue.pop();
      for(int channel=0; channel<NUM_LEDS; channel++) {
        leds[channel].r = templed.ledarr[channel].r;
        leds[channel].g = templed.ledarr[channel].g;
        leds[channel].b = templed.ledarr[channel].b;    
      }
      FastSPI_LED.show();     
    }
  }
}

// check that millis has not reset using two timestamps
void checkMillis(unsigned long olderTime, unsigned long now) {
  if (olderTime > now) {
    //clear queue
    if (!LEDqueue.isEmpty()) {
      while(!LEDqueue.isEmpty()) {
        LEDqueue.pop(); 
      }
    }
    //reset serial timeout by pretending we just had data
    serialLineClear = false;  
  }
}


// manage serial status
void updateSerialStatus(boolean haveData) {
  switch (haveData) {
  case true:
    serialLineClear = false;
    break;
  case false:
    //set time if the last stream was successful
    unsigned long now = millis();
    checkMillis(serialLineClearStarted, now);
    if (!serialLineClear) {
      serialLineClear = true;
      serialLineClearStarted = now;
    }
    //check whether we've had a clear line for too long
    if (serialLineClear && (serialLineClearStarted + SERIAL_TIMEOUT< now))
    {
      //clear bulbs
      clearLEDs();
      //reset serial timeout by pretending we just had data
      serialLineClear = false;    
    }    
    break;
  } 
}

// load serial data into current LED set in array
boolean loadLEDs()  {
  //create a temp structure
  LEDdata templed;
  //find out how many channels it thinks we have
  int channels = readByte();
  for(int channel=0; channel<NUM_LEDS; channel++){
    for(int colour=0; colour<3; colour++) {
      int data = readByte();
      if (data > -2) { 
        switch (colour) {
        case 0:
          templed.ledarr[channel].r = (uint8_t)data;
          break;
        case 1:
          templed.ledarr[channel].g = (uint8_t)data;
          break;
        case 2:
          templed.ledarr[channel].b = (uint8_t)data;
          break;
        }
      }
      else
      {
        return false;
      }
    }
  }
  //set the time we received this data
  templed.timestamp = millis();
  //load onto queue
  LEDqueue.push(templed);
  return true;
}

// read a byte
int readByte(){
  //  will retry 5 times with a delay to ensure any current stream from host is loaded in full
  //  returns -2 if failed, which should trigger a LED load fail
  int retries = 0;
  while (retries < 5) {
    if(Serial.available() > 0) {
      int returnData = Serial.read();
      return returnData;        
    }
    else
    {
      delay(1);
      retries = retries + 1;
    }
  }
  return -2;
}




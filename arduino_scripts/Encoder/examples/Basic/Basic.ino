/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <OctoWS2811.h>


// set up the LED stuff...
const int ledsPerStrip = 10;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

struct int24{
    unsigned int data : 24;
};

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
  leds.show();
}


// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(0, 1);
//   avoid using pins with LEDs attached

long oldPosition  = -999;

#define PINK   0xFF1088
#define BLACK  0x000000

int ledloop[28] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 19, 29, 39, 49, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 40, 30, 20, 10
};
int LOOP_LENGTH = 28;
int old_position = 28;
int color_position = 5;

void loop() {
  
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {

    old_position = color_position;
    color_position++;
    
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
  
  if(color_position >= LOOP_LENGTH) {
    color_position = 0;
  }

  leds.setPixel(color_position, PINK);
  leds.setPixel(old_position, BLACK);

  leds.show();
  //int microsec = 100000; // 2000000 / leds.numPixels();  // change them all in 2 seconds
  //delayMicroseconds(microsec);
}


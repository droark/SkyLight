
#include <Encoder.h>
#include "utility/HSBconvert/HSBColor.h"
#include <OctoWS2811.h>

Encoder knob(0, 1);

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

/*
Do this ten times...
- grab a random value between 1 and 360
- generate a color given a hue of that number
- give us the 24 bit representation of that color.
*/



#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define BLACK  0x000000
int NOLED = 255;

int ledloop[28] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

int LOOP_LENGTH = 28;

int old_color = 28;
int new_color = 0;

long old_knob = 0;
int hue = 0;
int toRGB[3];

int convertArrayTo32b(int *array){
  int ret = 0;
  for(int i=0;i<3;i++){
    ret += (array[i] << 8*i);
  }
  return ret;
}

void loop() {

/*
  long new_knob = knob.read();
  if (new_knob != old_knob) {
    old_color = new_color;
    if (new_knob > old_knob) {
      // turn clockwise
      new_color += 1;      
    } else {
      // turn counterclockwise
      new_color -= 1;      
    }
    old_knob = new_knob;
    if (new_color > LOOP_LENGTH) {
      new_color = 0;
    }
    if (new_color < 0) {
      new_color = LOOP_LENGTH-1;
    }
  }
*/
  hue += 1;
  hue = hue % 360;

  H2R_HSBtoRGB(hue, 99, 99, toRGB);
  int color = convertArrayTo32b(toRGB);
  

  leds.setPixel(0, color);
  
  //leds.setPixel(ledloop[old_color], BLACK);

  leds.show();
  int microsec = 10000; // 2000000 / leds.numPixels();  // change them all in 2 seconds
  delayMicroseconds(microsec);
}

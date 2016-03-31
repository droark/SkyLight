

#include <OctoWS2811.h>

const int ledsPerStrip = 300;

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

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define BLACK  0x000000


int counterRed = 4;
int counterGreen = 3;
int counterBlue = 2;
int counterBlack = 1;

int offset = 0;

void loop() {
  counterRed++;
  counterGreen++;
  counterBlue++;
  counterBlack++;

  for(int i=0; i<8; i++) {
    offset = i * 300;
    leds.setPixel(offset + counterRed, RED);
    leds.setPixel(offset + counterGreen, GREEN);
    leds.setPixel(offset + counterBlue, BLUE);
    leds.setPixel(offset + counterBlack, BLACK);
  }
  leds.show();
  int microsec = 100000; // 2000000 / leds.numPixels();  // change them all in 2 seconds
  delayMicroseconds(microsec);
}


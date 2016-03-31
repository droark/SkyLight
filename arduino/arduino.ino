/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/

#include <OctoWS2811.h>

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

// here's the map of how the LEDs connect to each other.
int ledmap[60][3] = {
  {1, 10, NOLED},  // 0
  {0, 2, NOLED},   // 1
  {1, 3, NOLED},   // 2
  {2, 4, NOLED},   // 3
  {3, 5, NOLED},   // 4
  {4, 6, NOLED},   // 5
  {5, 7, NOLED},   // 6
  {6, 8, NOLED},   // 7
  {7, 9, NOLED},   // 8
  {19, 8, NOLED},  // 9
  {0, 11, NOLED},  // 10
  {10, 12, NOLED}, // 11
  {11, 13, NOLED}, // 12
  {12, 14, NOLED}, // 13
  {13, 15, NOLED}, // 14
  {14, 16, NOLED}, // 15
  {15, 17, 26}, // 16
  {16, 18, NOLED}, // 17
  {17, 19, NOLED}, // 18
  {9, 18, NOLED}, // 19
  {30, 21, NOLED}, // 20
  {20, 22, NOLED}, // 21
  {21, 23, NOLED}, // 22
  {22, 24, NOLED}, // 23
  {23, 25, NOLED}, // 24
  {26, 24, NOLED}, // 25
  {25, 27, 16}, // 26
  {26, 28, NOLED}, // 27
  {27, 29, NOLED}, // 28
  {39, 28, NOLED}, // 29
  {20, 31, 40}, // 30
  {30, 32, NOLED}, // 31
  {33, 31, NOLED}, // 32
  {32, 34, NOLED}, // 33
  {35, 33, NOLED}, // 34
  {34, 36, NOLED}, // 35
  {35, 37, NOLED}, // 36
  {36, 38, NOLED}, // 37
  {37, 39, NOLED}, // 38
  {49, 38, NOLED}, // 39
  {41, 50, 30}, // 40
  {40, 42, NOLED}, // 41
  {41, 43, NOLED}, // 42
  {42, 44, NOLED}, // 43
  {43, 54, NOLED}, // 44
  {46, 55, NOLED}, // 45
  {47, 45, NOLED}, // 46
  {46, 48, NOLED}, // 47
  {47, 49, NOLED}, // 48
  {48, 59, 39}, // 49
  {40, 51, NOLED}, // 50
  {50, 52, NOLED}, // 51
  {51, 53, NOLED}, // 52
  {52, 54, NOLED}, // 53
  {53, 44, NOLED}, // 54
  {56, 45, NOLED}, // 55
  {57, 55, NOLED}, // 56
  {56, 58, NOLED}, // 57
  {57, 59, NOLED}, // 58
  {58, 49, NOLED}  // 59
};

int redslug[2] = {rand() % 60, 1}; // our little walker guy.
int greenslug[2] = {rand() % 60, 1}; // our little walker guy.
int blueslug[2] = {rand() % 60, 1}; // our little walker guy.
int yellowslug[2] = {rand() % 60, 1}; // our little walker guy.
int pinkslug[2] = {rand() % 60, 1}; // our little walker guy.
int orangeslug[2] = {rand() % 60, 1}; // our little walker guy.
int whiteslug[2] = {rand() % 60, 1}; // our little walker guy.

void loop() {

  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;
  crawl(redslug, RED);
  crawl(greenslug, GREEN);
  crawl(blueslug, BLUE);
  crawl(yellowslug, YELLOW);
  crawl(pinkslug, PINK);
  crawl(orangeslug, ORANGE);
  crawl(whiteslug, WHITE);
  leds.show();
  int microsec = 100000; // 2000000 / leds.numPixels();  // change them all in 2 seconds
  delayMicroseconds(microsec);
}

int pickFromOptions(int A, int B, int C, int Last) {
  if(A != NOLED && A != Last) {
    return A;
  }
  if(B != NOLED && B != Last) {
  return B;
  }
  return C;
}

void crawl(int slug[2], int color) {
    int present_location = slug[0];
    int last_location = slug[1];
    int optionA = ledmap[present_location][0];
    int optionB = ledmap[present_location][1];
    int optionC = ledmap[present_location][2];

    int random_direction = rand() % 4;
    if(random_direction == 0) {
      slug[0] = pickFromOptions(optionA, optionB, optionC, last_location);
    }
    if(random_direction == 1) {
      slug[0] = pickFromOptions(optionC, optionB, optionA, last_location);
    }
    if(random_direction == 2) {
      slug[0] = pickFromOptions(optionB, optionA, optionC, last_location);
    }
    if(random_direction == 3) {
      slug[0] = pickFromOptions(optionB, optionC, optionA, last_location);
    }

    slug[1] = present_location;

    // lights up the pixels
    leds.setPixel(slug[0], color);
    leds.setPixel(slug[1], BLACK);
}

void colorWipe(int color, int wait)
{
  leds.setPixel(35, color);
  leds.show();
  delayMicroseconds(wait);
}

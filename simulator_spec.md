# SkyLight Code / Simulator
This document is a description of the SkyLight API. In particular, it's meant to assist those developing simulators. The basic idea is to use WebSockets to have a browser interact with the compiled C code in order to simulate the final product.

## Actual code
When in doubt, look at the *arduino* directory, which has the C code to run. *arduino/arduino.ino* currently has the guts of the system.

## Important LED API calls
Here's a slightly modified [excerpt from the "Basic Usage" section of the page on the octoWS2811 library page](https://www.pjrc.com/teensy/td_libs_OctoWS2811.html). If in doubt, [consult the OctoWS2811 source](https://github.com/PaulStoffregen/OctoWS2811).

```
// Create OctoWS2811 object. You can only create a single object, but it must be
// created with these parameters:
//
// ledsPerStrip: # of LEDs connected to each pin, or max # if different on each pin.
// displayMemory: Mem used for display data. Use an int array of ledsPerStrip*6.
// drawingMemory: Memory used for drawing operations. Use an int array of ledsPerStrip*6
//                or NULL to perform all drawing directly to the display memory.
// config: Config the WS2811 speed and LED color order. Options are WS2811_RGB,
//         WS2811_RBG, WS2811_GRB, WS2811_GBR, WS2811_800kHz, WS2811_400kHz. 
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Initialize the library. This must be called before using setPixel or show.
leds.begin();

// Set a pixel. Red, green and blue are 0 to 255 (uint8_t values each).
leds.setPixel(num, red, green, blue);

// Set a pixel. Color is a 24 bit color in RGB order (the same as HTML).
// 1st arg: index of the pixel to be modded (uint32_t)
// 2nd arg: Color of the pixel (int)
leds.setPixel(num, color);

// Start an update of the LEDs. This function returns within 2 microseconds.
// The display update continues, taking 30 microseconds for for each LED, plus
// 50 microseconds to reset the WS2811. If called while a previous update is
// running, this function waits for previous update to complete and then starts
// a new update.
leds.show();

// Check if a previous show() is still running. Returns true if the WS2811 LEDs
// are busy being updated, or false if no update is in progress.
leds.busy();

// Read a pixel's color. The return is a 24 bit color number. 
leds.getPixel(num);
```

## Miscellaneous simulator notes

Here are some extra notes regarding the LED code and what does and does not need to be implemented.

```
// These lines define the # of LEDs (array length) we use for the display. In
// this example, it's 180 pixels. DO NOT ALLOW THE ARRAY TO BE DYNAMIC.
DMAMEM int displayMemory[180];
int drawingMemory[180];

// These calls should be NOOPs in the simulator.
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
leds.begin();

// Tell a pixel what color it'll eventually be. The pixel won't light up just yet.
// When you call setPixel, don't send the data to the display yet!
#define PINK 0xFF1088
leds.setPixel(138, PINK);

// This sends all of the data you've been collecting in setPixels to the display.
leds.show();
```

## Warning
As always, everything is in progress, and you run this code [at your own risk](https://www.youtube.com/watch?v=GCCZvpEMTGM). Don't run this code unless you want your dog to get eaten by a [woman who is also a mongoose](https://www.youtube.com/watch?v=pCv7CtOafL0).

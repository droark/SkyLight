#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define USE_OCTOWS2811


#include <OctoWS2811.h>
#include <FastLED.h>
#include <IntervalTimer.h>
#include <Encoder.h>
#include "ArtMap.h"


/***************** Settings ****************/
#define NUM_STRIPS 7
#define NUM_CTRLS 1
#define FRAME_TICK_HZ 60  // How many frames per second we update the LEDs.
#define MVMNT_TICK_HZ 600 // How many times per second the light positions are calculated.
#define CHANCE 200
#define DEFAULT_RATE 15
#define TICKS_FOR_NEW_COLOR 5
#define STEPS_IN_INJ_ANIM 9

// Soft Enums
#define ALL_SEXTANTS 6
#define BTN_ARCADE 0
#define BTN_ENC 1

// GPIOs
#define PIN_BTN_ARC_0 1
#define PIN_BTN_ARC_1 1
#define PIN_BTN_ARC_2 1
#define PIN_BTN_ENC_0 1
#define PIN_BTN_ENC_1 1
#define PIN_BTN_ENC_2 1
#define PIN_ENC_0_A 1
#define PIN_ENC_0_B 1
#define PIN_ENC_1_A 1
#define PIN_ENC_1_B 1
#define PIN_ENC_2_A 1
#define PIN_ENC_2_B 1

// For now, only 1 control box. 
/*uint8_t  btnpins[2][NUM_CTRLS] = { { PIN_BTN_ARC_0, PIN_BTN_ARC_1, PIN_BTN_ARC_2 }, { PIN_BTN_ENC_0, PIN_BTN_ENC_1, PIN_BTN_ENC_2 } };
uint8_t encpins[2][NUM_CTRLS] = { { PIN_ENC_0_A, PIN_ENC_1_A, PIN_ENC_2_A }, { PIN_ENC_0_B, PIN_ENC_1_B, PIN_ENC_2_B } };*/
uint8_t  btnpins[2][NUM_CTRLS] = { { PIN_BTN_ARC_0 }, { PIN_BTN_ENC_0 } };
uint8_t encpins[2][NUM_CTRLS] = { { PIN_ENC_0_A }, { PIN_ENC_0_B } };


/***************** Data Structures ****************/
// Set of lights that can move from panel to panel. ("Box" really isn't accurate. PanelLights, maybe?)
typedef struct Box {
	uint8_t sextant;
	uint8_t box_idx;
	uint8_t hue = 0;
	uint8_t move_rate = DEFAULT_RATE;
	uint8_t move_counter = move_rate;
	Box *next;
	int age = 0;
} Box;


// Point of light that migrates. (Separate from actual, individual LEDs!)
typedef struct Pixel {
	uint8_t sextant;
	uint16_t addr;
	uint8_t hue;
	uint8_t move_rate;
	uint8_t move_counter;
	int age;
	Pixel * next;
} Pixel;


// Boxes that controls the panels and such.
typedef struct CBox {
	Encoder * enc;
	// uint8_t pin_btn_enc; // pin of button on rotary encoder
	uint8_t pin_btn_arc; // pin of arcade button
	uint8_t color_picker = 0; // what color is this controller currently.
	uint8_t new_color_counter = TICKS_FOR_NEW_COLOR; // how many ticks of the encoder before we change to the next color.

	uint8_t anim_step = STEPS_IN_INJ_ANIM;  // what step of the animation are we on
	uint8_t anim_delay_counter = DEFAULT_RATE; // how many ticks before we move to the next animation step
	bool animating_injection = false; 


	bool btn_pressed[2] = { false, false };
	long btn_press_time[2];
} CtrlBox;

enum Injectable
{
	PIXEL,
	SHAPE,
	SHAPE_CLASS,
	LEVEL
};

// Objects
IntervalTimer frame_ticker, movement_ticker;
CRGB leds[NUM_STRIPS * SEXTANT_LED_COUNT];

// Global Variables
CtrlBox ctrl[NUM_CTRLS];
uint8_t level_color[5];
uint8_t shape_color[8];
Box *head_box_ptr;
Pixel *head_px_ptr;

Encoder encr(1, 1); // <---- update these pin assignments

// Gates
volatile bool show_lights = false;
volatile bool move = false;


// Interrupt handler for LED update. Just lets us know lights can be updated.
void frameTick() {
  show_lights = true;
}


// Interrupt handler for movements. Just lets us know lights can move.
void mvmntTick() {
  move = true;
}


// TODO: Function description.
void lightBox(int sextant, int box, CHSV color) {

  CRGB c = color;

  for (int i = shape_start_addr[sextant][box]; i < shape_start_addr[sextant][box + 1]; i++)
  {
    leds[i] = c;
  }
}


// TODO: Function description.
void injectColor(uint8_t color) {
  //if (random8() > CHANCE) {
    Box new_box;
    new_box.hue = color;
    new_box.next = head_box_ptr;
    head_box_ptr = &new_box;
}


// TODO: Function description.
void btnArcade(uint8_t idx) {
// For now, pin_btn_enc is commented out. Make sure if cond isn't hit 'til the var comes back.
//  if (digitalRead(ctrl[idx].pin_btn_enc)) {
  if (0) {
    if (!ctrl[idx].btn_pressed[BTN_ARCADE]) {
      ctrl[idx].btn_pressed[BTN_ARCADE] = true;

      // When the button is pressed.
        // TODO: Start injection animation
      if (ctrl[idx].animating_injection) {
        for (int i = 0; i < 10; i++)
        {
          leds[ctrl_px_inject_path[i]] = CRGB::Black;
        }
        ctrl[idx].anim_step = 0;
      }
      else
      {
        ctrl[idx].animating_injection = true;
      }

        // Inject color
      injectColor(ctrl[idx].color_picker);
    }
  }
  else {
    ctrl[idx].btn_pressed[BTN_ARCADE] = false;
  }
}


// TODO: Function description.
void doMove() {
  // Injection Animation
  for (uint8_t i = 0; i < NUM_CTRLS; i++)
  {
    if (ctrl[i].animating_injection) {
      ctrl[i].anim_delay_counter--;
      if (!ctrl[i].anim_delay_counter) {
        ctrl[i].anim_delay_counter = DEFAULT_RATE;

        switch (ctrl[i].anim_step)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
          leds[ctrl_px_inject_path[ctrl[i].anim_step - 1]] = CRGB::Black;
        case 0:
          leds[ctrl_px_inject_path[ctrl[i].anim_step]] = CHSV(ctrl[i].color_picker, 255, 255);
          break;
        case 8:
          leds[ctrl_px_inject_path[ctrl[i].anim_step - 2]] = CRGB::Black;
        case 6:
          leds[ctrl_px_inject_path[ctrl[i].anim_step - 1]] = CRGB::Black;
          leds[ctrl_px_inject_path[ctrl[i].anim_step]] = CHSV(ctrl[i].color_picker, 255, 255);
          leds[ctrl_px_inject_path[ctrl[i].anim_step + 1]] = CHSV(ctrl[i].color_picker, 255, 255);
          ctrl[i].anim_step++;
        case 9:
          leds[ctrl_px_inject_path[ctrl[i].anim_step]] = CRGB::Black;
          leds[ctrl_px_inject_path[ctrl[i].anim_step - 1]] = CRGB::Black;
          // Stop the animation
          ctrl[i].animating_injection = false;
          ctrl[i].anim_step = 0;
        default:
          break;
        }

        ctrl[i].anim_step++;
      }
    }
  }
}


// TODO: Function description.
void btnEnc(uint8_t idx) {
// For now, pin_btn_enc is commented out. Make sure if cond isn't hit 'til the var comes back.
//  if (digitalRead(ctrl[idx].pin_btn_enc)) {
  if (0) {
    if (!ctrl[idx].btn_pressed[BTN_ENC]) {
      ctrl[idx].btn_pressed[BTN_ENC] = true;
      ctrl[idx].btn_press_time[BTN_ENC] = millis();
      // When the button is pressed.
    }
  }
  else {
    if (ctrl[idx].btn_pressed[BTN_ENC]) {
      ctrl[idx].btn_pressed[BTN_ENC] = false;
      // When the button is released.
      long press_time = millis() - ctrl[idx].btn_press_time[BTN_ARCADE];
      if (press_time > 0) {
        // Can do things depending how long the button was pressed.
      }
      else
      {
        // TODO: What goes here? Anything?
      }
    }
  }
}


// TODO: Function description.
void updateControllerLEDs(uint8_t idx, CHSV c) {
  // Selected color
  for (uint8_t i = 0; i < 9; i++)
  {
    leds[ctrl_px_selected[i]] = c;
  }
  // Spectrum

  c.h -= 5;
  for (int8_t i = 0; i < 10; i++)
  {
    leds[ctrl_px_spectrum[0]] = c;
    c.h += (i == 4 ? 1 : 2);
  }
}


// TODO: Function description.
void chkEncoder(uint8_t idx) {
  if (int8_t val = ctrl[idx].enc->read()) {
    ctrl[idx].new_color_counter += val;
    if (ctrl[idx].new_color_counter <= 0) {
      ctrl[idx].new_color_counter = TICKS_FOR_NEW_COLOR;
      ctrl[idx].color_picker--;
      updateControllerLEDs(idx, CHSV(ctrl[idx].color_picker, 255, 255));
    }
    else if (ctrl[idx].new_color_counter >= (2*TICKS_FOR_NEW_COLOR))
    {
      ctrl[idx].new_color_counter = TICKS_FOR_NEW_COLOR;
      ctrl[idx].color_picker++;
      updateControllerLEDs(idx, CHSV(ctrl[idx].color_picker, 255, 255));
    }
  }
  ctrl[idx].enc->write(0);
}


/* Functs that aren't needed for v1. Bring back for v2 or v3.
// TODO: Function description.
void lightLevelForSextant(int sextant, int level, CHSV color) {
  for (int j = 0; j < (level == 0 ? 2 : 4); j++) // only 2 shapes on level 0, 4 on all others
  {
    lightBox(sextant, levels[level][j], color);
  }
}


// TODO: Function description.
void lightLevel(int sextant, int level, CHSV color) {
  // Do for full piece
  if (sextant == ALL_SEXTANTS)
  {
    for (int i = 0; i < 6; i++)
    {
      lightLevelForSextant(i, level, color);
    }
  }
  // Do for a sextant
  else
  {
    lightLevelForSextant(sextant, level, color);
  }
}


// TODO: Function description.
void lightShapesForSextant(int sextant, int shape, CHSV color) {
  for (int j = 0; j < shape_count[shape]; j++) // only 2 shapes on level 0, 4 on all others
  {
    lightBox(sextant, shapes[shape][j], color);
  }
}


// TODO: Function description.
void lightShapes(int sextant, int shape, CHSV color) {
  // Do for full piece
  if (sextant == ALL_SEXTANTS)
  {
    for (int i = 0; i < 6; i++)
    {
      lightShapesForSextant(i, shape, color);
    }
  }
  // Do for a sextant
  else
  {
    lightShapesForSextant(sextant, shape, color);
  }
}
*/


// Setup function that inits all variables before the Arduino goes into the main loop.
void setup() {
  LEDS.addLeds<OCTOWS2811>(leds, SEXTANT_LED_COUNT); // LED setup.
  mapInit();  // Initialize pixel address array.
  Serial.begin(38400);
  Serial.println("I Live!");

  // Set callbacks.
  frame_ticker.begin(frameTick, (1000000 / FRAME_TICK_HZ));
  movement_ticker.begin(mvmntTick, (1000000 / MVMNT_TICK_HZ));

  // Init Controllers.
  for (int i = 0; i < NUM_CTRLS; i++)
  {
    ctrl[i].enc = &encr;
    ctrl[i].pin_btn_arc = btnpins[BTN_ARCADE][i];
//    ctrl[i].pin_btn_enc = btnpins[BTN_ENC][i];
  }
}


// The loop the Arduino enters after setup(). Ideally, any helper functs will go above this to keep
// Linux/UNIX-based systems happy when compiling.
void loop() {
	{// DEBUG
		static uint8_t hue = 0;
		lightBox(5, 0, CHSV(hue, 255, 255));
		lightBox(5, 1, CHSV(hue + 63, 255, 255));
		lightBox(5, 2, CHSV(hue + 127, 255, 255));
    lightBox(5, 3, CHSV(hue + 191, 255, 255));
		/*
		for (int i = 0; i < NUM_STRIPS; i++) {
			for (int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
				leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32 * i) + hue + j, 192, 255);
			}
		}

		*/
		// Set the first n leds on each strip to show which strip it is
		for (int i = 0; i < NUM_STRIPS; i++) {
			for (int j = 0; j <= i; j++) {
				leds[(i*SEXTANT_LED_COUNT) + j] = CRGB::Red;
			}
		}
		hue++;
		LEDS.show();
		LEDS.delay(10);
	}

	// Check knobs and buttons
	for (uint8_t i = 0; i < NUM_CTRLS; i++)
	{
		// chkEncoder(i);
		//btnEnc(i);
		btnArcade(i);
	}

	// Tick down the movement counter for each element
	if (move) {
		doMove();
		move = false;
	}

	// Show next frame gate
	if (show_lights) {
		LEDS.show();
		show_lights = false;
	}
}

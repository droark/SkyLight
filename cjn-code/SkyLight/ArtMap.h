// Map.h

#ifndef _MAP_h
#define _MAP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

enum Shape
{
	HEXAGON,
	SM_TRIANGLE,
	LG_TRIANGLE,
	J_SHAPE,
	SM_CHEVRON,
	LG_CHEVRON,
	SM_TRUNC_TRI,
	LG_TRUNC_TRI
};


#define SEXTANT_LED_COUNT 426


// FOR DEBUGGING
/*
// Holds addresses of pixels at the start of each shape in each sextant
int shape_start_addr[6][19] = 
{
	{ 0,5,17,35,47,62,77,92,131,158,185,212,260,272,284,296,314,362,428 },   // First sextant. The rest gets calculated on first startup
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};*/
int shape_start_addr[6][19] =
{
	{ 0,12,24,36,54,62,77,92,131,158,185,212,260,272,284,296,314,362,428 },   // First sextant. The rest gets calculated on first startup
	{ 0,12,24,36,54,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};
// END DEBUG


// How many of each shape are in a sextant. Needed to prevent for-loop spillover.
const uint8_t shape_count[8] = {3,1,3,3,4,2,1,1};

// Holds indices of shapes in shape_start_addr that correspond to like shapes
const uint8_t shapes[8][4] = 
{
	{ 0,2,15 },			// HEXAGON		
	{ 3 },				// SM_TRIANGLE
	{ 8,9,10 },			// LG_TRIANGLE
	{ 4,5,6 },			// J_SHAPE
	{ 1,12,13,14 },		// SM_CHEVRON
	{ 11,16 },			// LG_CHEVRON
	{ 7 },				// SM_TRUNC_TRI
	{ 17 },				// LG_TRUNC_TRI
};


// Holds indices of shapes in shape_start_addr that correspond to levels
const uint8_t levels[5][4] =
{
	{ 0,15,0,0 },		// Closest to audience. Only 2 Shapes in this one, so last two elements are unused
	{ 1,12,13,14 },
	{ 2,7,11,16 },
	{ 4,5,6,17 },
	{ 3,8,9,10 }		// Farthest from audience.
};

//TODO: update this when we decide to add more controllers
// Controller box pixel mapping
const uint8_t ctrl_px_spectrum[10] = {3,4,5,6,7,11,12,13,14,15};
const uint8_t ctrl_px_selected[9] = {0,1,2,8,9,10,16,17,18};
const uint8_t ctrl_px_inject_path[10] = {19,20,21,22,23,26,25,27,28,24 };



// This function populates all the data for the rest of the pixel address array.
void mapInit() {
	for (int i = 1; i < 6; i++) {
		for (int j = 0; j < 18; j++)
		{
			shape_start_addr[i][j] = shape_start_addr[i - 1][j];// +SEXTANT_LED_COUNT;
		}
	}
}

#endif


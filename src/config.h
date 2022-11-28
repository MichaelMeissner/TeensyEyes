#pragma once

#include "eyes/eyes.h"

// Enable the eye(s) you want to #include -- these are large graphics tables for various eyes:
// Note, you have to also adjust eyeDefinitions in main.cpp when you add more eyes.
// Meissner change, add more eyes.
#include "eyes/240x240/anime.h"
#include "eyes/240x240/bigBlue.h"
#include "eyes/240x240/brown.h"
#include "eyes/240x240/cat.h"
#include "eyes/240x240/demon.h"
#include "eyes/240x240/doe.h"
#include "eyes/240x240/doomRed.h"
#include "eyes/240x240/doomSpiral.h"
#include "eyes/240x240/dragon.h"
#include "eyes/240x240/fish.h"
#include "eyes/240x240/fizzgig.h"
#include "eyes/240x240/hazel.h"
#include "eyes/240x240/hypnoRed.h"
#include "eyes/240x240/newt.h"
#include "eyes/240x240/skull.h"
#include "eyes/240x240/snake.h"
#include "eyes/240x240/spikes.h"
#include "eyes/240x240/toonstripe.h"
// end Meissner change

#include "displays/GC9A01A_Display.h"

// DISPLAY HARDWARE SETTINGS (screen type & connections) -------------------

// Define the pins used for each display. If multiple displays use the same reset pin,
// only specify it on the FIRST display and leave the other set to -1. This prevents the
// GC9A01A_t3n->begin() function from resetting both displays after one is initialized.
// With two displays, one should be configured to mirror on the X axis. This simplifies
// eyelid handling -- no need for distinct L-to-R or R-to-L inner loops. Just the X
// coordinate of the iris is then reversed when drawing this eye, so they move the same.
GC9A01A_Config eyeInfo[] = {
//    // CS DC MOSI SCK RST ROT MIRROR USE_FB ASYNC
//    {0,  2, 26, 27, 3, 0, true,  true, true}, // Left display
//    {10, 9, 11, 13, 8, 0, false, true, true}, // Right display

  // Meissner changes
  // CS  DC  MOSI  SCK  RST  ROT  MIRROR USE_FB  ASYNC
  {   0, 24,   26,  27,  25,   0,      0,     1,  true },	// Right eye
  {  22,  9,   11,  13,  10,   0,      0,     1,  true }	// Left eye

};

// Override defaults
//#define CONFIG_EYE_DURATION_MS	4'000	// Eye change duration
//#define CONFIG_BLINK_PIN		-1	// Pin to do an explicit blink
//#define CONFIG_JOYSTICK_X_PIN		-1	// Analog input pin for joystick X axis
//#define CONFIG_JOYSTICK_Y_PIN		-1	// Analog input pin for joystick Y axis
//#define CONFIG_LIGHT_PIN		-1	// Light sensor pin

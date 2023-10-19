#pragma once

#include "eyes/eyes.h"

// Enable the eye(s) you want to #include -- these are large graphics tables for various eyes:
//#include "eyes/240x240/anime.h"
#include "eyes/240x240/bigBlue.h"
//#include "eyes/240x240/blueFlame1.h"
//#include "eyes/240x240/blueFlame2.h"
#include "eyes/240x240/brown.h"
#include "eyes/240x240/cat.h"
#include "eyes/240x240/demon.h"
#include "eyes/240x240/doe.h"
#include "eyes/240x240/doomRed.h"
#include "eyes/240x240/doomSpiral.h"
#include "eyes/240x240/dragon.h"
//#include "eyes/240x240/firebox.h"
//#include "eyes/240x240/fish.h"
#include "eyes/240x240/fizzgig.h"
//#include "eyes/240x240/flame.h"
//#include "eyes/240x240/hazel.h"
#include "eyes/240x240/hypnoRed.h"
//#include "eyes/240x240/leopard.h"
//#include "eyes/240x240/newt.h"
#include "eyes/240x240/skull.h"
#include "eyes/240x240/snake.h"
//#include "eyes/240x240/spikes.h"
#include "eyes/240x240/toonstripe.h"

#include "eyes/EyeController.h"

#ifdef ORIG_CODE
#define USE_GC9A01A
//#define USE_ST7789

#else	/* meissner changes.  */
#include "config-display.h"
#endif	/* meissner changes.  */

#ifdef USE_GC9A01A
#include "displays/GC9A01A_Display.h"
#elif defined USE_ST7789
#include "displays/ST7789_Display.h"
#ifdef ST7735_SPICLOCK
#undef ST7735_SPICLOCK
#endif
#define ST7735_SPICLOCK 30'000'000
#endif

// A list of all the different eye definitions we want to use
std::array<std::array<EyeDefinition, 2>, 13> eyeDefinitions{{
//                                                               {anime::left, anime::right},
                                                               {bigBlue::eye, bigBlue::eye},
//                                                               {blueFlame1::eye, blueFlame1::eye},
//                                                               {blueFlame2::eye, blueFlame2::eye},
                                                               {brown::eye, brown::eye},
                                                               {cat::eye, cat::eye},
                                                               {demon::left, demon::right},
                                                               {doe::left, doe::right},
                                                               {doomRed::eye, doomRed::eye},
                                                               {doomSpiral::left, doomSpiral::right},
                                                               {dragon::eye, dragon::eye},
//                                                               {firebox::eye, firebox::eye},
//                                                               {fish::eye, fish::eye},
                                                               {fizzgig::eye, fizzgig::eye},
//                                                               {flame::eye, flame::eye},
//                                                               {hazel::eye, hazel::eye},
                                                               {hypnoRed::eye, hypnoRed::eye},
//                                                               {leopard::left, leopard::right},
//                                                               {newt::eye, newt::eye},
                                                               {skull::eye, skull::eye},
                                                               {snake::eye, snake::eye},
//                                                                {spikes::eye, spikes::eye}
                                                               {toonstripe::eye, toonstripe::eye},
                                                           }
};

// DISPLAY HARDWARE SETTINGS (screen type & connections) -------------------

// Define the pins used for each display. If multiple displays use the same reset pin,
// only specify it on the FIRST display and leave the other set to -1. This prevents the
// GC9A01A_t3n->begin() function from resetting both displays after one is initialized.
// With two displays, one should be configured to mirror on the X axis. This simplifies
// eyelid handling -- no need for distinct L-to-R or R-to-L inner loops. Just the X
// coordinate of the iris is then reversed when drawing this eye, so they move the same.
#ifdef ORIG_CODE
#ifdef USE_GC9A01A
GC9A01A_Config eyeInfo[] = {
    // CS DC MOSI SCK RST ROT MIRROR USE_FB ASYNC
    {0,  2, 26, 27, 3, 0, true,  true, true}, // Left display
    {10, 9, 11, 13, 8, 0, false, true, true}, // Right display
};
#elif defined USE_ST7789
ST7789_Config eyeInfo[] = {
    // CS DC MOSI SCK RST ROT MIRROR USE_FB ASYNC
    {-1,  2, 26, 27, 3, 0, true,  true, true}, // Left display
    {-1, 9, 11, 13, 8, 0, false, true, true}, // Right display
};
#endif

#else	/* meissner changes.  */
#ifdef USE_GC9A01A
GC9A01A_Config eyeInfo[] = {
  // Meissner defaults
  // CS  DC  MOSI  SCK  RST  ROT  MIRROR USE_FB  ASYNC
  {   0, 24,   26,  27,  25,   2,      0,     1, true },	// Right eye
  {  22,  9,   11,  13,  10,   2,      0,     1, true }		// Left eye
};
#elif defined (USE_ST7789)
ST7789_Config eyeInfo[] = {
  // Meissner defaults
  // CS  DC  MOSI  SCK  RST  ROT  MIRROR USE_FB  ASYNC
  {   0, 24,   26,  27,  25,   2,      0,     1, true },	// Right eye
  {  22,  9,   11,  13,  10,   2,      0,     1, true }		// Left eye
};
#endif
#endif	/* meissner changes.  */

constexpr uint32_t EYE_DURATION_MS{4'000};

/// The speed of the SPI bus. For maximum performance, set this as high as you can get away with.
/// It will depend on the displays themselves, wire lengths, shielding/interference etc. My
/// setup works up to about 90,000,000. At 100,000,000 I start seeing corruption on the displays.
constexpr uint32_t SPI_SPEED{30'000'000};

// Set to -1 to disable the blink button and/or joystick
constexpr int8_t BLINK_PIN{-1};
constexpr int8_t JOYSTICK_X_PIN{-1};
constexpr int8_t JOYSTICK_Y_PIN{-1};
constexpr int8_t LIGHT_PIN{-1};
constexpr bool USE_PERSON_SENSOR{false};

#ifdef USE_GC9A01A
EyeController<2, GC9A01A_Display> *eyes{};
#elif defined USE_ST7789
EyeController<2, ST7789_Display> *eyes{};
#endif

void initEyes(bool autoMove, bool autoBlink, bool autoPupils) {
  // Create the displays and eye controller
  auto &defs = eyeDefinitions.at(0);
#ifdef USE_GC9A01A
  auto l = new GC9A01A_Display(eyeInfo[0], SPI_SPEED);
  auto r = new GC9A01A_Display(eyeInfo[1], SPI_SPEED);
  const DisplayDefinition<GC9A01A_Display> left{l, defs[0]};
  const DisplayDefinition<GC9A01A_Display> right{r, defs[1]};
  eyes = new EyeController<2, GC9A01A_Display>({left, right}, autoMove, autoBlink, autoPupils);
#elif defined USE_ST7789
  auto l = new ST7789_Display(eyeInfo[0]);
  auto r = new ST7789_Display(eyeInfo[1]);
  const DisplayDefinition<ST7789_Display> left{l, defs[0]};
  const DisplayDefinition<ST7789_Display> right{r, defs[1]};
  eyes = new EyeController<2, ST7789_Display>({left, right}, autoMove, autoBlink, autoPupils);
#endif
}

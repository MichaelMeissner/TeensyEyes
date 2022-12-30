#pragma once

#include "eyes/eyes.h"

#ifdef ORIG_CODE
// Enable the eye(s) you want to #include -- these are large graphics tables for various eyes:
//#include "eyes/240x240/anime.h"
#include "eyes/240x240/bigBlue.h"
#include "eyes/240x240/brown.h"
#include "eyes/240x240/cat.h"
#include "eyes/240x240/demon.h"
#include "eyes/240x240/doe.h"
#include "eyes/240x240/doomRed.h"
#include "eyes/240x240/doomSpiral.h"
#include "eyes/240x240/dragon.h"
//#include "eyes/240x240/fish.h"
#include "eyes/240x240/fizzgig.h"
//#include "eyes/240x240/hazel.h"
#include "eyes/240x240/hypnoRed.h"
//#include "eyes/240x240/leopard.h"
//#include "eyes/240x240/newt.h"
#include "eyes/240x240/skull.h"
#include "eyes/240x240/snake.h"
//#include "eyes/240x240/spikes.h"
#include "eyes/240x240/toonstripe.h"

#include "eyes/EyeController.h"
#include "displays/GC9A01A_Display.h"

// A list of all the different eye definitions we want to use
std::array<std::array<EyeDefinition, 2>, 13> eyeDefinitions{{
//                                                               {anime::eye, anime::eye},
                                                               {bigBlue::eye, bigBlue::eye},
                                                               {brown::eye, brown::eye},
                                                               {cat::eye, cat::eye},
                                                               {demon::left, demon::right},
                                                               {doe::left, doe::right},
                                                               {doomRed::eye, doomRed::eye},
                                                               {doomSpiral::left, doomSpiral::right},
                                                               {dragon::eye, dragon::eye},
//                                                               {fish::eye, fish::eye},
                                                               {fizzgig::eye, fizzgig::eye},
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

#else	/* meissner changes.  */
// Enable the eye(s) you want to #include -- these are large graphics tables for various eyes:
#include "eyes/240x240/cat.h"
#include "eyes/240x240/demon.h"
#include "eyes/240x240/dragon.h"
#include "eyes/240x240/hazel.h"
#include "eyes/240x240/hypnoRed.h"
#include "eyes/240x240/skull.h"
#include "eyes/240x240/snake.h"
#include "eyes/240x240/toonstripe.h"

// Note, Teensy 4.0 does not have enough space for all of the eyes, so only
// build a subset of the eyes on Teensy 4.0.  Build all 18 of the eyes on the
// Teensy 4.1.
#if defined(ARDUINO_TEENSY41)
#define ALL_EYES
#endif

#if defined(ALL_EYES)
#include "eyes/240x240/anime.h"
#include "eyes/240x240/bigBlue.h"
#include "eyes/240x240/brown.h"
#include "eyes/240x240/doe.h"
#include "eyes/240x240/doomRed.h"
#include "eyes/240x240/doomSpiral.h"
#include "eyes/240x240/fish.h"
#include "eyes/240x240/fizzgig.h"
#include "eyes/240x240/leopard.h"
#include "eyes/240x240/newt.h"
#include "eyes/240x240/spikes.h"

#define NUM_EYE_PATTERNS 19
#else
#define NUM_EYE_PATTERNS 8
#endif

#include "eyes/EyeController.h"
#include "displays/GC9A01A_Display.h"

// A list of all the different eye definitions we want to use
std::array<std::array<EyeDefinition, 2>, NUM_EYE_PATTERNS> eyeDefinitions{{
  { cat::eye,           cat::eye          },
  { demon::left,        demon::right      },
  { dragon::eye,        dragon::eye       },
  { hazel::eye,         hazel::eye        },
  { hypnoRed::eye,      hypnoRed::eye     },
  { skull::eye,         skull::eye        },
  { snake::eye,         snake::eye        },
  { toonstripe::eye,    toonstripe::eye   },

#if defined(ALL_EYES)
  { anime::eye,         anime::eye        },
  { bigBlue::eye,       bigBlue::eye      },
  { brown::eye,         brown::eye        },
  { doe::left,          doe::right        },
  { doomRed::eye,       doomRed::eye      },
  { doomSpiral::left,   doomSpiral::right },
  { fish::eye,          fish::eye         },
  { fizzgig::eye,       fizzgig::eye      },
  { leopard::eye,       leopard::eye      },
  { newt::eye,          newt::eye         },
  { spikes::eye,        spikes::eye       },
#endif	/* end of ALL_EYES.  */
}};
#endif

#ifdef ORIG_CODE
// DISPLAY HARDWARE SETTINGS (screen type & connections) -------------------

// Define the pins used for each display. If multiple displays use the same reset pin,
// only specify it on the FIRST display and leave the other set to -1. This prevents the
// GC9A01A_t3n->begin() function from resetting both displays after one is initialized.
// With two displays, one should be configured to mirror on the X axis. This simplifies
// eyelid handling -- no need for distinct L-to-R or R-to-L inner loops. Just the X
// coordinate of the iris is then reversed when drawing this eye, so they move the same.
GC9A01A_Config eyeInfo[] = {
    // CS DC MOSI SCK RST ROT MIRROR USE_FB ASYNC
    {0,  2, 26, 27, 3, 0, true,  true, true}, // Left display
    {10, 9, 11, 13, 8, 0, false, true, true}, // Right display
};

#else	/* meissner changes.  */
// DISPLAY HARDWARE SETTINGS (screen type & connections) -------------------

// Define the pins used for each display. If multiple displays use the same reset pin,
// only specify it on the FIRST display and leave the other set to -1. This prevents the
// GC9A01A_t3n->begin() function from resetting both displays after one is initialized.
// With two displays, one should be configured to mirror on the X axis. This simplifies
// eyelid handling -- no need for distinct L-to-R or R-to-L inner loops. Just the X
// coordinate of the iris is then reversed when drawing this eye, so they move the same.
GC9A01A_Config eyeInfo[] = {
  // Meissner defaults
  // CS  DC  MOSI  SCK  RST  ROT  MIRROR USE_FB  ASYNC
  {   0, 24,   26,  27,  25,   2,      0,     1, true },	// Right eye
  {  22,  9,   11,  13,  10,   2,      0,     1, true }		// Left eye
};
#endif	/* meissner changes.  */

#ifdef ORIG_CODE
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
constexpr bool PERSON_SENSOR_PRESENT{false};

#else	/* meissner changes.  */
constexpr uint32_t EYE_DURATION_MS{16'000};		// meissner, slow down change

/// The speed of the SPI bus. For maximum performance, set this as high as you can get away with.
/// It will depend on the displays themselves, wire lengths, shielding/interference etc. My
/// setup works up to about 90,000,000. At 100,000,000 I start seeing corruption on the displays.
constexpr uint32_t SPI_SPEED{90'000'000};		// meissner bump up speed

// Set to -1 to disable the blink button and/or joystick
constexpr int8_t BLINK_PIN{3};				// meissner blink pin
constexpr int8_t JOYSTICK_X_PIN{-1};
constexpr int8_t JOYSTICK_Y_PIN{-1};
constexpr int8_t LIGHT_PIN{-1};
constexpr bool PERSON_SENSOR_PRESENT{false};
#endif	/* meissner changes.  */

EyeController<2, GC9A01A_Display> *eyes{};

void initEyes(bool autoMove, bool autoBlink, bool autoPupils) {
  // Create the displays and eye controller
  auto &defs = eyeDefinitions.at(0);
  auto l = new GC9A01A_Display(eyeInfo[0], SPI_SPEED);
  auto r = new GC9A01A_Display(eyeInfo[1], SPI_SPEED);
  const DisplayDefinition<GC9A01A_Display> left{l, defs[0]};
  const DisplayDefinition<GC9A01A_Display> right{r, defs[1]};
  eyes = new EyeController<2, GC9A01A_Display>({left, right}, autoMove, autoBlink, autoPupils);
}

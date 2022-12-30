#pragma once

#include "../eyes.h"
#include "polarAngle_240.h"
#include "polarDist_240_120_75_0.h"
#include "disp_240_120.h"
#include "noeyelids_120.h"

namespace toonstripe {
  // 8x128, 16 bit 565 RGB
  constexpr uint16_t eyeIrisWidth = 8;
  constexpr uint16_t eyeIrisHeight = 128;
  const uint16_t eyeIris[eyeIrisWidth * eyeIrisHeight] PROGMEM = {
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB,
    0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4BB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4DB, 0xB4BB, 0xB4DB,
    0xB4DB, 0xB4BB, 0xB4DB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4DB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB,
    0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB,
    0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB,
    0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB,
    0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB4BB, 0xB49B, 0xB4BB, 0xB49B, 0xB4BB,
    0xB49B, 0xB4BB, 0xB4BB, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B,
    0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B,
    0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xB49B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xB49B,
    0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B,
    0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B,
    0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC5B, 0xAC5B, 0xAC7B, 0xAC7B,
    0xAC5B, 0xAC7B, 0xAC7B, 0xAC7B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B,
    0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC5B, 0xAC3B, 0xAC5B, 0xAC5B,
    0xAC3B, 0xAC5B, 0xAC5B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B,
    0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xAC3B, 0xA43B, 0xAC3B, 0xAC3B, 0xAC3B,
    0xA43B, 0xAC3B, 0xA43B, 0xAC3B, 0xA43B, 0xA43B, 0xA43B, 0xA43B, 0xA43B, 0xA43B, 0xA43B, 0xA43B,
    0xA41B, 0xA41B, 0xA41B, 0xA41B, 0xA41B, 0xA41B, 0xA43B, 0xA43B, 0xA41B, 0xA41B, 0xA41B, 0xA41B,
    0xA41B, 0xA41B, 0xA41B, 0xA41B, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A,
    0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA41A, 0xA3FA, 0xA3FA, 0xA41A, 0xA3FA,
    0xA3FA, 0xA41A, 0xA41A, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA,
    0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3FA, 0xA3DA, 0xA3FA, 0xA3DA, 0xA3DA, 0xA3DA, 0xA3DA,
    0xA3DA, 0xA3DA, 0xA3DA, 0xA3DA, 0x9BDA, 0xA3DA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0xA3DA,
    0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA,
    0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BDA, 0x9BBA, 0x9BBA, 0x9BDA, 0x9BBA, 0x9BBA,
    0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA,
    0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9BBA, 0x9B9A,
    0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A,
    0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x9B9A, 0x937A, 0x9B9A, 0x937A, 0x937A, 0x9B9A, 0x937A, 0x937A,
    0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A,
    0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A,
    0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x937A, 0x935A, 0x935A,
    0x937A, 0x937A, 0x935A, 0x935A, 0x935A, 0x935A, 0x9359, 0x935A, 0x935A, 0x9359, 0x9359, 0x935A,
    0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359, 0x9359,
    0x9359, 0x9359, 0x9359, 0x9359, 0x9339, 0x9339, 0x9339, 0x9339, 0x9339, 0x9359, 0x9339, 0x9339,
    0x8B39, 0x8B39, 0x9339, 0x8B39, 0x9339, 0x9339, 0x8B39, 0x8B39, 0x8B39, 0x8B39, 0x8B39, 0x8B39,
    0x8B19, 0x8B39, 0x8B39, 0x8B39, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19,
    0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19,
    0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19, 0x8B19,
    0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9,
    0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AF9, 0x8AD9, 0x8AD9, 0x8AF9, 0x8AD9, 0x8AD9,
    0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9,
    0x8AD9, 0x8AD9, 0x8AD9, 0x8AD9, 0x82D9, 0x82D9, 0x82D9, 0x82D9, 0x82D9, 0x82D9, 0x82D9, 0x82D9,
    0x82B9, 0x82D9, 0x82D9, 0x82D9, 0x82B9, 0x82D9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9,
    0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9,
    0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9, 0x82B9,
    0x8299, 0x82B9, 0x82B9, 0x82B9, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299,
    0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8299, 0x8278, 0x8299, 0x8299,
    0x8299, 0x8299, 0x8299, 0x8299, 0x8278, 0x8278, 0x8278, 0x8278, 0x8278, 0x8278, 0x8278, 0x8278,
    0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x8278, 0x8278, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78,
    0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78, 0x7A78,
    0x7A58, 0x7A78, 0x7A78, 0x7A78, 0x7A58, 0x7A78, 0x7A78, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58,
    0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58,
    0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58,
    0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58, 0x7A58,
    0x7A58, 0x7A38, 0x7A38, 0x7A38, 0x7A58, 0x7A38, 0x7A38, 0x7A58, 0x7A38, 0x7A38, 0x7A38, 0x7A38,
    0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38,
    0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7A38, 0x7218, 0x7218, 0x7A38, 0x7218,
    0x7218, 0x7A38, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7A38, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218, 0x7218,
    0x7218, 0x7218, 0x7218, 0x7218
  };

  // 1x100, 16 bit 565 RGB
  constexpr uint16_t eyeScleraWidth = 1;
  constexpr uint16_t eyeScleraHeight = 100;
  const uint16_t eyeSclera[eyeScleraWidth * eyeScleraHeight] PROGMEM = {
    0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xFC98, 0xFC98, 0xFC98,
    0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF,
    0xBFDF, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xBFDF, 0xBFDF, 0xBFDF,
    0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98,
    0xFC98, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xFC98, 0xFC98, 0xFC98, 0xFC98,
    0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xFC98, 0xFC98, 0xFC98,
    0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xFC98, 0xFC98,
    0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xFC98, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF,
    0xBFDF, 0xBFDF, 0xBFDF, 0xBFDF
  };

  const EyeDefinition eye PROGMEM = {
      "toonstripe", 120, 35793, true, 0.5, disp_240_120,
      { 24579, 0, 0.3, 0.7 },
      { 75, { eyeIris, eyeIrisWidth, eyeIrisHeight }, 0, 0, 0, 0, 0 },
      { { eyeSclera, eyeScleraWidth, eyeScleraHeight }, 0, 0, 0, 0, 0 },
      { noUpper_120, noLower_120, 0 },
      { 240, polarAngle_240, polarDist_240_120_75_0 }
  };
}

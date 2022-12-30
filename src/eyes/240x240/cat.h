#pragma once

#include "../eyes.h"
#include "polarAngle_240.h"
#include "polarDist_240_125_90_90.h"
#include "disp_240_125.h"

namespace cat {
  // An array of vertical start (inclusive) and end (exclusive) locations for each eyeUpper eyelid column
  const uint8_t eyeUpper[screenWidth * 2] PROGMEM = {
    0x8F, 0x9B, 0x88, 0x9C, 0x82, 0x9D, 0x7D, 0x9F, 0x78, 0xA0, 0x74, 0xA1, 0x71, 0xA2, 0x6D, 0xA3,
    0x6A, 0xA4, 0x67, 0xA5, 0x64, 0xA6, 0x61, 0xA7, 0x5E, 0xA8, 0x5C, 0xA9, 0x59, 0xAA, 0x57, 0xAB,
    0x55, 0xAC, 0x53, 0xAD, 0x51, 0xAD, 0x4F, 0xAE, 0x4D, 0xAF, 0x4B, 0xB0, 0x49, 0xB1, 0x47, 0xB1,
    0x45, 0xB2, 0x43, 0xB3, 0x42, 0xB3, 0x40, 0xB4, 0x3E, 0xB5, 0x3D, 0xB5, 0x3B, 0xB6, 0x3A, 0xB7,
    0x38, 0xB7, 0x37, 0xB8, 0x36, 0xB8, 0x34, 0xB9, 0x33, 0xBA, 0x32, 0xBA, 0x30, 0xBB, 0x2F, 0xBB,
    0x2E, 0xBC, 0x2D, 0xBC, 0x2B, 0xBD, 0x2A, 0xBD, 0x29, 0xBE, 0x28, 0xBE, 0x27, 0xBF, 0x26, 0xBF,
    0x25, 0xC0, 0x24, 0xC0, 0x23, 0xC1, 0x22, 0xC1, 0x21, 0xC1, 0x20, 0xC2, 0x1F, 0xC2, 0x1E, 0xC3,
    0x1D, 0xC3, 0x1C, 0xC4, 0x1C, 0xC4, 0x1B, 0xC4, 0x1A, 0xC5, 0x19, 0xC5, 0x18, 0xC5, 0x17, 0xC6,
    0x17, 0xC6, 0x16, 0xC6, 0x15, 0xC7, 0x15, 0xC7, 0x14, 0xC7, 0x13, 0xC8, 0x13, 0xC8, 0x12, 0xC8,
    0x11, 0xC9, 0x11, 0xC9, 0x10, 0xC9, 0x0F, 0xC9, 0x0F, 0xCA, 0x0E, 0xCA, 0x0E, 0xCA, 0x0D, 0xCB,
    0x0D, 0xCB, 0x0C, 0xCB, 0x0C, 0xCB, 0x0B, 0xCC, 0x0B, 0xCC, 0x0A, 0xCC, 0x0A, 0xCC, 0x09, 0xCC,
    0x09, 0xCD, 0x08, 0xCD, 0x08, 0xCD, 0x08, 0xCD, 0x07, 0xCD, 0x07, 0xCE, 0x06, 0xCE, 0x06, 0xCE,
    0x06, 0xCE, 0x05, 0xCE, 0x05, 0xCF, 0x05, 0xCF, 0x05, 0xCF, 0x04, 0xCF, 0x04, 0xCF, 0x04, 0xCF,
    0x04, 0xCF, 0x03, 0xD0, 0x03, 0xD0, 0x03, 0xD0, 0x03, 0xD0, 0x02, 0xD0, 0x02, 0xD0, 0x02, 0xD0,
    0x02, 0xD0, 0x02, 0xD1, 0x02, 0xD1, 0x02, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1,
    0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1,
    0x01, 0xD1, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2,
    0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2, 0x01, 0xD2,
    0x01, 0xD2, 0x01, 0xD1, 0x01, 0xD1, 0x01, 0xD1, 0x02, 0xD1, 0x02, 0xD1, 0x02, 0xD1, 0x02, 0xD1,
    0x02, 0xD1, 0x02, 0xD1, 0x03, 0xD1, 0x03, 0xD1, 0x03, 0xD1, 0x03, 0xD1, 0x03, 0xD1, 0x04, 0xD1,
    0x04, 0xD0, 0x04, 0xD0, 0x04, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x05, 0xD0, 0x06, 0xD0, 0x06, 0xD0,
    0x06, 0xCF, 0x07, 0xCF, 0x07, 0xCF, 0x08, 0xCF, 0x08, 0xCF, 0x08, 0xCF, 0x09, 0xCF, 0x09, 0xCE,
    0x0A, 0xCE, 0x0A, 0xCE, 0x0B, 0xCE, 0x0B, 0xCE, 0x0C, 0xCD, 0x0C, 0xCD, 0x0D, 0xCD, 0x0E, 0xCD,
    0x0E, 0xCC, 0x0F, 0xCC, 0x10, 0xCC, 0x10, 0xCC, 0x11, 0xCB, 0x12, 0xCB, 0x12, 0xCB, 0x13, 0xCB,
    0x14, 0xCA, 0x15, 0xCA, 0x16, 0xCA, 0x17, 0xC9, 0x17, 0xC9, 0x18, 0xC9, 0x19, 0xC8, 0x1A, 0xC8,
    0x1B, 0xC8, 0x1C, 0xC7, 0x1E, 0xC7, 0x1F, 0xC7, 0x20, 0xC6, 0x21, 0xC6, 0x22, 0xC5, 0x24, 0xC5,
    0x25, 0xC5, 0x26, 0xC4, 0x28, 0xC4, 0x29, 0xC3, 0x2B, 0xC3, 0x2C, 0xC2, 0x2E, 0xC2, 0x2F, 0xC1,
    0x31, 0xC1, 0x33, 0xC0, 0x35, 0xBF, 0x37, 0xBF, 0x39, 0xBE, 0x3B, 0xBE, 0x3D, 0xBD, 0x40, 0xBC,
    0x42, 0xBB, 0x45, 0xBB, 0x47, 0xBA, 0x4A, 0xB9, 0x4D, 0xB8, 0x51, 0xB7, 0x54, 0xB6, 0x58, 0xB5,
    0x5C, 0xB4, 0x60, 0xB3, 0x65, 0xB2, 0x6A, 0xB1, 0x70, 0xB0, 0x77, 0xAE, 0x7F, 0xAC, 0x8B, 0xAB
  };

  // An array of vertical start (inclusive) and end (exclusive) locations for each eyeLower eyelid column
  const uint8_t eyeLower[screenWidth * 2] PROGMEM = {
    0x99, 0x9C, 0x9A, 0x9E, 0x9B, 0xA0, 0x9C, 0xA2, 0x9D, 0xA4, 0x9E, 0xA6, 0x9F, 0xA7, 0xA0, 0xA9,
    0xA0, 0xAB, 0xA1, 0xAD, 0xA2, 0xAE, 0xA3, 0xB0, 0xA4, 0xB1, 0xA5, 0xB3, 0xA6, 0xB4, 0xA6, 0xB6,
    0xA7, 0xB7, 0xA8, 0xB8, 0xA9, 0xBA, 0xA9, 0xBB, 0xAA, 0xBC, 0xAB, 0xBE, 0xAC, 0xBF, 0xAC, 0xC0,
    0xAD, 0xC1, 0xAE, 0xC2, 0xAE, 0xC3, 0xAF, 0xC5, 0xB0, 0xC6, 0xB0, 0xC7, 0xB1, 0xC8, 0xB1, 0xC9,
    0xB2, 0xCA, 0xB3, 0xCB, 0xB3, 0xCC, 0xB4, 0xCC, 0xB4, 0xCD, 0xB5, 0xCE, 0xB5, 0xCF, 0xB6, 0xD0,
    0xB6, 0xD1, 0xB7, 0xD2, 0xB7, 0xD2, 0xB8, 0xD3, 0xB8, 0xD4, 0xB9, 0xD5, 0xB9, 0xD6, 0xBA, 0xD6,
    0xBA, 0xD7, 0xBB, 0xD8, 0xBB, 0xD8, 0xBC, 0xD9, 0xBC, 0xDA, 0xBD, 0xDA, 0xBD, 0xDB, 0xBD, 0xDC,
    0xBE, 0xDC, 0xBE, 0xDD, 0xBF, 0xDD, 0xBF, 0xDE, 0xBF, 0xDF, 0xC0, 0xDF, 0xC0, 0xE0, 0xC0, 0xE0,
    0xC1, 0xE1, 0xC1, 0xE1, 0xC1, 0xE2, 0xC2, 0xE2, 0xC2, 0xE3, 0xC2, 0xE3, 0xC3, 0xE4, 0xC3, 0xE4,
    0xC3, 0xE5, 0xC4, 0xE5, 0xC4, 0xE5, 0xC4, 0xE6, 0xC5, 0xE6, 0xC5, 0xE7, 0xC5, 0xE7, 0xC5, 0xE7,
    0xC6, 0xE8, 0xC6, 0xE8, 0xC6, 0xE8, 0xC6, 0xE9, 0xC7, 0xE9, 0xC7, 0xE9, 0xC7, 0xEA, 0xC7, 0xEA,
    0xC8, 0xEA, 0xC8, 0xEB, 0xC8, 0xEB, 0xC8, 0xEB, 0xC9, 0xEC, 0xC9, 0xEC, 0xC9, 0xEC, 0xC9, 0xEC,
    0xC9, 0xEC, 0xC9, 0xED, 0xCA, 0xED, 0xCA, 0xED, 0xCA, 0xED, 0xCA, 0xEE, 0xCA, 0xEE, 0xCA, 0xEE,
    0xCB, 0xEE, 0xCB, 0xEE, 0xCB, 0xEE, 0xCB, 0xEF, 0xCB, 0xEF, 0xCB, 0xEF, 0xCB, 0xEF, 0xCC, 0xEF,
    0xCC, 0xEF, 0xCC, 0xEF, 0xCC, 0xEF, 0xCC, 0xEF, 0xCC, 0xF0, 0xCC, 0xF0, 0xCC, 0xF0, 0xCC, 0xF0,
    0xCC, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0,
    0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0,
    0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xF0, 0xCD, 0xEF, 0xCD, 0xEF, 0xCD, 0xEF, 0xCD, 0xEF,
    0xCD, 0xEF, 0xCD, 0xEF, 0xCD, 0xEF, 0xCD, 0xEF, 0xCD, 0xEE, 0xCD, 0xEE, 0xCD, 0xEE, 0xCD, 0xEE,
    0xCD, 0xEE, 0xCD, 0xEE, 0xCD, 0xED, 0xCC, 0xED, 0xCC, 0xED, 0xCC, 0xED, 0xCC, 0xED, 0xCC, 0xEC,
    0xCC, 0xEC, 0xCC, 0xEC, 0xCC, 0xEC, 0xCC, 0xEB, 0xCC, 0xEB, 0xCB, 0xEB, 0xCB, 0xEB, 0xCB, 0xEA,
    0xCB, 0xEA, 0xCB, 0xEA, 0xCB, 0xE9, 0xCA, 0xE9, 0xCA, 0xE9, 0xCA, 0xE8, 0xCA, 0xE8, 0xCA, 0xE8,
    0xCA, 0xE7, 0xC9, 0xE7, 0xC9, 0xE7, 0xC9, 0xE6, 0xC9, 0xE6, 0xC9, 0xE5, 0xC8, 0xE5, 0xC8, 0xE4,
    0xC8, 0xE4, 0xC8, 0xE4, 0xC7, 0xE3, 0xC7, 0xE3, 0xC7, 0xE2, 0xC7, 0xE2, 0xC6, 0xE1, 0xC6, 0xE0,
    0xC6, 0xE0, 0xC5, 0xDF, 0xC5, 0xDF, 0xC5, 0xDE, 0xC4, 0xDE, 0xC4, 0xDD, 0xC4, 0xDC, 0xC3, 0xDC,
    0xC3, 0xDB, 0xC3, 0xDA, 0xC2, 0xDA, 0xC2, 0xD9, 0xC1, 0xD8, 0xC1, 0xD8, 0xC1, 0xD7, 0xC0, 0xD6,
    0xC0, 0xD5, 0xBF, 0xD5, 0xBF, 0xD4, 0xBE, 0xD3, 0xBE, 0xD2, 0xBD, 0xD1, 0xBD, 0xD0, 0xBC, 0xCF,
    0xBC, 0xCE, 0xBB, 0xCD, 0xBA, 0xCC, 0xBA, 0xCB, 0xB9, 0xCA, 0xB9, 0xC9, 0xB8, 0xC8, 0xB7, 0xC7,
    0xB6, 0xC6, 0xB6, 0xC4, 0xB5, 0xC3, 0xB4, 0xC2, 0xB3, 0xC0, 0xB3, 0xBF, 0xB2, 0xBD, 0xB1, 0xBC,
    0xB0, 0xBA, 0xAF, 0xB8, 0xAE, 0xB7, 0xAD, 0xB5, 0xAB, 0xB3, 0xAA, 0xB0, 0xA9, 0xAE, 0xA8, 0xAB
  };

  const EyeDefinition eye PROGMEM = {
      "cat", 125, 65504, true, 0.5, disp_240_125,
      { 0, 90, 0.3, 0.4 },
      { 90, { nullptr, 0, 0 }, 65504, 0, 0, 0, 0 },
      { { nullptr, 0, 0 }, 65504, 0, 0, 0, 0 },
      { eyeUpper, eyeLower, 0 },
      { 240, polarAngle_240, polarDist_240_125_90_90 },
      { "cat" }
  };
}

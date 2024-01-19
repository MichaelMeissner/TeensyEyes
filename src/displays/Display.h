#pragma once

#include <Arduino.h>

#ifdef ORIG_CODE
// #define SHOW_FPS

#else	/* meissner changes.  */
#define SHOW_FPS
#endif	/* meissner changes.  */

template <typename T>
class Display {
public:
  inline void drawPixel(int16_t x, int16_t y, uint16_t color565) const __attribute__((always_inline)) {
    static_cast<T&>(*this).drawPixel(x, y, color565);
  }

  inline void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color565) const __attribute__((always_inline)) {
    static_cast<T&>(*this).drawFastVLine(x, y, h, color565);
  }

  void drawText(uint16_t x, uint16_t y, char *text) {
    static_cast<T&>(*this).drawText(x, y, text);
  }

  void update() {
    static_cast<T&>(*this).update();
  }

  /// Whether the display is available for drawing to.
  /// \return true if the display is ready for use, false otherwise.
  bool isAvailable() {
    return static_cast<T&>(*this).isAvailable();
  }
};

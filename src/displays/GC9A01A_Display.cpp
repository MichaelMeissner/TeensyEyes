#include "GC9A01A_Display.h"

GC9A01A_t3n *createDisplay(const GC9A01A_Config &config) {
  // If project involves only ONE eye and NO other SPI devices, its select line can be
  // permanently tied to GND and corresponding pin set to -1. Best to use it though.
  if (config.cs >= 0) {
    pinMode(config.cs, OUTPUT);
    digitalWrite(config.cs, HIGH); // Deselect them all
  }
  return new GC9A01A_t3n(config.cs, config.dc, config.rst, config.mosi, config.sck);
}

GC9A01A_Display::GC9A01A_Display(const GC9A01A_Config &config) : display(createDisplay(config)),
                                                                 asyncUpdates(config.asyncUpdates) {
  static size_t displayNum{};
  Serial.print(F("Init GC9A01A display #"));
  Serial.print(displayNum);
  Serial.print(F(": rotate="));
  Serial.print(config.rotation);
  Serial.print(F(", mirror="));
  Serial.println(config.mirror);
  display->begin();
  display->setRotation(config.rotation);
  if (config.mirror) {
    const std::array<uint8_t, 4> mirrorTFT{0x8, 0x20, 0x40, 0xE0}; // Mirror + rotate
    display->sendCommand(GC9A01A_MADCTL, &mirrorTFT.at(config.rotation & 3), 1);
  }

  if (config.useFrameBuffer) {
    Serial.print(displayNum);
    Serial.print(F(": useFrameBuffer() "));
    if (!display->useFrameBuffer(true)) {
      Serial.println(F("failed"));
    } else {
      Serial.println(F("OK"));
    }
  }
  Serial.println(F("Success"));
  this->displayNum = displayNum;
  displayNum++;
}

GC9A01A_Display::~GC9A01A_Display() {
  // Do not delete the display element.  It gets the following warning:
  // In file included from /home/meissner/Arduino/meissner/TeensyEyes/build-GC9A01A.cpp:6:
  // GC9A01A_Display.cpp: In destructor 'virtual GC9A01A_Display::~GC9A01A_Display()':
  // GC9A01A_Display.cpp:45:3: warning: deleting object of polymorphic class type 'GC9A01A_t3n' which has non-virtual destructor might cause undefined behavior [-Wdelete-non-virtual-dtor]

  // delete display;
}

void GC9A01A_Display::drawPixel(int16_t x, int16_t y, uint16_t color565) {
  display->drawPixel(x, y, color565);
}

void GC9A01A_Display::drawText(int16_t x, int16_t y, char *text) {
  display->setCursor(x, y);
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);
  display->print(text);
}

void GC9A01A_Display::update() {

#ifdef SHOW_FPS
  // A per-display FPS counter
  if (elapsed >= 1000L) {
    fps = framesDrawn;
    framesDrawn = 0;
    elapsed = 0;
  }
  display->setTextSize(2);
  display->setTextColor(WHITE, BLACK);
  display->drawNumber(fps, 110, 110);
  framesDrawn++;
#endif

  if (asyncUpdates) {
    if (!display->updateScreenAsync()) {
      Serial.print(F("updateScreenAsync() failed for display "));
      Serial.println(displayNum);
    }
  } else {
    display->updateScreen();
  }
}

bool GC9A01A_Display::isAvailable() const {
  return !display->asyncUpdateActive();
}

// Define if you wish to debug memory usage.  Only works on T4.x
//#define DEBUG_MEMORY

#include <SPI.h>
#include <array>

#include "config.h"
#include "util/logging.h"
#include "eyes/EyeController.h"
#include "displays/GC9A01A_Display.h"
#include "LightSensor.h"

// Allow config.h to override the defaults for eye duration time, blink pin,
// joystick pins, and light sensor pins.
#ifndef CONFIG_EYE_DURATION_MS
#define CONFIG_EYE_DURATION_MS	4'000
#endif

// Set to -1 to disable the blink button and/or joystick
#ifndef CONFIG_BLINK_PIN
#define CONFIG_BLINK_PIN	-1
#endif

#ifndef CONFIG_JOYSTICK_X_PIN
#define CONFIG_JOYSTICK_X_PIN	-1
#endif

#ifndef CONFIG_JOYSTICK_Y_PIN
#define CONFIG_JOYSTICK_Y_PIN	-1
#endif

#ifndef CONFIG_LIGHT_PIN
#define CONFIG_LIGHT_PIN	-1
#endif

constexpr uint32_t EYE_DURATION_MS{CONFIG_EYE_DURATION_MS};
constexpr int8_t BLINK_PIN{CONFIG_BLINK_PIN};
constexpr int8_t JOYSTICK_X_PIN{CONFIG_JOYSTICK_X_PIN};
constexpr int8_t JOYSTICK_Y_PIN{CONFIG_JOYSTICK_Y_PIN};
constexpr int8_t LIGHT_PIN{CONFIG_LIGHT_PIN};

// TFT: use max SPI (clips to 12 MHz on M0)
constexpr uint32_t SPI_FREQUENCY{48'000'000};
const SPISettings settings(SPI_FREQUENCY, MSBFIRST, SPI_MODE0);

// Start Meissner change, add more eyes
// A list of all the different eye definitions we want to use
std::array<std::array<EyeDefinition, 2>, 18> eyeDefinitions{{
                                                                {anime::eye, anime::eye},
                                                                {bigBlue::eye, bigBlue::eye},
                                                                {brown::eye, brown::eye},
                                                                {cat::eye, cat::eye},
                                                                {demon::left, demon::right},
                                                                {doe::left, doe::right},
                                                                {doomRed::eye, doomRed::eye},
                                                                {doomSpiral::left, doomSpiral::right},
                                                                {dragon::eye, dragon::eye},
                                                                {fish::eye, fish::eye},
                                                                {fizzgig::eye, fizzgig::eye},
                                                                {hazel::eye, hazel::eye},
                                                                {hypnoRed::eye, hypnoRed::eye},
                                                                {newt::eye, newt::eye},
                                                                {skull::eye, skull::eye},
                                                                {snake::eye, snake::eye},
                                                                {spikes::eye, spikes::eye},
                                                                {toonstripe::eye, toonstripe::eye},
                                                            }
};
// End Meissner change, add more eyes

// The index of the currently selected eye definitions
static uint32_t defIndex{0};

EyeController<2, GC9A01A_Display> *eyes{};

LightSensor lightSensor(LIGHT_PIN);

bool hasBlinkButton() {
  return BLINK_PIN >= 0;
}

bool hasLightSensor() {
  return LIGHT_PIN >= 0;
}

bool hasJoystick() {
  return JOYSTICK_X_PIN >= 0 && JOYSTICK_Y_PIN >= 0;
}

/// INITIALIZATION -- runs once at startup ----------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000);
  delay(200);
  DumpMemoryInfo();
  Serial.println("Init");
  Serial.flush();
  randomSeed(analogRead(A3)); // Seed random() from floating analog input

  if (hasBlinkButton()) {
    pinMode(BLINK_PIN, INPUT_PULLUP);
  }

  if (hasJoystick()) {
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);
  }

  // Create the displays and eye controller
  auto &defs = eyeDefinitions.at(defIndex);
  auto l = new GC9A01A_Display(eyeInfo[0]);
  auto r = new GC9A01A_Display(eyeInfo[1]);
  const DisplayDefinition<GC9A01A_Display> left{l, defs[0]};
  const DisplayDefinition<GC9A01A_Display> right{r, defs[1]};
  eyes = new EyeController<2, GC9A01A_Display>({left, right}, !hasJoystick(), !hasBlinkButton(), !hasLightSensor());

 if (defs[0].name && defs[1].name) {
   if (defs[0].name == defs[1].name)
     Serial.printf ("Eye (%d): %s\n", defIndex, defs[0].name);
   else
     Serial.printf ("Eye (%d): %s, %s\n", defIndex, defs[0].name, defs[1].name);
 }
}

void nextEye() {
  // For the first pass go through the eyes in linear order.  After the first
  // pass choose the next eye at random.
  static bool in_order = true;
  uint32_t size = eyeDefinitions.size();
  if (in_order && defIndex < size-1) {
    defIndex++;

  } else {
    in_order = false;
    size_t loop_count = 0;
    uint32_t oldIndex = defIndex;
    do {
      defIndex = random (0, size - 1);
    } while (defIndex == oldIndex && ++loop_count < 4);
  }

  eyes->updateDefinitions(eyeDefinitions.at(defIndex));

 auto &defs = eyeDefinitions.at(defIndex);
 if (defs[0].name && defs[1].name) {
   if (defs[0].name == defs[1].name)
     Serial.printf ("Eye (%d): %s\n", defIndex, defs[0].name);
   else
     Serial.printf ("Eye (%d): %s, %s\n", defIndex, defs[0].name, defs[1].name);
 }
}

/// MAIN LOOP -- runs continuously after setup() ----------------------------
void loop() {
  // Switch eyes periodically
  static elapsedMillis eyeTime{};
  if (eyeTime > EYE_DURATION_MS) {
    nextEye();
    eyeTime = 0;
  }

  // Blink on button press
  if (hasBlinkButton() && digitalRead(BLINK_PIN) == LOW) {
    eyes->blink();
  }

  // Move eyes with an analog joystick
  if (hasJoystick()) {
    auto x = analogRead(JOYSTICK_X_PIN);
    auto y = analogRead(JOYSTICK_Y_PIN);
    eyes->setPosition((x - 512) / 512.0f, (y - 512) / 512.0f);
  }

  if (hasLightSensor()) {
    lightSensor.readDamped([](float value) {
      eyes->setPupil(value);
    });
  }

  eyes->renderFrame();
}

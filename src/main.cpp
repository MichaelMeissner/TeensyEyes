// Define if you wish to debug memory usage.  Only works on T4.x
//#define DEBUG_MEMORY

#include <SPI.h>
#include <array>
#include <Wire.h>

#include "config.h"
#include "util/logging.h"
#include "sensors/LightSensor.h"
#include "sensors/PersonSensor.h"

// The index of the currently selected eye definitions
static uint32_t defIndex{0};

LightSensor lightSensor(LIGHT_PIN);
PersonSensor personSensor;

bool hasBlinkButton() {
  return BLINK_PIN >= 0;
}

bool hasLightSensor() {
  return LIGHT_PIN >= 0;
}

bool hasJoystick() {
  return JOYSTICK_X_PIN >= 0 && JOYSTICK_Y_PIN >= 0;
}

bool hasPersonSensor() {
  return PERSON_SENSOR_PRESENT;
}

static void printEyeName (const char *lname, const char *rname)
{
  if (!lname || lname[0] == '\0')
    lname = "no left name";

  if (!rname || rname[0] == '\0')
    rname = "no right name";

  if (strcmp (lname, rname) == 0)
    Serial.printf ("Eye #%-2d %s\n", (int)defIndex, lname);
  else
    Serial.printf ("Eye #%-2d %s, %s\n", (int)defIndex, lname, rname);
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

  if (hasPersonSensor()) {
    Wire.begin();
    personSensor.enableID(false);
    personSensor.enableLED(false);
    personSensor.setMode(PersonSensor::Mode::Continuous);
  }

  initEyes(!hasJoystick(), !hasBlinkButton(), !hasLightSensor());
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
 printEyeName (defs[0].name, defs[1].name);
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

  if (hasPersonSensor() && personSensor.read()) {
    // Find the closest face that is facing the camera, if any
    int maxSize = 0;
    person_sensor_face_t maxFace{};
    
    for (int i = 0; i < personSensor.numFacesFound(); i++) {
      const person_sensor_face_t face = personSensor.faceDetails(i);
      if (face.is_facing && face.box_confidence > 150) {
        int size = (face.box_right - face.box_left) * (face.box_bottom - face.box_top);
        if (size > maxSize) {
          maxSize = size;
          maxFace = face;
        }
      }
    }

    if (maxSize > 0) {
      eyes->setAutoMove(false);
      float targetX = (static_cast<float>(maxFace.box_left) + static_cast<float>(maxFace.box_right - maxFace.box_left) / 2.0f) / 127.5f - 1.0f;
      float targetY = (static_cast<float>(maxFace.box_top) + static_cast<float>(maxFace.box_bottom - maxFace.box_top) / 3.0f) / 127.5f - 1.0f;
      eyes->setTargetPosition(targetX, targetY);
    } else if (personSensor.timeSinceFaceDetectedMs() > 1'000) {
      eyes->setAutoMove(true);
    }
  }

  eyes->renderFrame();
}

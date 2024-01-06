// Define if you wish to debug memory usage.  Only works on T4.x
//#define DEBUG_MEMORY

#include <SPI.h>
#include <array>
#include <Wire.h>
#include <Entropy.h>

#include "config.h"
#include "util/logging.h"
#include "sensors/LightSensor.h"
#include "sensors/PersonSensor.h"

#if defined(USE_AUDIO_MEMORY)
#include <Audio.h>
#endif

#ifdef USE_AUDIO_MEMORY
#ifndef AUDIO_PROGMEM
#define AUDIO_PROGMEM	PROGMEM
#endif

#if    !defined(USE_AUDIO_CACKLE)  && !defined(USE_AUDIO_CATSCREAM)	\
    && !defined(USE_AUDIO_GHOUL)   && !defined(USE_AUDIO_KNOCKING)	\
    && !defined(USE_AUDIO_SCREAM)  && !defined(USE_AUDIO_SCREAM2)	\
    && !defined(USE_AUDIO_SCREECH) && !defined(USE_AUDIO_THUNDER)

#if defined(ARDUINO_TEENSY41)
#define USE_AUDIO_SCREAM2	1	/*  7,457 unsigned ints.  */
#define USE_AUDIO_SCREAM	1	/*  7,041 unsigned ints.  */
#define USE_AUDIO_GHOUL		1	/* 10,513 unsigned ints.  */
#define USE_AUDIO_SCREECH	1	/* 19,665 unsigned ints.  */
#define USE_AUDIO_CACKLE	1	/* 23,105 unsigned ints.  */
#define USE_AUDIO_KNOCKING	0	/* 36,017 unsinged ints.  */
#define USE_AUDIO_CATSCREAM	0	/* 55,137 unsigned ints.  */
#define USE_AUDIO_THUNDER	0	/* 8,3745 unsigned ints.  */

#else	/* Teensy 4.0.  */
#define USE_AUDIO_SCREAM2	1	/*  7,457 unsigned ints.  */
#define USE_AUDIO_SCREAM	0	/*  7,041 unsigned ints.  */
#define USE_AUDIO_GHOUL		0	/* 10,513 unsigned ints.  */
#define USE_AUDIO_SCREECH	0	/* 19,665 unsigned ints.  */
#define USE_AUDIO_CACKLE	0	/* 23,105 unsigned ints.  */
#define USE_AUDIO_KNOCKING	0	/* 36,017 unsinged ints.  */
#define USE_AUDIO_CATSCREAM	0	/* 55,137 unsigned ints.  */
#define USE_AUDIO_THUNDER	0	/* 8,3745 unsigned ints.  */
#endif	/* ARDUINO_TEENSY41.  */
#endif	/* Use default sound list.  */

#if USE_AUDIO_SCREAM2
#include "AudioSampleScream2.h"
#endif

#if USE_AUDIO_SCREAM
#include "AudioSampleScream.h"
#endif

#if USE_AUDIO_GHOUL
#include "AudioSampleGhoul.h"
#endif

#if USE_AUDIO_SCREECH
#include "AudioSampleScreech.h"
#endif

#if USE_AUDIO_CACKLE
#include "AudioSampleCackle.h"
#endif

#if USE_AUDIO_KNOCKING
#include "AudioSampleKnocking.h"
#endif

#if USE_AUDIO_CATSCREAM
#include "AudioSampleCatscream.h"
#endif

#if USE_AUDIO_THUNDER
#include "AudioSamplenThunder.h"
#endif

static size_t sound_num = 0;

struct audio_sounds {
  const char		*const	name;		// name of the audio file
  const unsigned int	*const	sound;		// sound location
  const size_t			size;		// size of the sound file
};

const struct audio_sounds sounds[] = {

#if USE_AUDIO_SCREAM2
  { "scream2",		AudioSampleScream2,	sizeof (AudioSampleScream2)	},
#endif

#if USE_AUDIO_SCREAM
  { "scream",		AudioSampleScream,	sizeof (AudioSampleScream)	},
#endif

#if USE_AUDIO_GHOUL
  { "ghoul",		AudioSampleGhoul,	sizeof (AudioSampleGhoul)	},
#endif

#if USE_AUDIO_SCREECH
  { "screech",		AudioSampleScreech,	sizeof (AudioSampleScreech)	},
#endif

#if USE_AUDIO_CACKLE
  { "cackle",		AudioSampleCackle,	sizeof (AudioSampleCackle)	},
#endif

#if USE_AUDIO_KNOCKING
  { "knocking",		AudioSampleKnocking,	sizeof (AudioSampleKnocking)	},
#endif

#if USE_AUDIO_CATSCREAM
  { "catscream",	AudioSampleCatscream,	sizeof (AudioSampleCatscream)	},
#endif

#if USE_AUDIO_THUNDER
  { "thunder",		AudioSampleThunder,	sizeof (AudioSampleThunder)	},
#endif
};

const size_t max_sounds = sizeof (sounds) / sizeof (sounds[0]);

// GUItool: begin automatically generated code
AudioPlayMemory		playMem;		//xy=691,246
AudioOutputI2S		shield;			//xy=1081,255
AudioConnection		patchCord1 (playMem, 0, shield, 0);
AudioControlSGTL5000	sgtl5000;
// GUItool: end automatically generated code

#endif	/* USE_AUDIO.  */

// The index of the currently selected eye definitions
static uint32_t defIndex{0};

LightSensor lightSensor(LIGHT_PIN);
PersonSensor personSensor(Wire);
bool personSensorFound = USE_PERSON_SENSOR;

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
  return personSensorFound;
}

#ifndef ORIG_CODE
static void printEyeName ()
{
  auto &defs = eyeDefinitions.at(defIndex);
  const char *name = defs[0].name;

  if (!name || name[0] == '\0')
    name = "no name";

  Serial.printf ("Eye #%-2d %s\n", (int)defIndex, name);
}
#endif	/* meissner changes.  */

/// INITIALIZATION -- runs once at startup ----------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000);
  delay(200);
  DumpMemoryInfo();
  Serial.println("");
  Serial.println("==============================");
  Serial.println("");
  Serial.println("Init");
  Serial.flush();
  Entropy.Initialize();
  randomSeed(Entropy.random());

#ifndef ORIG_CODE
  Serial.printf("Eye duration = %d\n", (int)((EYE_DURATION_MS + 500) / 1000));

  if (hasLightSensor()) {
    Serial.printf("Light sensor pin is %d\n", LIGHT_PIN);
  }
#endif	/* meissner changes.  */

  if (hasBlinkButton()) {
    pinMode(BLINK_PIN, INPUT_PULLUP);

#ifndef ORIG_CODE
    Serial.printf("Blink pin is %d\n", BLINK_PIN);
#endif	/* meissner changes.  */
  }

  if (hasJoystick()) {
    pinMode(JOYSTICK_X_PIN, INPUT);
    pinMode(JOYSTICK_Y_PIN, INPUT);

#ifndef ORIG_CODE
    Serial.printf("Joystick pins are %d, %d\n", JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
#endif	/* meissner changes.  */
  }

  if (hasPersonSensor()) {
    Wire.begin();
    personSensorFound = personSensor.isPresent();
    if (personSensorFound) {
      Serial.println("Person Sensor detected");
      personSensor.enableID(false);
      // personSensor.enableLED(false);
      personSensor.setMode(PersonSensor::Mode::Continuous);
    } else {
      Serial.println("No Person Sensor was found!");
    }
  }

  initEyes(!hasJoystick(), !hasBlinkButton(), !hasLightSensor());

#ifndef ORIG_CODE

#ifdef USE_AUDIO_MEMORY
  Serial.printf("Sounds:\n");
  for (size_t i = 0; i < max_sounds; i++)
    Serial.printf ("    %-10s %8u\n", sounds[i].name, (unsigned long)sounds[i].size);
  Serial.printf ("\n");

  AudioMemory (8);
  sgtl5000.enable ();
  sgtl5000.volume (0.5f);
#endif

  Serial.println("");
  printEyeName();
#endif	/* meissner changes.  */
}

/* start meissner changes.  */
#ifdef USE_AUDIO_MEMORY
void loop_audio () {
  if (!playMem.isPlaying ()) {
    sound_num = Entropy.random (0, max_sounds - 1);
    Serial.printf ("Playing #%lu %s (%lu bytes)\n",
		   (unsigned long) sound_num,
		   sounds[sound_num].name,
		   (unsigned long) sounds[sound_num].size);

    playMem.play (sounds[sound_num].sound);
  }
}
#endif
/* end meissner changes.  */

void nextEye() {

#ifdef ORIG_CODE
  defIndex = (defIndex + 1) % eyeDefinitions.size();

#else	/* meissner changes.  */
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
      defIndex = Entropy.random (0, size - 1);
    } while (defIndex == oldIndex && ++loop_count < 4);
  }
#endif	/* meissner changes.  */

  eyes->updateDefinitions(eyeDefinitions.at(defIndex));

#ifndef ORIG_CODE
  printEyeName();
#endif	/* meissner changes.  */
}

/// MAIN LOOP -- runs continuously after setup() ----------------------------
void loop() {

#ifdef USE_AUDIO_MEMORY
  // Update sounds
  loop_audio ();
#endif

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
      if (face.is_facing && face.box_confidence > 60) {
        int size = (face.box_right - face.box_left) * (face.box_bottom - face.box_top);
        if (size > maxSize) {
          maxSize = size;
          maxFace = face;
        }
      }
    }

    if (maxSize > 0) {
      eyes->setAutoMove(false);
      float targetX = -((static_cast<float>(maxFace.box_left) + static_cast<float>(maxFace.box_right - maxFace.box_left) / 2.0f) / 127.5f - 1.0f);
      float targetY = (static_cast<float>(maxFace.box_top) + static_cast<float>(maxFace.box_bottom - maxFace.box_top) / 3.0f) / 127.5f - 1.0f;
      eyes->setTargetPosition(targetX, targetY);
    } else if (personSensor.timeSinceFaceDetectedMs() > 5'000 && !eyes->autoMoveEnabled()) {
      // We haven't seen a face for a while so enable automove
      eyes->setAutoMove(true);
    }
  }

  eyes->renderFrame();
}

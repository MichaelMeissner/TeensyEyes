#include <SPI.h>
#include <array>
#include <Wire.h>
#include <Entropy.h>
#include <SD.h>

#include "config-display.h"

#ifdef USE_AUDIO_MEMORY
#include "audio-memory.h"
#warning "Use audio memory"
#include <Audio.h>

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
static AudioPlayMemory		playMem;		//xy=691,246
static AudioOutputI2S		shield;			//xy=1081,255
static AudioConnection		patchCord1 (playMem, 0, shield, 0);
static AudioControlSGTL5000	sgtl5000;
// GUItool: end automatically generated code


// Loop support, play next song.

void loop_audio_memory ()
{
  if (!playMem.isPlaying ()) {
    sound_num = Entropy.random (0, max_sounds - 1);
    Serial.printf ("Playing #%lu %s (%lu bytes)\n",
		   (unsigned long) sound_num,
		   sounds[sound_num].name,
		   (unsigned long) sounds[sound_num].size);

    playMem.play (sounds[sound_num].sound);
  }
}


// Setup support

void setup_audio_memory ()
{
  Serial.printf("Sounds:\n");
  for (size_t i = 0; i < max_sounds; i++)
    Serial.printf ("    %-10s %8u\n", sounds[i].name, (unsigned long)sounds[i].size);

  Serial.printf ("\n");

  AudioMemory (8);
  sgtl5000.enable ();
  sgtl5000.volume (0.5f);
}

#endif	/* USE_AUDIO_MEMORY.  */

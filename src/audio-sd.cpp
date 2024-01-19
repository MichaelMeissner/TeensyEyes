// Inspiration from
// The Complete_MP3_AAC_MP4_M4A_example in the release:
// https://github.com/FrankBoesing/Arduino-Teensy-Codec-lib
// Example sketch by Dnstje.nl
//
// Also the SdCardTest from the Audio library.
//
// Simple MP3/AAC/WAV/RAW file player example
//
// Requires the audio shield:
//   http://www.pjrc.com/store/teensy3_audio.html
//   or dac / pwm / SD shield
//
// MP3/AAC library code by Frank Boesing.
//
// Modified for inclusion in Teensy Eyes by Michael Meissner


#define AUDIO_BOARD // if using audioboard 
//#define PWMDAC // if using DAC and PWM as output.  (dac ~ pin 14 = left ch, PWM ~ pin 3 ~ 4 + resistor dac right ch)

//#define USE_MP3
//#define USE_AAC

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Entropy.h>

#include "config-display.h"

#ifdef USE_AUDIO_SDCARD
#warning "Using audio-sdcard"
#include "audio-sd.h"

#if USE_MP3
#include <play_sd_mp3.h>		// mp3 decoder
#endif	/* USE_MP3.  */

#if USE_AAC
#include <play_sd_aac.h>		// AAC decoder
#endif	/* USE_AAC.  */

const int chipSelect = AUDIO_CS;	// if using another pin for SD card CS.
static File root;

#if USE_MP3
static AudioPlaySdMp3	mp3;
#endif	/* USE_MP3.  */

#if USE_AAC
static AudioPlaySdAac	aac;
#endif	/* USE_AAC.  */

static AudioPlaySdWav	wav;
static AudioPlaySdRaw	raw;

// Which player is playing (and channel to use on the mixer)
const int		PLAYER_MP3	=  0;
const int		PLAYER_AAC	=  1;
const int		PLAYER_WAV	=  2;
const int		PLAYER_RAW	=  3;

// Simplified class to handle various audio formats
class audio_common {
public:
  audio_common () {}
  ~audio_common () {}
  virtual bool play (const char *filename)	= 0;
  virtual bool isPlaying (void)			= 0;
  virtual const char *name (void)		= 0;
};

// Dummy subclass
class audio_dummy : public audio_common {
public:
  audio_dummy () : audio_common ()	{}
  ~audio_dummy ()			{}
  bool play (const char *filename)	{ return false; }
  bool isPlaying (void)			{ return false; }
  const char *name (void)		{ return "dummy"; }
};

#if USE_MP3
// MP3 subclass
class audio_mp3 : public audio_common {
public:
  audio_mp3 () : audio_common ()		{}
  ~audio_mp3 ()				{}
  bool play (const char *filename)	{ return mp3.play (filename); }
  bool isPlaying (void)			{ return mp3.isPlaying (); }
  const char *name (void)		{ return "mp3"; }
};
#endif	/* USE_MP3.  */

#if USE_AAC
// AAC subclass
class audio_aac : public audio_common {
public:
  audio_aac () : audio_common ()		{}
  ~audio_aac ()				{}
  bool play (const char *filename)	{ return aac.play (filename); }
  bool isPlaying (void)			{ return aac.isPlaying (); }
  const char *name (void)		{ return "aac"; }
};
#endif	/* USE_AAC.  */

// WAV subclass
class audio_wav : public audio_common {
public:
  audio_wav () : audio_common ()		{ wav.begin (); }
  ~audio_wav ()				{}
  bool play (const char *filename)	{ return wav.play (filename); }
  bool isPlaying (void)			{ return wav.isPlaying (); }
  const char *name (void)		{ return "wav"; }
};

// RAW subclass
class audio_raw : public audio_common {
private:
  AudioPlaySdRaw raw;

public:
  audio_raw () : audio_common ()		{ raw.begin (); }
  ~audio_raw ()				{}
  bool play (const char *filename)	{ return raw.play (filename); }
  bool isPlaying (void)			{ return raw.isPlaying (); }
  const char *name (void)		{ return "raw"; }
};
  
#ifdef PWMDAC
static AudioOutputAnalog	dac1;
static AudioOutputPWM		pwm1;
#endif

#ifdef AUDIO_BOARD
static AudioOutputI2S		i2s1;
#endif

static AudioMixer4		mixleft;
static AudioMixer4		mixright;

// Delays to use
const unsigned long		delay_after_start	=  250UL;
const unsigned long		delay_loop		= 9000UL;

// Audio common
#if USE_MP3
static class audio_mp3		mp3_player;
#endif	/* USE_MP3.  */

#if USE_AAC
static class audio_aac		aac_player;
#endif	/* USE_AAC.  */

static class audio_wav		wav_player;
static class audio_raw		raw_player;
static class audio_dummy		dummy_player;
static class audio_common	*audio = (class audio_common *) &dummy_player;

#if USE_MP3
// MP3
static AudioConnection		patch_mp3_l (mp3, 0, mixleft,  PLAYER_MP3);
static AudioConnection		patch_mp3_r (mp3, 1, mixright, PLAYER_MP3);
#endif	/* USE_MP3.  */

#if USE_AAC
// AAC
static AudioConnection		patch_aac_l (aac, 0, mixleft,  PLAYER_AAC);
static AudioConnection		patch_aac_r (aac, 1, mixright, PLAYER_AAC);
#endif

// WAV
static AudioConnection		patch_wav_l (wav, 0, mixleft,  PLAYER_WAV);
static AudioConnection		patch_wav_r (wav, 1, mixright, PLAYER_WAV);

// RAW.  Note RAW is mono only.
static AudioConnection		patch_raw_l (raw, 0, mixleft,  PLAYER_RAW);
static AudioConnection		patch_raw_r (raw, 0, mixright, PLAYER_RAW);

// Output devices
#ifdef PWMDAC
static AudioConnection		patch_pwmdac_l (mixleft,  0, dac1, 0);
static AudioConnection		patch_pwmdac_r (mixright, 0, pwm1, 0);
#endif

#ifdef AUDIO_BOARD
static AudioConnection		patch_audio_l (mixleft,  0, i2s1, 0);
static AudioConnection		patch_audio_r (mixright, 0, i2s1, 1);
static AudioControlSGTL5000	sgtl5000;
#endif

// Recorded file names
#ifndef MAX_FILES
#define MAX_FILES		32
#endif

#ifndef MAX_FILENAME_LEN
#define MAX_FILENAME_LEN	32
#endif

static char	filenames[MAX_FILES][MAX_FILENAME_LEN+1];
static size_t	num_files	= 0;


void setup_audio_sd ()
{
  Sd2Card card;
  SdVolume sd_card_size;

  while (!Serial && millis () < 3000UL)
    ;

  Serial.begin (115200);

  // Audio connections require memory to work.
  AudioMemory (16);

  // Setup the output device
#ifdef AUDIO_BOARD
  sgtl5000.enable ();
  sgtl5000.volume (0.5f);

#if !defined(__IMXRT1062__)
  Serial.println ("Reset MOSI and SCK");
  SPI.setMOSI (7);
  SPI.setSCK  (14);
#endif
#endif

  // Print SD information
  if (card.init (SPI_FULL_SPEED, chipSelect)) {
    Serial.println ("SD card is connected");

  } else {
    while (1) {
      Serial.println ("SD card is not connected");
      delay (delay_loop);
    }
  }

  int type = card.type();
  if (type == SD_CARD_TYPE_SD1 || type == SD_CARD_TYPE_SD2) {
    Serial.println ("Card type is SD");

  } else if (type == SD_CARD_TYPE_SDHC) {
    Serial.println ("Card type is SDHC");

  } else {

    Serial.println ("Card is an unknown type (maybe SDXC?)");
  }

  // Look at the file system and print its capacity
  if (!sd_card_size.init (card)) {
    while (1) {
      Serial.println("Unable to access the filesystem on this card. :-(");
      delay (delay_loop);
    }
  }

  float size = sd_card_size.blocksPerCluster() * sd_card_size.clusterCount();
  size = size * (512.0 / 1e6);			// convert blocks to millions of bytes
  Serial.printf ("File system space is %g Mbytes.\n", size);

  // Start SD card
  if (SD.begin (chipSelect)) {
    Serial.printf ("SD library is able to access the file system, CS = %d\n", chipSelect);

  } else {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println ("Unable to access the SD card");
      delay (delay_loop);
    }
  }

  // Read the files
  Serial.println ("Open SD root");
  root = SD.open ("/");
  File files;

  while (files = root.openNextFile ()) {
    const char *filename = files.name ();

    if (!filename) {
      Serial.printf ("Weird, no filename\n\n");

    } else {
      const char *extension = strrchr (filename, '.');

      if (extension
	  && num_files < MAX_FILES
	  && strlen (filename) <= MAX_FILENAME_LEN
	  && (strcmp (extension, ".WAV") == 0
	      || strcmp (extension, ".wav") == 0

#if USE_MP3
	      || strcmp (extension, ".MP3") == 0
	      || strcmp (extension, ".mp3") == 0
#endif	/* USE_MP3.  */

#if USE_AAC
	      || strcmp (extension, ".AAC") == 0
	      || strcmp (extension, ".aac") == 0
	      || strcmp (extension, ".MP4") == 0
	      || strcmp (extension, ".mp4") == 0
	      || strcmp (extension, ".M4A") == 0
	      || strcmp (extension, ".m4a") == 0
#endif	/* USE_AAC.  */

	      || strcmp (extension, ".RAW") == 0
	      || strcmp (extension, ".raw") == 0)) {

	strcpy (filenames[num_files], filename);
	Serial.printf ("# %2d, filename = %s\n", (int)num_files, filename);
	num_files++;
      }
    }
  }
}


// Set the mixer for a particular value
static void do_mixer (int channel, float gain)
{
  for (int i = 0; i < 4; i++)
    {
      float gain2 = (i == channel) ? gain : 0.0f;
      mixleft.gain  (i, gain2);
      mixright.gain (i, gain2);
    }
}


// Loop reading and playing a file
void loop_audio_sd ()
{
  // Did we find any files?
  if (!num_files) {
    return;
  }

  // Still playing?
  if (audio->isPlaying ()) {
    return;
  }

  audio = (class audio_common *) &dummy_player;

  int cur_file = Entropy.random (0, num_files - 1);
  const char *filename  = filenames[cur_file];
  const char *extension = strrchr (filename, '.');
  bool skip = false;

  if (!extension) {
    skip = true;

#if USE_MP3
  } else if (strcmp (extension, ".MP3") == 0
	     || strcmp (extension, ".mp3") == 0) {

    audio = (class audio_common *) &mp3_player;

    // Put the gain a bit lower, some MP3 files will clip otherwise.
    do_mixer (PLAYER_MP3, 0.9f);
#endif	/* USE_MP3.  */

#if USE_AAC
  } else if (strcmp (extension, ".AAC") == 0
	     || strcmp (extension, ".aac") == 0
	     || strcmp (extension, ".MP4") == 0
	     || strcmp (extension, ".mp4") == 0
	     || strcmp (extension, ".M4A") == 0
	     || strcmp (extension, ".m4a") == 0) {

    audio = (class audio_common *) &aac_player;
    do_mixer (PLAYER_AAC, 0.9f);
#endif	/* USE_AAC.  */

  } else if (strcmp (extension, ".WAV") == 0
	     || strcmp (extension, ".wav") == 0) {

    audio = (class audio_common *) &wav_player;
    do_mixer (PLAYER_WAV, 1.0f);

  } else if (strcmp (extension, ".RAW") == 0
	     || strcmp (extension, ".raw") == 0) {

    audio = (class audio_common *) &raw_player;
    do_mixer (PLAYER_RAW, 1.0f);

  } else {
    skip = true;
  }

  if (skip) {
    Serial.printf ("Skipping %s\n", filename);

  } else {
    bool ret = audio->play (filename);
    static unsigned long prev_ms = 0UL;
    unsigned long ms = millis ();

    Serial.printf ("%s.play (\"%s\"), #%d, returned %s, %lu ms\n",
		   audio->name (),
		   filename,
		   cur_file,
		   ret ? "true" : "false",
		   ms - prev_ms);

    prev_ms = ms;
    delay (delay_after_start);
  }
}
#endif	/* USE_AUDIO_SDCARD.  */

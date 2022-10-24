# TeensyEyes

### Introduction

This project is my take on "[Uncanny Eyes](https://github.com/adafruit/Uncanny_Eyes)", adapted for [Teensy 4.x](https://www.pjrc.com/store/teensy40.html)
microcontrollers. It renders animated eyes on round GC9A01A LCD screens. See the video below for an example of the eyes in action.

This is all still very much a work in progress. The aim is to turn this into a library that is fully configurable and
easily integrated with your own Teensy projects.

This codebase is based on code and ideas from the following projects:
  - https://github.com/adafruit/Uncanny_Eyes
  - https://github.com/adafruit/Adafruit_Learning_System_Guides/tree/main/M4_Eyes
  - https://github.com/mjs513/GC9A01A_t3n

### Building and Running

This project is developed with PlatformIO and (optionally) CLion.
Before you build and run it for the first time, you will need to first run the following from within the TeensyEyes directory:
```
mkdir lib
cd lib
git clone https://github.com/mjs513/GC9A01A_t3n
```
This will check out the [GC9A01A_t3n library](https://github.com/mjs513/GC9A01A_t3n), which is required to use GC9A01 LCD screens.

Running the "PlatformIO Upload | eyes" target from CLion, or alternatively running
```platformio run --target upload -e eyes```
will then compile the firmware and upload it to your Teensy 4.x.

### What does it Look Like?
Here's a video of the eyes in action:
<br/>

[![Teensy Eyes](http://img.youtube.com/vi/Ke1SJ8-6zJw/0.jpg)](https://www.youtube.com/watch?v=Ke1SJ8-6zJw "Teensy Eyes")

### Creating Your Own Eyes
Each type of eye is defined with a config.eye JSON file specifying various parameters of the eye, pupil, iris and
sclera, plus (optional) bitmaps that define the extents of the upper and lower eyelids, along with any textures for the
iris and sclera.

Detailed documentation for config.eye is coming, but in the meantime, if you want to try creating your own eyes
then copying and editing existing eyes in the `resources/eyes/240x240` directory is a good
starting point. Note that colors are currently only able to be specified as 5:6:5 RGB decimal
or hex values. Try [this](http://greekgeeks.net/#maker-tools_convertColor) for help in creating
your own 5:6:5 color codes.

Once you have an eye definition that you want to try, you need to generate code from it for
use in your firmware.

To generate the data files for a single eye, run the following from the `resources/eyes/240x240` directory:
```shell
python tablegen.py <output dir> path/to/your/config.eye
```
To generate _all_ eyes, run the `genall.py` command shown below. Note that you may want to make sure the output directory is empty first, so you don't end
up with redundant data files in there if your eye's configurations have changed.
```shell
python genall.py <output dir>
```

To use your newly created eye, include it with `#include "<path/to/eyename.h"` and access it in your
code using `eyename::eye` (or `eyename::left` and `eyename::right` if your eye has different parameters
for the left and right eyes).
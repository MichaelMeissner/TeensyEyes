// This forces the display driver to be built in the current directory.
// Arduino does not build *.cpp files in a sub-directory.

#ifdef ARDUINO
#include "config-display.h"

#ifdef USE_GC9A01A
#include "displays/GC9A01A_Display.h"
#include "displays/GC9A01A_Display.cpp"
#endif	/* USE_GC9A01A.  */
#endif	/* ARDUINO.  */

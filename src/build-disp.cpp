// These are the various tables used by the 240x240 eye patterns.  We need to
// include them here so that they get built under the Arduino rules (which
// typically does not build *.cpp files in a sub-directory).

#ifdef ARDUINO
#include "eyes/240x240/disp_240_120.h"
#include "eyes/240x240/disp_240_120.cpp"

#include "eyes/240x240/disp_240_125.h"
#include "eyes/240x240/disp_240_125.cpp"

#include "eyes/240x240/disp_240_130.h"
#include "eyes/240x240/disp_240_130.cpp"
#endif

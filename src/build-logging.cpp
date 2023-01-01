// This forces the memory logging facility to be built in the current
// directory.  Arduino does not build *.cpp files in a sub-directory.

#ifdef ARDUINO
#include "util/logging.h"
#include "util/logging.cpp"
#endif

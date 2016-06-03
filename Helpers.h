// Helpers.h

#ifndef _HELPERS_h
#define _HELPERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "i2c_t3.h"

class Helpers
{
 protected:


 public:
	 static void utils_scanI2C(i2c_t3 * wire);
	 static void print_scan_status(uint8_t target, bool all);
};

#endif


// Helpers.h

#ifndef _HELPERS_h
#define _HELPERS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class HelpersClass
{
 protected:


 public:
	 static void utils_scanI2C(Wire * wire);
	 static void print_scan_status(uint8_t target, bool all);
};

extern HelpersClass Helpers;

#endif


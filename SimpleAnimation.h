#ifndef _SIMPLEANIMATION_h
#define _SIMPLEANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class SimpleAnimation : public Animation
{
 protected:
	 int _lrCurrentPosition = 0;
	 int _rrCurrentPosition = 0;
	 int _mbCurrentPosition = 0;
	 int step(Adafruit_NeoPixel * pixels, int position);

 public:
	 void step();
};

#endif


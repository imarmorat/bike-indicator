// Animation.h
#ifndef _ANIMATION_h
#define _ANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class SimpleAnimation
{
 protected:
	 int _currentPosition = 0;
	 int _nbLeds = 16;

 public:
	void init();
	void step(Adafruit_NeoPixel * pixels);
};

#endif


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
	 Adafruit_NeoPixel *_leftRing;
	 Adafruit_NeoPixel *_rightRing;
	 Adafruit_NeoPixel *_middleBar;
	 void step(Adafruit_NeoPixel * pixels);

 public:
	 void init(Adafruit_NeoPixel *leftRing, Adafruit_NeoPixel *middleBar, Adafruit_NeoPixel *rightRing);
	 void step();
};

#endif


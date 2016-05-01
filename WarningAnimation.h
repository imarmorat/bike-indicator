#ifndef _WARNINGANIMATION_h
#define _WARNINGANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class WarningAnimation
{
protected:
	Adafruit_NeoPixel *_leftRing;
	Adafruit_NeoPixel *_rightRing;
	Adafruit_NeoPixel *_middleBar;

public:
	void init(Adafruit_NeoPixel *leftRing, Adafruit_NeoPixel *middleBar, Adafruit_NeoPixel *rightRing);
	void step();
};

#endif


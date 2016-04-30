#ifndef _TURNLEFTANIMATION_h
#define _TURNLEFTANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class TurnLeftAnimation
{
protected:
	int _currentPosition = 0;
	int _nbLeds = 16;
	Adafruit_NeoPixel *_leftRing;
	Adafruit_NeoPixel *_rightRing;
	Adafruit_NeoPixel *_middleBar;

public:
	void init(Adafruit_NeoPixel *leftRing, Adafruit_NeoPixel *middleBar, Adafruit_NeoPixel *rightRing);
	void reset();
	void step();
};

#endif


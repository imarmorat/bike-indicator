#ifndef _BREAKINGANIMATION_h
#define _BREAKINGANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class BreakingAnimation
{
 protected:
	 Adafruit_NeoPixel *_leftRing;
	 Adafruit_NeoPixel *_rightRing;
	 Adafruit_NeoPixel *_middleBar;
	 double _min;
	 double _max;

 public:
	 void init(Adafruit_NeoPixel *leftRing, Adafruit_NeoPixel *middleBar, Adafruit_NeoPixel *rightRing);
	 void step(double breakingLevel);
	 void setLimits(double min, double max);
	 bool isWithinLimit(double val);
	 bool isHigherThanMinLimit(double val);
};

#endif


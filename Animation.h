#ifndef _ANIMATION_h
#define _ANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class Animation
{
 protected:
	 Adafruit_NeoPixel *_leftRing;
	 Adafruit_NeoPixel *_rightRing;
	 Adafruit_NeoPixel *_middleBar;

 public:
	 void init(Adafruit_NeoPixel *leftRing, Adafruit_NeoPixel *middleBar, Adafruit_NeoPixel *rightRing)
	 {
		 _leftRing = leftRing;
		 _rightRing = rightRing;
		 _middleBar = middleBar;
	 }

	virtual void reset() 
	{
		_leftRing->clear();
		_rightRing->clear();
		_middleBar->clear();
	
		_leftRing->show();
		_rightRing->show();
		_middleBar->show();
	};

	virtual void step() {};
};

#endif


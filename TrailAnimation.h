// TrailAnimation.h

#ifndef _TRAILANIMATION_h
#define _TRAILANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TrailAnimation : public Animation
{
protected:
	int _trailLength = 8;
	int _startColorR, _startColorG, _startColorB;
	int _endColorR, _endColorG, _endColorB;
	int _trailHeadPosition;
	void internalInit();
	void setPixel(int position, uint32_t color);

public:
	void step();
	void reset();
};


#endif


#ifndef _TURNLEFTANIMATION_h
#define _TURNLEFTANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TurnAnimation : public Animation
{
protected:
	int _currGradStep = 0;
	int _direction = 0; // 0 = left, 1 = right
	int _currStep = 0;
	unsigned long _lastMillis = 0;
	float _redGrad, _greenGrad, _blueGrad;

public:
	void setDirection(int direction);
	void step();
};

#endif


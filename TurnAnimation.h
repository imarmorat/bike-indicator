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
	int _currentPosition = 0;
	int _direction = 0; // 0 = left, 1 = right

public:
	void setDirection(int direction);
	void step();
};

#endif


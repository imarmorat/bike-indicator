#ifndef _WARNINGANIMATION_h
#define _WARNINGANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class WarningAnimation : public Animation
{
protected:

public:
	void step();
};

#endif


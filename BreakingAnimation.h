#ifndef _BREAKINGANIMATION_h
#define _BREAKINGANIMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class BreakingAnimation : public Animation
{
 public:
	 void step();
};

#endif


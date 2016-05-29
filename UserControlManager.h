// UserControlManager.h

#ifndef _USERCONTROLMANAGER_h
#define _USERCONTROLMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ADS1x15.h"

enum UserCtrl_Input
{
	UserCtrl_left = 1,
	UserCtrl_right = 2,
	UserCtrl_top = 4,
	UserCtrl_bottom = 8,
	UserCtrl_click = 16,
	UserCtrl_none = 0
} ;

class UserControlManagerClass
{
 protected:
	 Adafruit_ADS1115 * _ads;

 public:
	void init(Adafruit_ADS1115 * ads);
	UserCtrl_Input getInput();
};


#endif


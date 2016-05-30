#include "UserControlManager.h"

#define HIGH_THRESHOLD	24000
#define LOW_THRESHOLD	150
#define CLICK_THRESHOLD 2

void UserControlManagerClass::init(Adafruit_ADS1115 * ads)
{
	_ads = ads;
	_ads->begin();
}

UserCtrl_Input UserControlManagerClass::getInput()
{
	int16_t adc0, adc1, adc2, adc3;

	adc0 = _ads->readADC_SingleEnded(0);
	adc1 = _ads->readADC_SingleEnded(1);
	adc2 = _ads->readADC_SingleEnded(2);

	if (adc0 >= HIGH_THRESHOLD)
		return UserCtrl_right;

	if (adc0 <= LOW_THRESHOLD)
		return UserCtrl_left;

	if (adc1 >= HIGH_THRESHOLD)
		return UserCtrl_top;

	if (adc1 <= LOW_THRESHOLD)
		return UserCtrl_bottom;

	if (adc2 <= CLICK_THRESHOLD)
		return UserCtrl_click;

	return UserCtrl_none;
}



#include "UserControlManager.h"

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
	adc3 = _ads->readADC_SingleEnded(3);

	return UserCtrl_none;
}



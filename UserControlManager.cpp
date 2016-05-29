#include "UserControlManager.h"

void UserControlManagerClass::init(Adafruit_ADS1115 * ads)
{
	_ads = ads;
}

UserCtrl_Input UserControlManagerClass::getInput()
{
	int16_t adc0, adc1, adc2, adc3;

	adc0 = ads.readADC_SingleEnded(0);
	adc1 = ads.readADC_SingleEnded(1);
	adc2 = ads.readADC_SingleEnded(2);
	adc3 = ads.readADC_SingleEnded(3);

	return UserCtrl_none;
}



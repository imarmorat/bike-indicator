#include "Animation.h"
#include "WarningAnimation.h"

void WarningAnimation::step()
{
	uint32_t warningColor = _middleBar->Color(128, 255, 6);

	//
	// switch on or off the whole leds display
	if (_stepCount == 0)
	{
		if (_switchOn)
		{
			for (int i = 0; i < _middleBar->numPixels(); i++)
				_middleBar->setPixelColor(i, warningColor);

			for (int i = 0; i < _leftRing->numPixels(); i++)
				_leftRing->setPixelColor(i, warningColor);

			for (int i = 0; i < _rightRing->numPixels(); i++)
				_rightRing->setPixelColor(i, warningColor);

			_switchOn = false;
		}
		else
		{
			_middleBar->clear();
			_leftRing->clear();
			_rightRing->clear();
			_switchOn = true;
		}

		_middleBar->show();
		_leftRing->show();
		_rightRing->show();
		
		// start delay
		_lastMillis = millis();
		_stepCount++;
	}

	// 
	// check delay reached
	if (_stepCount == 1)
	{
		unsigned long currentMillis = millis();
		if (currentMillis - _lastMillis >= 800)
			_stepCount = 0;
	}
}

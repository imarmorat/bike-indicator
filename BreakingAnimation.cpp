#include "Animation.h"
#include "BreakingAnimation.h"

void BreakingAnimation::step()
{
	//double percent = breakingLevel > _max ?
	//	1.0 :
	//	breakingLevel < _min ? 0.0 : (breakingLevel - _min) / (_max - _min);
	//Serial.println(percent);
	double percent = 1.0;
	int nbSteps = (int)(8.0); // 8leds = 100%
	//int nbSteps = (int)(8.0 * percent); // 8leds = 100%

	for (int i = 0; i < nbSteps; i++)
	{
		_middleBar->setPixelColor(i, _middleBar->Color(0, 255, 0));
		_middleBar->setPixelColor(15 - i, _middleBar->Color(0, 255, 0));
	}
	for (int i = nbSteps; i < 8; i++)
	{
		_middleBar->setPixelColor(i, _middleBar->Color(0, 0, 0));
		_middleBar->setPixelColor(15 - i, _middleBar->Color(0, 0, 0));
	}
	_middleBar->show();

	// if we hit the 100% mark, we light up both rings otherwise we clear them
	for (int i = 0; i < 16; i++)
	{
		_leftRing->setPixelColor(i, percent == 1.0 ? _leftRing->Color(0, 255, 0) : _leftRing->Color(0, 0, 0));
		_rightRing->setPixelColor(i, percent == 1.0 ? _rightRing->Color(0, 255, 0) : _rightRing->Color(0, 0, 0));
	}

	_leftRing->show();
	_rightRing->show();
}

#include "BreakingAnimation.h"

void BreakingAnimation::init(Adafruit_NeoPixel * leftRing, Adafruit_NeoPixel * middleBar, Adafruit_NeoPixel * rightRing)
{
	_leftRing = leftRing;
	_rightRing = rightRing;
	_middleBar = middleBar;
}

void BreakingAnimation::setLimits(double min, double max)
{
	_min = min; 
	_max = max;
}

bool BreakingAnimation::isWithinLimit(double val)
{
	return val >= _min && val <= _max;
}

bool BreakingAnimation::isHigherThanMinLimit(double val)
{
	return val > _min;
}

void BreakingAnimation::step(double breakingLevel)
{
	double percent = breakingLevel > _max ?
		1.0 :
		breakingLevel < _min ? 0.0 : (breakingLevel - _min) / (_max - _min);
	Serial.println(percent);
	int nbSteps = (int)(8.0 * percent); // 8leds = 100%

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

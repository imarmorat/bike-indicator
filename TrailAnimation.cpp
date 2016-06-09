#include "Animation.h"
#include "TrailAnimation.h"

void TrailAnimation::internalInit()
{
	_startColorR = 0;
	_startColorG = 0;
	_startColorB = 255;
	_endColorR = 0;
	_endColorG = 0;
	_endColorB = 10;
}

void TrailAnimation::reset()
{
	_trailHeadPosition = 0;
}

void TrailAnimation::step()
{
	int stepR = (_startColorR - _endColorR) / (_trailLength - 1);
	int stepG = (_startColorG - _endColorG) / (_trailLength - 1);
	int stepB = (_startColorB - _endColorB) / (_trailLength - 1);

	for (int i = _trailLength - 1; i >= 0; i--)
		setPixel((_trailHeadPosition + i) % 48, Adafruit_NeoPixel::Color(
			_endColorR + i * stepR,
			_endColorG + i * stepG,
			_endColorB + i * stepB));

	setPixel((_trailHeadPosition - 1) % 48, 0);

	_trailHeadPosition++;

	_leftRing->show();
	_rightRing->show();
	_middleBar->show();
}

void TrailAnimation::setPixel(int position, uint32_t color)
{
	Adafruit_NeoPixel * device = NULL;
	int posOnDevice = 0;

	int rightRingStartShift = 1;
	int leftRingStartShift = 9;

	if (position >= 0 && position <= 15)
	{
		device = _rightRing;
		posOnDevice = (position + rightRingStartShift) % 16;
	}

	if (position >= 16 && position <= 23)
	{
		device = _middleBar;
		posOnDevice = position - 16;
	}

	if (position >= 24 && position <= 39)
	{
		device = _leftRing;
		posOnDevice = ((position - 24) + leftRingStartShift) % 16;
	}

	if (position >= 40 && position <= 47)
	{
		device = _middleBar;
		posOnDevice = position - 40 + 8;
	}

	if (device != NULL)
	{
		device->setPixelColor(posOnDevice, color);
	}
}


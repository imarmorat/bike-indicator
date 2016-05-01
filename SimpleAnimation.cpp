#include "SimpleAnimation.h"

void SimpleAnimation::init(Adafruit_NeoPixel * leftRing, Adafruit_NeoPixel * middleBar, Adafruit_NeoPixel * rightRing)
{
	_leftRing = leftRing;
	_rightRing = rightRing;
	_middleBar = middleBar;
}

void SimpleAnimation::step()
{
	step(_leftRing);
	step(_middleBar);
	step(_rightRing);
}


void SimpleAnimation::step(Adafruit_NeoPixel * pixels)
{
	int previous = _currentPosition - 1 < 0 ? _nbLeds - 1 : _currentPosition - 1;
	pixels->setPixelColor(previous, pixels->Color(0, 0, 0));
	pixels->setPixelColor(_currentPosition, pixels->Color(255, 255, 255));
	_currentPosition = _currentPosition + 1 >= _nbLeds ? 0 : _currentPosition + 1;
	pixels->show();
}


#include "SimpleAnimation.h"

void SimpleAnimation::init(Adafruit_NeoPixel * leftRing, Adafruit_NeoPixel * middleBar, Adafruit_NeoPixel * rightRing)
{
	_leftRing = leftRing;
	_rightRing = rightRing;
	_middleBar = middleBar;
}

void SimpleAnimation::step()
{
	_lrCurrentPosition = step(_leftRing, _lrCurrentPosition);
	_rrCurrentPosition = step(_rightRing, _rrCurrentPosition);
	_mbCurrentPosition = step(_middleBar, _mbCurrentPosition);
}


int SimpleAnimation::step(Adafruit_NeoPixel * pixels, int position)
{
	int previous = position - 1 < 0 ? _nbLeds - 1 : position - 1;
	pixels->setPixelColor(previous, pixels->Color(0, 0, 0));
	pixels->setPixelColor(position, pixels->Color(255, 255, 255));
	pixels->show();
	return position + 1 >= _nbLeds ? 0 : position + 1;
}


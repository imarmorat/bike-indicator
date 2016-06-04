#include "Animation.h"
#include "SimpleAnimation.h"

void SimpleAnimation::step()
{
	_lrCurrentPosition = step(_leftRing, _lrCurrentPosition);
	_rrCurrentPosition = step(_rightRing, _rrCurrentPosition);
	_mbCurrentPosition = step(_middleBar, _mbCurrentPosition);
}

int SimpleAnimation::step(Adafruit_NeoPixel * pixels, int position)
{
	pixels->clear();
	pixels->setPixelColor(position, pixels->Color(255, 255, 255));
	pixels->show();
	return position + 1 >= pixels->numPixels() ? 0 : position + 1;
}


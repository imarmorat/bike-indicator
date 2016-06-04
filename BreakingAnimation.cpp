#include "Animation.h"
#include "BreakingAnimation.h"

void BreakingAnimation::step()
{
	uint32_t breakColor = _middleBar->Color(0, 255, 0);

	for (int i = 0; i < _middleBar->numPixels(); i++)
		_middleBar->setPixelColor(i, breakColor);

	for (int i = 0; i < _leftRing->numPixels(); i++)
		_leftRing->setPixelColor(i, breakColor);

	for (int i = 0; i < _rightRing->numPixels(); i++)
		_rightRing->setPixelColor(i, breakColor);

	_middleBar->show();
	_leftRing->show();
	_rightRing->show();
}

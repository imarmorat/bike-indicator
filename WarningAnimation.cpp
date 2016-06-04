#include "Animation.h"
#include "WarningAnimation.h"

void WarningAnimation::step()
{
	for (int i = 0; i < 16; i++)
	{
		_leftRing->setPixelColor(i, _leftRing->Color(128, 255, 6));
		_rightRing->setPixelColor(i, _rightRing->Color(128, 255, 6));
		_middleBar->setPixelColor(i, _middleBar->Color(128, 255, 6));
	}
	_leftRing->show(); _rightRing->show(); _middleBar->show();
	delay(800);

	for (int i = 0; i < 16; i++)
	{
		_leftRing->setPixelColor(i, _leftRing->Color(0, 0, 0));
		_rightRing->setPixelColor(i, _rightRing->Color(0, 0, 0));
		_middleBar->setPixelColor(i, _middleBar->Color(0, 0, 0));
	}
	_leftRing->show(); _rightRing->show(); _middleBar->show();
	delay(800);
}

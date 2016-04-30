#include "TurnLeftAnimation.h"
#include <Adafruit_NeoPixel.h>

void TurnLeftAnimation::init(Adafruit_NeoPixel * leftRing, Adafruit_NeoPixel * middleBar, Adafruit_NeoPixel * rightRing)
{
	_leftRing = leftRing;
	_rightRing = rightRing;
	_middleBar = middleBar;
}

void TurnLeftAnimation::step()
{
	int rs = 6, bs = 6, gs = 6;
	int re = 128, ge = 255, be = 6;
	float rstep = (re - rs) / 8;
	float bstep = (be - bs) / 8;
	float gstep = (ge - gs) / 8;

	int barPos = 0;
	for (int i = 0; i < 8; i++)
	{
		_middleBar->setPixelColor(i, _middleBar->Color(rs, gs, bs));
		_middleBar->setPixelColor(15 - i, _middleBar->Color(rs, gs, bs));
		_middleBar->show();
		delay(50);
		rs += rstep; gs += gstep; bs += bstep;
	}

	for (int i = 0; i<16; i++) 
	{ 
		_leftRing->setPixelColor(i, _leftRing->Color(128, 255, 6));
	}
	_leftRing->show();
	delay(1000);
	reset();
}

void TurnLeftAnimation::reset()
{
	_leftRing->clear();
	_rightRing->clear();
	_middleBar->clear();
	_leftRing->show();
	_rightRing->show();
	_middleBar->show();
}
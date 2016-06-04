#include "Animation.h"
#include "TurnAnimation.h"
#include <Adafruit_NeoPixel.h>

void TurnAnimation::setDirection(int direction)
{
	_direction = direction;
}

void TurnAnimation::step()
{
	int rs = 6, bs = 6, gs = 6;
	int re = 128, ge = 255, be = 6;
	float rstep = (re - rs) / 8;
	float bstep = (be - bs) / 8;
	float gstep = (ge - gs) / 8;
	
	for (int i = 0; i<16; i++)
	{
		_leftRing->setPixelColor(i, _leftRing->Color(0,0,0)); 
		_rightRing->setPixelColor(i, _rightRing->Color(0,0,0)); 
	}
	_leftRing->show();
	_rightRing->show();

	/*
		Step 1
	*/
	for (int i = 0; i < 8; i++)
	{
		switch (_direction)
		{
		case 0:
			_middleBar->setPixelColor(i, _middleBar->Color(rs, gs, bs));
			_middleBar->setPixelColor(15 - i, _middleBar->Color(rs, gs, bs));
			break;
		case 1:
			_middleBar->setPixelColor(7 - i, _middleBar->Color(rs, gs, bs));
			_middleBar->setPixelColor(8 + i, _middleBar->Color(rs, gs, bs));
			break;
		}

		_middleBar->show();
		delay(25);
		rs += rstep; gs += gstep; bs += bstep;
	}

	/*
		Step 2
	*/
	for (int i = 0; i<16; i++) 
	{ 
		switch (_direction)
		{
		case 0:_leftRing->setPixelColor(i, _leftRing->Color(128, 255, 6)); break;
		case 1:_rightRing->setPixelColor(i, _rightRing->Color(128, 255, 6)); break;
		}
	}

	switch (_direction)
	{
	case 0:_leftRing->show(); break;
	case 1:_rightRing->show(); break;
	}
	delay(500);


	/*
		Step 3
	*/
	for (int i = 0; i < 8; i++)
	{
		switch (_direction)
		{
		case 0:
			_middleBar->setPixelColor(i, _middleBar->Color(0, 0, 0));
			_middleBar->setPixelColor(15 - i, _middleBar->Color(0, 0, 0));
			break;
		case 1:
			_middleBar->setPixelColor(7 - i, _middleBar->Color(0, 0, 0));
			_middleBar->setPixelColor(8 + i, _middleBar->Color(0, 0, 0));
			break;
		}

		_middleBar->show();
		delay(25);
		rs += rstep; gs += gstep; bs += bstep;
	}
	delay(500);

	reset();
}

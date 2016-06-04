#include "Animation.h"
#include "TurnAnimation.h"
#include <Adafruit_NeoPixel.h>

void TurnAnimation::setDirection(int direction)
{
	_direction = direction;
}

void TurnAnimation::step()
{
	//uint32_t _finalColor = Adafruit_NeoPixel::Color(128, 255, 6);
	//uint32_t _initialColor = Adafruit_NeoPixel::Color(6, 6, 6);

	int rs = 6, bs = 6, gs = 6;
	int re = 128, ge = 255, be = 6;
	float rstep = (re - rs) / 8;
	float bstep = (be - bs) / 8;
	float gstep = (ge - gs) / 8;
	int gradientSteps = 8;

	//
	// first step is the gradient display on middle bar
	if (_currStep == 0)
	{
		auto redGrad = rs + _currGradStep * (re - rs) / (gradientSteps - 1);
		auto greenGrad = gs + _currGradStep * (ge - gs) / (gradientSteps - 1);
		auto blueGrad = bs + _currGradStep * (be - bs) / (gradientSteps - 1);

		switch (_direction)
		{
		case 0:
			_middleBar->setPixelColor(_currGradStep, _middleBar->Color(redGrad, greenGrad, blueGrad));
			_middleBar->setPixelColor(15 - _currGradStep, _middleBar->Color(redGrad, greenGrad, blueGrad));
			break;
		case 1:
			_middleBar->setPixelColor(7 - _currGradStep, _middleBar->Color(redGrad, greenGrad, blueGrad));
			_middleBar->setPixelColor(8 + _currGradStep, _middleBar->Color(redGrad, greenGrad, blueGrad));
			break;
		}
		_middleBar->show();

		if (_currGradStep++ >= 8)
		{
			_currStep++;
			_currGradStep = 0;
		}
	}
	
	//
	// light up the correct ring depending on the direction
	if (_currStep == 1)
	{
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

		_lastMillis = millis();
		_currStep++;		
	}

	//
	// wait a bit
	if (_currStep == 2)
	{
		unsigned long currentMillis = millis();
		if (currentMillis - _lastMillis >= 1000)
			_currStep++;
	}

	if (_currStep == 3)
	{
		switch (_direction)
		{
		case 0:
			_middleBar->setPixelColor(_currGradStep, 0);
			_middleBar->setPixelColor(15 - _currGradStep, 0);
			break;
		case 1:
			_middleBar->setPixelColor(7 - _currGradStep, 0);
			_middleBar->setPixelColor(8 + _currGradStep, 0);
			break;
		}

		if (_currGradStep++ >= 8)
		{
			_currGradStep = 0;
			_currStep++;
		}
	}

	//
	// final step, clear all
	if (_currStep == 3)
	{
		for (int i = 0; i<16; i++)
		{
			switch (_direction)
			{
			case 0:_leftRing->setPixelColor(i, 0); break;
			case 1:_rightRing->setPixelColor(i, 0); break;
			}
		}

		switch (_direction)
		{
		case 0:_leftRing->show(); break;
		case 1:_rightRing->show(); break;
		}

		_currStep = 0;
	}
}

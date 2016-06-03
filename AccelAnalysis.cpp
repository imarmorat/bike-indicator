#include "AccelAnalysis.h"

void AccelAnalysis::init(Adafruit_ADXL345_Unified * adx345, uint32_t frequency, double multiplier, double min, double max)
{
	_adx345 = adx345;
	_frequency = frequency;
	_multiplier = multiplier;
	_latest = 0;
	_min = min;
	_max = max;
}

void AccelAnalysis::update()
{
	//Old code
	//sensors_event_t event;
	//accel.getEvent(&event);
	//float calcdG = sqrt(
	//	event.acceleration.x*event.acceleration.x +
	//	event.acceleration.y*event.acceleration.y +
	//	event.acceleration.z*event.acceleration.z -
	//	SENSORS_GRAVITY_STANDARD * SENSORS_GRAVITY_STANDARD
	//);

	/* Display the results (acceleration is measured in m/s^2) */
	//Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
	//Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
	//Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" -- ");
	//Serial.print("G: "); Serial.print(calcdG); Serial.println(" g");

	double sample = _adx345->getX() * ADXL345_MG2G_MULTIPLIER;
	_latest = (1 - _multiplier) * _latest + _multiplier * sample;
}

void AccelAnalysis::setLimits(double min, double max)
{
	_min = min;
	_max = max;
}

bool AccelAnalysis::isWithinLimit(double val)
{
	return val >= _min && val <= _max;
}

bool AccelAnalysis::isHigherThanMinLimit(double val)
{
	return val > _min;
}


double AccelAnalysis::getLatest()
{
	return _latest;
}

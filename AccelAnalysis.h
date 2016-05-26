#ifndef _ACCELANALYSIS_h
#define _ACCELANALYSIS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ADXL345.h"

class AccelAnalysis
{
 protected:
	 uint32_t _frequency;
	 double _multiplier;
	 double _latest;
	 Adafruit_ADXL345_Unified * _adx345;
 public:
	void init(Adafruit_ADXL345_Unified * adx345, uint32_t frequency, double multiplier);
	void update();
	double getLatest();
};

#endif


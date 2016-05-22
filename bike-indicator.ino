#include "LiPoFuel.h"
#include "AccelAnalysis.h"
#include "WarningAnimation.h"
#include "BreakingAnimation.h"
#include "TurnAnimation.h"
#include "SimpleAnimation.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include "defs.h"

// https://github.com/adafruit/Adafruit_ADXL345
// https://github.com/adafruit/Adafruit_Sensor

int led = 13;
#define LEFTRING_PIN 0
#define RIGHTRING_PIN 2
#define BAR_PIN 1
#define BREAKING_BUTTON_PIN 4
#define TURNLEFT_BUTTON_PIN 5
#define TURNRIGHT_BUTTON_PIN 6
uint8_t offset = 0;

Adafruit_NeoPixel leftRingPixels = Adafruit_NeoPixel(16, LEFTRING_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel rightRingPixels = Adafruit_NeoPixel(16, RIGHTRING_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel middleBarsPixels = Adafruit_NeoPixel(16, BAR_PIN, NEO_RGBW + NEO_KHZ800);
uint32_t blinkerColor = leftRingPixels.Color(128, 255, 6);
uint32_t breakColor = leftRingPixels.Color(0, 255, 0); // green red blue format

SimpleAnimation simpleAnim;
TurnAnimation turnAnim;
BreakingAnimation breakAnim;
WarningAnimation warningAnim;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
AccelAnalysis accelAnalysis;
volatile boolean alert = false;
void lowPower() { alert = true; }
LiFuelGauge gauge(MAX17043, 0, lowPower);
bool useAccel = true;

Mode currentMode = Mode_none;

void ChangeMode(Mode newMode)
{
	if ((currentMode == Mode_blinkRight && newMode == Mode_blinkLeft) || (currentMode == Mode_blinkLeft && newMode == Mode_blinkRight))
	{
		currentMode = Mode_warning;
		return;
	}

	if (currentMode == newMode)
	{
		currentMode = Mode_none;
		return;
	}

	currentMode = newMode;
}

void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, true);
	Serial.begin(38400);
	Serial.println("helloo");
	pinMode(BREAKING_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNLEFT_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNRIGHT_BUTTON_PIN, INPUT_PULLUP);

	leftRingPixels.begin();
	leftRingPixels.setBrightness(100); 
	rightRingPixels.begin();
	rightRingPixels.setBrightness(100); 
	middleBarsPixels.begin();
	middleBarsPixels.setBrightness(100);

	leftRingPixels.clear();
	rightRingPixels.clear();
	middleBarsPixels.clear();
	leftRingPixels.show();
	rightRingPixels.show();
	middleBarsPixels.show();

	attachInterrupt(BREAKING_BUTTON_PIN, breakButtonActivated, FALLING); 
	attachInterrupt(TURNLEFT_BUTTON_PIN, turnLeftButtonActivated, FALLING); 
	attachInterrupt(TURNRIGHT_BUTTON_PIN, turnRightButtonActivated, FALLING); 

	turnAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	simpleAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	warningAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	breakAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	breakAnim.setLimits(2.0, 6.0);

	accelAnalysis.init(&accel, 0, 0.3);
	digitalWrite(led, false);
	/* Initialise the sensor */
	if (useAccel)
	{
		if (!accel.begin())
		{
			/* There was a problem detecting the ADXL345 ... check your connections */
			Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
			while (1);
		}
		else
			Serial.println("ADX345 init'd ok");
	}

	gauge.reset();
	gauge.setAlertThreshold(10);
	Serial.println(String("Alert Threshold is set to ") + gauge.getAlertThreshold() + '%');
}

void breakButtonActivated()
{
	cli();
	Serial.println("Breaking");
	ChangeMode(Mode_manualBreaking);
	sei();
}

void turnLeftButtonActivated()
{
	cli();
	Serial.println("Turning left");
	ChangeMode(Mode_blinkLeft);
	sei();
}

void turnRightButtonActivated()
{
	cli();
	Serial.println("Turning right");
	ChangeMode(Mode_blinkRight);
	sei();
}

// the loop routine runs over and over again forever:
void loop() {

	Serial.print("Charge: ");
	Serial.print(gauge.getSOC());  // Gets the battery's state of charge
	Serial.print("%, VBattery: ");
	Serial.print(gauge.getVoltage());  // Gets the battery voltage
	Serial.println('V');

	if (alert)
	{
		Serial.println("Beware, Low Power!");
		Serial.println("Stopping");
		gauge.clearAlertInterrupt();  // Resets the ALRT pin
		alert = false;
		gauge.sleep();  // Forces the MAX17043 into sleep mode
		while (true);
	}

	digitalWrite(led, true);
	double latestAccel = 0.0;
	if (useAccel)
	{
		accelAnalysis.update();
		latestAccel = accelAnalysis.getLatest();
		Serial.print(latestAccel); Serial.println(" ");
	}

	Serial.println(".");

	currentMode = Mode_blinkLeft;
	switch (currentMode)
	{
	case Mode_none:
		// when no special mode, play simple animation unless the g calculated is higher than the min limit
		if (breakAnim.isHigherThanMinLimit(latestAccel))
		{
			breakAnim.step(latestAccel);
			delay(100);
		}
		else
		{
			simpleAnim.step();
			delay(200);
		}
		break;

	case Mode_blinkLeft:
		turnAnim.step(0);
		break;

	case Mode_blinkRight:
		turnAnim.step(1);
		break;

	case Mode_warning:
		warningAnim.step();
		break;

	case Mode_manualBreaking:
		for (int i = 0; i<16; i++)
		{
			leftRingPixels.setPixelColor(i, breakColor);
			rightRingPixels.setPixelColor(i, breakColor);
			middleBarsPixels.setPixelColor(i, breakColor);
		}
		leftRingPixels.show();
		rightRingPixels.show();
		middleBarsPixels.show();
		break;
	}

	digitalWrite(led, false);
}
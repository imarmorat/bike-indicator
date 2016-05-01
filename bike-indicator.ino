#include "BreakingAnimation.h"
#include "TurnAnimation.h"
#include "SimpleAnimation.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "defs.h"

// https://github.com/adafruit/Adafruit_ADXL345
// https://github.com/adafruit/Adafruit_Sensor

int led = 13;
#define LEFTRING_PIN 1
#define RIGHTRING_PIN 0
#define BAR_PIN 2
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

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

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

void displaySensorDetails(void)
{
	sensor_t sensor;
	accel.getSensor(&sensor);
	Serial.println("------------------------------------");
	Serial.print("Sensor:       "); Serial.println(sensor.name);
	Serial.print("Driver Ver:   "); Serial.println(sensor.version);
	Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
	Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
	Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
	Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
	Serial.println("------------------------------------");
	Serial.println("");
	delay(500);
}

void setup() {
	Serial.begin(38400);
	Serial.println("helloo");
	pinMode(led, OUTPUT);
	pinMode(BREAKING_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNLEFT_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNRIGHT_BUTTON_PIN, INPUT_PULLUP);

	leftRingPixels.begin();
	leftRingPixels.setBrightness(60); 
	rightRingPixels.begin();
	rightRingPixels.setBrightness(60); 
	middleBarsPixels.begin();
	middleBarsPixels.setBrightness(60);

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
	breakAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);

	/* Initialise the sensor */
	if (!accel.begin())
	{
		/* There was a problem detecting the ADXL345 ... check your connections */
		Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
		while (1);
	}

	displaySensorDetails();
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
	uint8_t  i;
	//digitalWrite(led, HIGH);
	//delay(100);              
	//digitalWrite(led, LOW);    
	//delay(100);

	sensors_event_t event;
	accel.getEvent(&event);
	float calcdG = sqrt(
		event.acceleration.x*event.acceleration.x +
		event.acceleration.y*event.acceleration.y +
		event.acceleration.z*event.acceleration.z -
		SENSORS_GRAVITY_STANDARD * SENSORS_GRAVITY_STANDARD
	);

	/* Display the results (acceleration is measured in m/s^2) */
	Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
	Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
	Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" -- "); 
	Serial.print("G: "); Serial.print(calcdG); Serial.println(" g");

	switch (currentMode)
	{
	case Mode_none:
		simpleAnim.step();
		delay(500);
		break;

	case Mode_blinkLeft:
		turnAnim.step(0);
		break;

	case Mode_blinkRight:
		turnAnim.step(1);
		break;

	case Mode_warning:
		break;

	case Mode_manualBreaking:
		for (i = 0; i<16; i++)
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


}
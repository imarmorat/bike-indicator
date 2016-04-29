/*
Blink
Turns on an LED on for one second, then off for one second, repeatedly.

This example code is in the public domain.
*/

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
#include <OctoWS2811.h>
#include <Adafruit_NeoPixel.h>
int led = 13;
#define LEFTRING_PIN 1
#define RIGHTRING_PIN 0
#define BREAKING_BUTTON_PIN 4
#define TURNLEFT_BUTTON_PIN 5
#define TURNRIGHT_BUTTON_PIN 6
uint8_t offset = 0;

Adafruit_NeoPixel leftRingPixels = Adafruit_NeoPixel(16, LEFTRING_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel rightRingPixels = Adafruit_NeoPixel(16, RIGHTRING_PIN, NEO_RGBW + NEO_KHZ800);
uint32_t blinkerColor = leftRingPixels.Color(128, 255, 6);
uint32_t breakColor = leftRingPixels.Color(0, 255, 0); // green red blue format

enum Mode
{
	Mode_none = 0,
	Mode_blinkLeft = 1,
	Mode_blinkRight = 4,
	Mode_breaking = 2
};

Mode currentMode = Mode_none;

void setup() {
	// initialize the digital pin as an output.
	Serial.begin(38400);
	Serial.println("helloo");
	pinMode(led, OUTPUT);
	pinMode(BREAKING_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNLEFT_BUTTON_PIN, INPUT_PULLUP);
	pinMode(TURNRIGHT_BUTTON_PIN, INPUT_PULLUP);

	leftRingPixels.begin();
	leftRingPixels.setBrightness(60); // 1/3 brightness
	rightRingPixels.begin();
	rightRingPixels.setBrightness(60); // 1/3 brightness

	attachInterrupt(BREAKING_BUTTON_PIN, breakButtonActivated, FALLING); // interrrupt 1 is data read
	attachInterrupt(TURNLEFT_BUTTON_PIN, turnLeftButtonActivated, FALLING); // interrrupt 1 is data read
	attachInterrupt(TURNRIGHT_BUTTON_PIN, turnRightButtonActivated, FALLING); // interrrupt 1 is data read
}

void breakButtonActivated()
{
	cli();
	Serial.println("Breaking");
	currentMode = (currentMode == Mode_none ? Mode_breaking : Mode_none);
	sei();
}

void turnLeftButtonActivated()
{
	cli();
	Serial.println("Turning left");
	currentMode = (currentMode == Mode_none ? Mode_blinkLeft : Mode_none);
	sei();
}

void turnRightButtonActivated()
{
	cli();
	Serial.println("Turning right");
	currentMode = (currentMode == Mode_none ? Mode_blinkRight : Mode_none);
	sei();
}

// the loop routine runs over and over again forever:
void loop() {
	uint8_t  i;
	digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(100);               // wait for a second
	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
	delay(100);     
	Serial.println(currentMode);
	switch (currentMode)
	{
	case Mode_none:
		leftRingPixels.clear();
		rightRingPixels.clear();
		leftRingPixels.show();
		rightRingPixels.show();
		break;

	case Mode_blinkLeft:
		// wait for a second
		for (i = 0; i<16; i++) { leftRingPixels.setPixelColor(i, blinkerColor); }
		leftRingPixels.show();
		delay(500);

		for (i = 0; i<16; i++) { leftRingPixels.setPixelColor(i, 0x000000); }
		leftRingPixels.show();
		delay(500);
		break;


	case Mode_blinkRight:
		// wait for a second
		for (i = 0; i<16; i++) { rightRingPixels.setPixelColor(i, blinkerColor); }
		rightRingPixels.show();
		delay(500);

		for (i = 0; i<16; i++) { rightRingPixels.setPixelColor(i, 0x000000); }
		rightRingPixels.show();
		delay(500);
		break;

	case Mode_breaking:
		for (i = 0; i<16; i++) 
		{
			leftRingPixels.setPixelColor(i, breakColor);
			rightRingPixels.setPixelColor(i, breakColor);
		}
		leftRingPixels.show();
		rightRingPixels.show();
		break;
	}


}
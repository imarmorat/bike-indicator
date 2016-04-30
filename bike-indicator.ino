#include "TurnLeftAnimation.h"
#include "SimpleAnimation.h"
#include <Adafruit_NeoPixel.h>


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

SimpleAnimation a1;
SimpleAnimation a2;
SimpleAnimation a3;
TurnLeftAnimation tla;

enum Mode
{
	Mode_none = 0,
	Mode_blinkLeft = 1,
	Mode_blinkRight = 4,
	Mode_breaking = 2
};

Mode currentMode = Mode_none;

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

	tla.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	a1.init(); a2.init(); a3.init();
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
	//digitalWrite(led, HIGH);
	//delay(100);              
	//digitalWrite(led, LOW);    
	//delay(100);
	//Serial.println(currentMode);

	switch (currentMode)
	{
	case Mode_none:
		a1.step(&leftRingPixels);
		a2.step(&rightRingPixels);
		a3.step(&middleBarsPixels);
		delay(100);
		break;

	case Mode_blinkLeft:
		tla.step();
		//for (i = 0; i<16; i++) { leftRingPixels.setPixelColor(i, blinkerColor); }
		//leftRingPixels.show();
		//delay(500);

		//for (i = 0; i<16; i++) { leftRingPixels.setPixelColor(i, 0x000000); }
		//leftRingPixels.show();
		//delay(500);
		break;


	case Mode_blinkRight:
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
			middleBarsPixels.setPixelColor(i, breakColor);
		}
		leftRingPixels.show();
		rightRingPixels.show();
		middleBarsPixels.show();
		break;
	}


}
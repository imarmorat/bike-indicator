#include "Helpers.h"
#include "UserControlManager.h"
#include "ADS1x15.h"
#include "LiPoFuel.h"
#include "AccelAnalysis.h"
#include "WarningAnimation.h"
#include "BreakingAnimation.h"
#include "TurnAnimation.h"
#include "SimpleAnimation.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include "SSD1306.h"
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

Adafruit_SSD1306 display(6);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_ADS1115 ads;
UserControlManagerClass userControlManager;

AccelAnalysis accelAnalysis;
volatile boolean alert = false;
void lowPower() { alert = true; }
LiFuelGauge gauge(MAX17043, 0, lowPower);
bool useAccel = true;

Mode currentMode = Mode_none;
UserCtrl_Input latestInput = UserCtrl_none;

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

void setupLeds(Adafruit_NeoPixel * leds)
{
	leds->begin();
	leds->setBrightness(100);
	leds->clear();
	leds->show();
	Serial.println("[init::leds] init ok");
}

void setupDisplay(Adafruit_SSD1306 * display)
{
	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
	delay(2000);
	display->clearDisplay();
	display->setCursor(0, 0);
	display->setTextColor(WHITE, BLACK);
	display->setTextSize(1);
	Serial.println("[init::display] init ok");
}

void setupLipoGauge(LiFuelGauge * gauge)
{
	gauge->reset();
	gauge->setAlertThreshold(10);
	Serial.println(String("[init::gauge] Alert Threshold is set to ") + gauge->getAlertThreshold() + '%');
	Serial.print("[init::gauge] Charge: ");
	Serial.print(gauge->getSOC());  // Gets the battery's state of charge
	Serial.print("%, VBatt: ");
	Serial.print(gauge->getVoltage());  // Gets the battery voltage
	Serial.println('V');
}

void setupAccelerator(Adafruit_ADXL345_Unified * accelerator)
{
	if (!useAccel)
	{
		Serial.println("[init::accel] Accel disable");
		return;
	}

	if (!accelerator->begin())
	{
		Serial.println("[init::accel] Accel init failed");
		while (1);
	}
	else
	{
		Serial.println("[init::accel] Accel init ok");
		accelAnalysis.init(accelerator, 0, 0.3, 2.0, 6.0);
	}
}

void setupAnims()
{
	turnAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	simpleAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	warningAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	breakAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
}

void setupUserControls(Adafruit_ADS1115 * ads1115)
{
	userControlManager.init(ads1115);

	// get a reading to confirm correct initialization
	UserCtrl_Input input = userControlManager.getInput();
	Serial.println("[init::controls] init ok");
}

/*
	SETUP entry point
*/
void setup() {
	pinMode(led, OUTPUT);
	digitalWrite(led, true);

	Serial.begin(38400);
	Serial.println("** Bike Indictor **");
	Serial.println("[init] starting init...");

	setupLeds(&leftRingPixels);
	setupLeds(&rightRingPixels);
	setupLeds(&middleBarsPixels);
	setupAnims();
	setupDisplay(&display);
	setupAccelerator(&accel);
	setupLipoGauge(&gauge);
	setupUserControls(&ads);
	
	Serial.println("[init] init completed");
	digitalWrite(led, false);
}

//void breakButtonActivated()
//{
//	cli();
//	Serial.println("Breaking");
//	ChangeMode(Mode_manualBreaking);
//	sei();
//}

void updateDisplay()
{
	// [  main zone  ][indicator zone]
	uint16_t mainZoneX = 0, mainZoneY = 0, mainZoneW = 128 - 40, mainZoneH = 64;
	uint16_t indicatorZoneX = mainZoneW + 1, indicatorZoneY = 0, indicatorZoneW = 40, indicatorZoneH = 64;

	// update battery status
	if (!alert)
	{
		double battPercent = gauge.getSOC();
		if (battPercent > 1.0)
		{
			// powered by external
			display.setCursor(indicatorZoneX + 1, indicatorZoneY + 1);
			display.println("EXT");
		}
		else
		{
			// powered by battery
			display.drawRect(indicatorZoneX + 1, indicatorZoneY + 1, indicatorZoneW - 2, 20, WHITE);
			display.fillRect(indicatorZoneX + 2, indicatorZoneY + 2, (indicatorZoneW - 4) * battPercent, 18, WHITE);
		}
	}
	else
	{
		// low battery
		display.setCursor(indicatorZoneX + 1, indicatorZoneY + 1);
		display.println("LOW!");
	}
	
	// todo: update accel indicator
	// todo: update current mode (main zone)



	display.display();
}


/*
	LOOP entry point
*/
void loop() {
	//
	// Update input
	auto input = userControlManager.getInput();
	auto isInputDifferent = input != latestInput;
	if (isInputDifferent)
	{
		switch (input)
		{
			case UserCtrl_bottom: currentMode = Mode_none; break;
			case UserCtrl_top: currentMode = Mode_manualBreaking; break;
			case UserCtrl_click: currentMode = Mode_warning;  break;
			case UserCtrl_left: currentMode = Mode_blinkLeft; break;
			case UserCtrl_right: currentMode = Mode_blinkRight; break;
		}
	}

	//
	// Update accel
	if (useAccel)
	{
		accelAnalysis.update();
		auto latestAccel = accelAnalysis.getLatest();
	}

	//
	// Update battery
	if (alert)
	{
		Serial.println("Beware, Low Power!");
		Serial.println("Stopping");
		gauge.clearAlertInterrupt();  // Resets the ALRT pin
		alert = false;
		gauge.sleep();  // Forces the MAX17043 into sleep mode
		while (true);
	}

	updateDisplay();

	switch (currentMode)
	{
	case Mode_none:
		// when no special mode, play simple animation unless the g calculated is higher than the min limit
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

#include "Animation.h"
#include "TrailAnimation.h"
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

#define LED_PIN  13
#define LEFTRING_PIN 0
#define RIGHTRING_PIN 2
#define BAR_PIN 1

//
// Global vars
volatile boolean lowPowerAlert = false;
void lowPower() { lowPowerAlert = true; }
volatile boolean accelerationThresholdBreached = false;
void accelThreasholdBreachedCallback(int value) { accelerationThresholdBreached = true; }
bool isAccelerationDetectionEnabled = true;
Mode currentMode = Mode_none;
UserCtrl_Input latestInput = UserCtrl_none;
Animation * currentAnim = NULL;

//
// Hardware
Adafruit_NeoPixel leftRingPixels = Adafruit_NeoPixel(16, LEFTRING_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel rightRingPixels = Adafruit_NeoPixel(16, RIGHTRING_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel middleBarsPixels = Adafruit_NeoPixel(16, BAR_PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_SSD1306 display(6);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_ADS1115 ads;
LiFuelGauge gauge(MAX17043, 0, lowPower);

//
// Pre defined colors in GRB format
uint32_t blinkerColor = leftRingPixels.Color(128, 255, 6);
uint32_t breakColor = leftRingPixels.Color(0, 255, 0);

//
// Animations
TrailAnimation trailAnim;
SimpleAnimation simpleAnim;
TurnAnimation turnLeftAnim;
TurnAnimation turnRightAnim;
BreakingAnimation breakAnim;
WarningAnimation warningAnim;
int idlingAnimIndex = 0;

//
// Svc 
UserControlManagerClass userControlManager;
AccelAnalysis accelAnalysis;

void debug(String msg)
{
	Serial.println(msg);
	//display.println(msg);
	//display.display();
}

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
	debug("[init::leds] init ok");
}

void setupDisplay(Adafruit_SSD1306 * display)
{
	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
	delay(2000);
	display->clearDisplay();
	display->setCursor(0, 0);
	display->setTextColor(WHITE, BLACK);
	display->setTextSize(1);
	debug("[init::display] init ok");
}

void setupLipoGauge(LiFuelGauge * gauge)
{
	gauge->reset();
	gauge->setAlertThreshold(10);
	delay(1000);
	debug(String("[init::gauge] Alert Threshold is set to ") + gauge->getAlertThreshold() + '%');
	debug(String("[init::gauge] Charge: ") + gauge->getSOC() + "%, VBatt: " + gauge->getVoltage() + 'V');
}

void setupAccelerator(Adafruit_ADXL345_Unified * accelerator)
{
	if (!isAccelerationDetectionEnabled)
	{
		debug("[init::accel] Accel disable");
		return;
	}

	if (!accelerator->begin())
	{
		debug("[init::accel] Accel init failed");
		while (1);
	}
	else
	{
		Serial.println("[init::accel] Accel init ok");
		accelAnalysis.init(accelerator, 0, 0.3, 2.0, 6.0, accelThreasholdBreachedCallback);
	}
}

void setupAnims() 
{
	turnLeftAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	turnLeftAnim.setDirection(0);
	turnRightAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	turnRightAnim.setDirection(1);
	simpleAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	warningAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	breakAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	trailAnim.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);

	currentAnim = &trailAnim;
	currentAnim->reset();
}

void setupUserControls(Adafruit_ADS1115 * ads1115)
{
	userControlManager.init(ads1115);

	// get a reading to confirm correct initialization
	auto input = userControlManager.getInput();
	debug("[init::controls] init ok");
}

void updateDisplay()
{
	// [  main zone  ][indicator zone]
	uint16_t mainZoneX = 0, mainZoneY = 0, mainZoneW = 128 - 40, mainZoneH = 64;
	uint16_t indicatorZoneX = mainZoneW + 1, indicatorZoneY = 0, indicatorZoneW = 40, indicatorZoneH = 64;

	// update battery status
	if (!lowPowerAlert)
	{
		double battPercent = gauge.getSOC() / 100.0;
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

	//
	// Show accel indicator
	// TODO

	//
	// Update the main zone to reflect the current mode
	// TODO: using icons in the future?
	display.fillRect(mainZoneX, mainZoneY, mainZoneW, mainZoneH, BLACK);
	switch (currentMode)
	{
	case Mode_blinkLeft:
		display.drawTriangle(
			mainZoneX + mainZoneW - 10, mainZoneY + 10,
			mainZoneX + mainZoneW - 10, mainZoneY + mainZoneH - 10,
			mainZoneX + 10, mainZoneY + mainZoneH / 2,
			WHITE);
		break;

	case Mode_blinkRight:
		display.drawTriangle(
			mainZoneX + 10, mainZoneY + 10,
			mainZoneX + 10, mainZoneY + mainZoneH - 10,
			mainZoneX + mainZoneW - 10, mainZoneY + mainZoneH / 2,
			WHITE);
		break;

	case Mode_manualBreaking:
		display.drawRect(mainZoneX + 10, mainZoneY + 10, mainZoneX + mainZoneW - 10, mainZoneY + mainZoneH - 10, WHITE);
		break;

	case Mode_warning:
		display.drawCircle(mainZoneX + mainZoneW / 2 - 2, mainZoneY + mainZoneH / 2 - 2, 10, WHITE);
		break;

	case Mode_none:
		break;
	}

	display.display();
}

Animation * getNextIdlingAnim()
{
	int animCount = 2;
	idlingAnimIndex = (idlingAnimIndex + 1) % animCount;
	switch (idlingAnimIndex)
	{
	case 0: return &simpleAnim;
	case 1: return &trailAnim;
	default: return &simpleAnim;
	}
}

/*
	SETUP entry point
*/
void setup() {
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, true);
	Serial.begin(38400);

	debug("** Bike Indictor **");
	debug("[init] starting init...");
	setupDisplay(&display);

	setupLeds(&leftRingPixels);
	setupLeds(&rightRingPixels);
	setupLeds(&middleBarsPixels);
	setupAnims();
	setupAccelerator(&accel);
	setupLipoGauge(&gauge);
	setupUserControls(&ads);
	
	debug("[init] init completed");
	digitalWrite(LED_PIN, false);
}

/*
	LOOP entry point
*/
void loop() {
	digitalWrite(LED_PIN, true);
	//debug(String("[init::gauge] Charge: ") + gauge.getVoltage());

	//
	// Update input
	auto input = userControlManager.getInput();
	auto isInputDifferent = input != latestInput;
	if (isInputDifferent)
	{
		debug(String("[run] different input detected: ") + input);
		switch (input)
		{
		case UserCtrl_none: break;
		case UserCtrl_bottom: 
			currentMode = Mode_none; 
			currentAnim = getNextIdlingAnim();
			break;
		case UserCtrl_top: 
			currentMode = Mode_manualBreaking; 
			currentAnim = &breakAnim;
			break;
		case UserCtrl_click: 
			currentMode = Mode_warning;  
			currentAnim = &warningAnim;
			break;
		case UserCtrl_left: 
			currentMode = Mode_blinkLeft; 
			currentAnim = &turnLeftAnim;
			break;
		case UserCtrl_right: 
			currentMode = Mode_blinkRight; 
			currentAnim = &turnRightAnim;
			break;
		}

		if (input != UserCtrl_none)
			currentAnim->reset();
		latestInput = input;

	}

	//
	// Update accel
	if (isAccelerationDetectionEnabled)
		accelAnalysis.update();

	//
	// Update battery
	if (lowPowerAlert)
	{
		debug("Beware, Low Power!");
		debug("Stopping");
		gauge.clearAlertInterrupt();  // Resets the ALRT pin
		lowPowerAlert = false;
		gauge.sleep();  // Forces the MAX17043 into sleep mode
		while (true);
	}

	updateDisplay();
	currentAnim->step();

	digitalWrite(LED_PIN, false);
}

//void breakButtonActivated()
//{
//	cli();
//	Serial.println("Breaking");
//	ChangeMode(Mode_manualBreaking);
//	sei();
//}
#include "TurnAnimation.h"
#include "SimpleAnimation.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

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

SimpleAnimation a1;
SimpleAnimation a2;
SimpleAnimation a3;
TurnAnimation tla;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

enum Mode
{
	Mode_none = 0,
	Mode_blinkLeft = 1,
	Mode_blinkRight = 4,
	Mode_breaking = 2
};

Mode currentMode = Mode_none;


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

void displayDataRate(void)
{
	Serial.print("Data Rate:    ");

	switch (accel.getDataRate())
	{
	case ADXL345_DATARATE_3200_HZ:
		Serial.print("3200 ");
		break;
	case ADXL345_DATARATE_1600_HZ:
		Serial.print("1600 ");
		break;
	case ADXL345_DATARATE_800_HZ:
		Serial.print("800 ");
		break;
	case ADXL345_DATARATE_400_HZ:
		Serial.print("400 ");
		break;
	case ADXL345_DATARATE_200_HZ:
		Serial.print("200 ");
		break;
	case ADXL345_DATARATE_100_HZ:
		Serial.print("100 ");
		break;
	case ADXL345_DATARATE_50_HZ:
		Serial.print("50 ");
		break;
	case ADXL345_DATARATE_25_HZ:
		Serial.print("25 ");
		break;
	case ADXL345_DATARATE_12_5_HZ:
		Serial.print("12.5 ");
		break;
	case ADXL345_DATARATE_6_25HZ:
		Serial.print("6.25 ");
		break;
	case ADXL345_DATARATE_3_13_HZ:
		Serial.print("3.13 ");
		break;
	case ADXL345_DATARATE_1_56_HZ:
		Serial.print("1.56 ");
		break;
	case ADXL345_DATARATE_0_78_HZ:
		Serial.print("0.78 ");
		break;
	case ADXL345_DATARATE_0_39_HZ:
		Serial.print("0.39 ");
		break;
	case ADXL345_DATARATE_0_20_HZ:
		Serial.print("0.20 ");
		break;
	case ADXL345_DATARATE_0_10_HZ:
		Serial.print("0.10 ");
		break;
	default:
		Serial.print("???? ");
		break;
	}
	Serial.println(" Hz");
}

void displayRange(void)
{
	Serial.print("Range:         +/- ");

	switch (accel.getRange())
	{
	case ADXL345_RANGE_16_G:
		Serial.print("16 ");
		break;
	case ADXL345_RANGE_8_G:
		Serial.print("8 ");
		break;
	case ADXL345_RANGE_4_G:
		Serial.print("4 ");
		break;
	case ADXL345_RANGE_2_G:
		Serial.print("2 ");
		break;
	default:
		Serial.print("?? ");
		break;
	}
	Serial.println(" g");
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

	tla.init(&leftRingPixels, &middleBarsPixels, &rightRingPixels);
	a1.init(); a2.init(); a3.init();

	/* Initialise the sensor */
	if (!accel.begin())
	{
		/* There was a problem detecting the ADXL345 ... check your connections */
		Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
		while (1);
	}
	//displaySetRange(ADXL345_RANGE_4_G);
	displaySensorDetails();

	/* Display additional settings (outside the scope of sensor_t) */
	displayDataRate();
	displayRange();
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
	sensors_event_t event;
	accel.getEvent(&event);

	/* Display the results (acceleration is measured in m/s^2) */
	Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
	Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
	Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");
	delay(500);


	switch (currentMode)
	{
	case Mode_none:
		a1.step(&leftRingPixels);
		a2.step(&rightRingPixels);
		a3.step(&middleBarsPixels);
		delay(100);
		break;

	case Mode_blinkLeft:
		tla.step(0);
		break;


	case Mode_blinkRight:
		tla.step(1);
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
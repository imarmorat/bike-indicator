// Utils.h

#ifndef _UTILS_h
#define _UTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "i2c_t3.h"

static void Utils::print_scan_status(uint8_t target, bool all)
{
	switch (Wire1.status())
	{
	case I2C_WAITING:
		Serial.print("Addr 0x");
		Serial.print(target, HEX);
		Serial.print(" ACK\n");
		break;
	case I2C_ADDR_NAK:
		if (all)
		{
			Serial.print("Addr 0x");
			Serial.print(target, HEX);
			Serial.print("\n");
		}
		break;
	default:
		break;
	}
}

static void Utils::utils_scanI2C(Wire * wire)
{
	Serial.print("---------------------------------------------------\n");
	Serial.print("Starting scan...\n");
	Wire1.begin();
	uint8_t target; // slave addr
	bool all;
	for (target = 1; target <= 0x7F; target++) // sweep addr, skip general call
	{
		Wire1.beginTransmission(target);       // slave addr
		Wire1.endTransmission();               // no data, just addr
		print_scan_status(target, true);
	}
	Serial.print("---------------------------------------------------\n");
}

#endif


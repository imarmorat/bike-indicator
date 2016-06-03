#include "Helpers.h"
#include "i2c_t3.h"

void Helpers::print_scan_status(uint8_t target, bool all)
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

void Helpers::utils_scanI2C(i2c_t3 * wire)
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

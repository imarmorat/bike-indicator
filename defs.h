#pragma once

/*
Important: enums have to be in a .h file if passed as function parameter because of a weird bug in Adruino
*/

enum Mode
{
	Mode_none = 0,
	Mode_blinkLeft = 1,
	Mode_blinkRight = 4,
	Mode_manualBreaking = 2,
	Mode_warning = 8
};

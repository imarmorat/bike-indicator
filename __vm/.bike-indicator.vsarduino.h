/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Teensy 3.2 / 3.1, Platform=teensy3, Package=teensy
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __HARDWARE_MK20dx256__
#define __HARDWARE_MK20DX256__
#define __MK20DX256__
#define TEENSYDUINO 128
#define ARDUINO 10608
#define F_CPU 96000000
#define USB_SERIAL
#define LAYOUT_US_ENGLISH
#define __cplusplus 201103L
#define __arm__
#define __ARM__
#define __extension__
#define  __attribute__(x)
typedef void *__builtin_va_list;
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define NEW_H

#include <arduino.h>
#define __arm__
#define __ARM__
#define __extension__
#define  __attribute__(x)
typedef void *__builtin_va_list;
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __disable_irq() __asm__ volatile("");
#define __enable_irq()	__asm__ volatile("");

//#define sei() __enable_irq()
//#define cli() __disable_irq()
//#define interrupts() __enable_irq()
//#define noInterrupts() __disable_irq()

#define NEW_H

void lowPower();
inline void debug(String msg);
void ChangeMode(Mode newMode);
void setupLeds(Adafruit_NeoPixel * leds);
void setupDisplay(Adafruit_SSD1306 * display);
void setupLipoGauge(LiFuelGauge * gauge);
void setupAccelerator(Adafruit_ADXL345_Unified * accelerator);
void setupAnims();
void setupUserControls(Adafruit_ADS1115 * ads1115);
void updateDisplay();
//
//

#include <bike-indicator.ino>
#include <ADS1x15.cpp>
#include <ADS1x15.h>
#include <ADXL345.cpp>
#include <ADXL345.h>
#include <AccelAnalysis.cpp>
#include <AccelAnalysis.h>
#include <Animation.cpp>
#include <Animation.h>
#include <BreakingAnimation.cpp>
#include <BreakingAnimation.h>
#include <Helpers.cpp>
#include <Helpers.h>
#include <LiPoFuel.cpp>
#include <LiPoFuel.h>
#include <SSD1306.cpp>
#include <SSD1306.h>
#include <SimpleAnimation.cpp>
#include <SimpleAnimation.h>
#include <TurnAnimation.cpp>
#include <TurnAnimation.h>
#include <UserControlManager.cpp>
#include <UserControlManager.h>
#include <WarningAnimation.cpp>
#include <WarningAnimation.h>
#include <defs.h>
#include <i2c_t3.cpp>
#include <i2c_t3.h>
#endif

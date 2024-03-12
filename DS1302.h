#ifndef _H_DS1302_H
#define _H_DS1302_H
#if (defined STM32L011xx) || (defined STM32L021xx) || \
	(defined STM32L031xx) || (defined STM32L041xx) || \
	(defined STM32L051xx) || (defined STM32L052xx) || (defined STM32L053xx) || \
	(defined STM32L061xx) || (defined STM32L062xx) || (defined STM32L063xx) || \
	(defined STM32L071xx) || (defined STM32L072xx) || (defined STM32L073xx) || \
	(defined STM32L081xx) || (defined STM32L082xx) || (defined STM32L083xx)
#include "stm32l0xx_hal.h"
#elif defined (STM32L412xx) || defined (STM32L422xx) || \
	defined (STM32L431xx) || (defined STM32L432xx) || defined (STM32L433xx) || defined (STM32L442xx) || defined (STM32L443xx) || \
	defined (STM32L451xx) || defined (STM32L452xx) || defined (STM32L462xx) || \
	defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || \
    defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#include "stm32l4xx_hal.h"
#elif defined (STM32F7xx)
#include "stm32f7xx_hal.h"
#elif defined (STM32G070xx)
#include "stm32g0xx_hal.h"
#else
#error Platform not implemented
#endif

enum Register {
	kSecondReg = 0,
	kMinuteReg = 1,
	kHourReg = 2,
	kDateReg = 3,
	kMonthReg = 4,
	kDayReg = 5,
	kYearReg = 6,
	kWriteProtectReg = 7,
	kRamAddress0 = 32
};

enum Command {
	kClockBurstRead = 0xBF,
	kClockBurstWrite = 0xBE,
	kRamBurstRead = 0xFF,
	kRamBurstWrite = 0xFE
};
void rtc_init(void);
uint8_t bcd_to_dec(uint8_t bcd);
uint8_t rtc_read_reg(uint8_t reg);
void rtc_write_reg(uint8_t reg, uint8_t num);
void rtc_set_time(uint8_t year, uint8_t month, uint8_t date, uint8_t hour,
		uint8_t min, uint8_t sec);

#endif
